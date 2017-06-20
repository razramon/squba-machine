#include "GameManager.h"


bool GameManager::sortPlayers(const std::pair<std::string, std::vector<int>>& playerA,const std::pair<std::string, std::vector<int>>& playerB) {

	double playerAWinRate = static_cast<double>(playerA.second.at(0)) / (playerA.second.at(0) + playerA.second.at(1));
	double playerBWinRate = static_cast<double>(playerB.second.at(0)) / (playerB.second.at(0) + playerB.second.at(1));
	return playerAWinRate > playerBWinRate;
}

void GameManager::runGameThread() {
	std::cout << "In thread number: " << std::this_thread::get_id() << std::endl; //TODO:: DELETE THIS LINE!!
	// Infinite loop - only stop when there is no more games in the game manager
	while (gameNumber < allGamesData.size()) {
		// Create a new game, run it and then request another
		std::shared_ptr<GameBasicData> gameBD = nullptr;
		getGame(gameBD); //getGame() does the lock stuff

		if( gameBD == nullptr)
		{
			break;
		}

		std::unique_ptr<Game> g = std::make_unique<Game>((*gameBD).dllA.first, (*gameBD).dllB.first,
				(*gameBD).dllA.second, (*gameBD).dllB.second, (*(*gameBD).board),
				boardsShips[(*gameBD).indexOfGameShips], (*(*(*gameBD).board).first).rows(),
				(*(*(*gameBD).board).first).cols(), (*(*(*gameBD).board).first).depth());


		std::unique_ptr<GameInfo> gameResult = g->game();
		
		addNewGameInfo(gameResult);

		this->printRound(); 
	}
}

void GameManager::getGame(std::shared_ptr<GameBasicData>& gameBasicData) {

	// Lock for the threads, get the game from all the games
	std::mutex lock;
	lock.lock(); //to make sure gameNumber doesn't change while we're checking it!

	// Check if there are games left unplayed
	if (gameNumber >= allGamesData.size()) { //no more games left!
		gameBasicData = nullptr;
	}
	else
	{
		//Note: next line makes allGamesData[gameNumber] invalid (because we used move) - but that's OK: won't use it again
		gameBasicData = std::move(allGamesData[gameNumber]); 
		++gameNumber;
	}

	lock.unlock();
}


// Add to the playerInfo a new gameData, for both players.
void GameManager::addNewGameInfo(std::unique_ptr<GameInfo>& game) {
	for (int indexPlayer = 0; indexPlayer < this->allPlayersInfo.size(); indexPlayer++) {

		if (allPlayersInfo[indexPlayer]->getPlayerName() == game->getPlayerNames().first ||
			allPlayersInfo[indexPlayer]->getPlayerName() == game->getPlayerNames().second) 
		{
			allPlayersInfo[indexPlayer]->addNewGame(game);
		}
	}
	
}

void GameManager::setNumberThreads(int numberThreads) {

	this->numberThreads = numberThreads;
}

void GameManager::startGames() {

	this->gameNumber = 0;

	size_t threadsToCreate = (allGamesData.size() < numberThreads) ? allGamesData.size() : numberThreads;
	
	// Creating threads according to the number of threads
	// Note: in case there are more threads than games to play, creates only needed threads
	for (size_t indexThread = 0; indexThread < threadsToCreate; indexThread++) {
		std::thread gameThread(&GameManager::runGameThread, this);
		gameThread.join(); //wait for all tournament to finish
	}
}

GameManager::GameManager(std::unique_ptr<std::vector<std::string>>& dllsFiles,
	std::unique_ptr<std::vector<std::string>>& boardFiles, int numOfThreads) :
			numberThreads(numOfThreads), gameNumber(NOT_INIT), roundNumber(1),
			allGamesData(), allPlayersInfo(),
			dlls(), boards(), boardsShips()
{

	this->loadAllDlls(dllsFiles);
	if (dlls.size() < 2)
	{
		//TODO:: i'm not sure if we need to write to the logger...
		throw Exception("Error: not enough dlls were loaded");
	}

	this->loadAllBoards(boardFiles);
	if (boards.size() == 0)
	{		
		//TODO:: i'm not sure if we need to write to the logger...
		throw Exception("Error: no valid boards were found");
	}
	this->divideToGames();

	std::cout << "Number of legal players: " << this->allPlayersInfo.size() << std::endl;
	std::cout << "Number of legal boards: " << this->boards.size() << std::endl;
	
	//TODO: add printing of critical errors.(?)
}

void GameManager::loadAllDlls(std::unique_ptr<std::vector<std::string>>& dllsFiles) {

	std::string directoryPath = ((*dllsFiles).at((*dllsFiles).size()-1)).substr(0, ((*dllsFiles).at((*dllsFiles).size()-1)).find_last_of("/\\"));

	for (int i = 0; i < (*dllsFiles).size(); ++i)
	{
		// Load dynamic library
		HINSTANCE hDll = LoadLibraryA((*dllsFiles).at(i).c_str());
		if (!hDll)
		{
			Logger::instance().log("Could not load DLL", Logger::LogLevelError);
			continue;
		}
		GetAlgoFuncType getAlgoFunc = reinterpret_cast<GetAlgoFuncType>(GetProcAddress(hDll, "GetAlgorithm"));
		if (!getAlgoFunc)
		{
			Logger::instance().log("Could not load DLL", Logger::LogLevelError);
			continue;
		}
		size_t firstIndex = ((*dllsFiles).at(i)).find_last_of("/\\") + 1;
		size_t lastIndex = ((*dllsFiles).at(i)).find_last_of(".dll") - 4;
		std::string playerName = ((*dllsFiles).at(i)).substr(firstIndex, (lastIndex - firstIndex)+1);
		std::cout << "playerName is: " << playerName << std::endl;
		dlls.push_back(std::make_unique<std::pair<std::string, GetAlgoFuncType>>(std::make_pair(playerName,getAlgoFunc)));
		allPlayersInfo.push_back(std::make_shared<PlayerInfo>(playerName));
	}
}

void GameManager::loadAllBoards(std::unique_ptr<std::vector<std::string>>& boardFiles)
{
	for (std::vector<std::string>::iterator boardPath = (*boardFiles).begin(); boardPath != (*boardFiles).end(); ++boardPath)
	{
		try
		{
			int rows, cols, depth;
			std::unique_ptr<boardType> baseBoard = BoardCreator::getBoardFromFile((*boardPath).c_str(), rows, cols, depth);
			std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector >> shipsOfBoard = BoardCreator::checkBoard(baseBoard, rows, cols, depth);
			
			if (shipsOfBoard == nullptr)
			{
				Logger::instance().log("Invalid board", Logger::LogLevelError);
				continue; //continue to next board, this one's invalid
			}
			std::shared_ptr<boardType> cleanBaseBoard = BoardCreator::getBoardFromShips(shipsOfBoard->first, rows, cols, depth);
			BoardCreator::updateShipsInBoard(cleanBaseBoard, shipsOfBoard->second);
			boardsShips.push_back(shipsOfBoard);
			std::unique_ptr<Board> b0 = std::make_unique<Board>(rows, cols, depth, cleanBaseBoard, NOT_INIT);

			(*b0).setPlayerNumber(PLAYER_A); //so that the first board of the pair will match playerA
			std::unique_ptr<Board> b1 = std::make_unique<Board>(b0);
			(*b1).setPlayerNumber(PLAYER_B); //so that the second board of the pair will match playerB

			auto pairBoards = std::make_pair<std::shared_ptr<Board>, std::shared_ptr<Board>>(std::move(b0), std::move(b1));
			boards.push_back(std::make_unique<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>>(std::move(pairBoards)));

		} 
		catch(std::exception& e)
		{
			std::string errorStr("Could not load Board: ");
			errorStr.append(e.what());
			Logger::instance().log(errorStr, Logger::LogLevelError);
		}
	}
}


void GameManager::divideToGames() {

	// Loop over the first dll to enters
	for (int i = 0; i < dlls.size(); i++) {

		// Loop over the second dll to enter
		for (int j = i + 1; j < dlls.size(); j++) {

			// Loop over the boards to enter
			for (int indexBoard = 0; indexBoard < boards.size(); indexBoard++) {

				// Creating a new pointer, pushing the games to the collection

				allGamesData.push_back(std::make_unique<GameBasicData>((*dlls[i]), (*dlls[j]), (*boards[indexBoard]), indexBoard));
				allGamesData.push_back(std::make_unique<GameBasicData>((*dlls[j]), (*dlls[i]), (*boards[indexBoard]), indexBoard));
			}
		}
	}
}



void GameManager::printRound() {

	std::vector<std::pair<std::string,std::vector<int>>> playersToPrint;
	size_t maxLengthName = 0;


	//Locks "print_lock", a lock that will die at the end of this block:
	// == when this function finishes.
	std::mutex print_mutex;
	std::lock_guard<std::mutex> guard(print_mutex); 

	// Checking if all the players have played the round. if not, don't print. if so, print.
	for (std::shared_ptr<PlayerInfo> player : allPlayersInfo) {

		if (!player->hasGameInRound(this->roundNumber))
		{
			return; //because the rest of the function isn't relevant
		}
		//otherwise:
		playersToPrint.push_back(std::make_pair(player->getPlayerName(), player->getRoundPoints(this->roundNumber)));
		maxLengthName = maxLengthName > player->getPlayerName().length() ? maxLengthName : player->getPlayerName().length();
	}

	std::sort(playersToPrint.begin(), playersToPrint.end(), sortPlayers);

	// Head row
	std::cout << std::left << std::setfill(' ');
	std::cout << std::left << std::setw(6) << "#";
	std::cout << std::left << std::setw(maxLengthName + 4) << "Team Name";
	std::cout << std::left << std::setw(8) << "Wins";
	std::cout << std::left << std::setw(8) << "Losses";
	std::cout << std::left << std::setw(8) << "%";
	std::cout << std::left << std::setw(8) << "Pts For";
	std::cout << std::left << std::setw(8) << "Pts Against" << std::endl;

	std::cout << std::setw(50 + maxLengthName) << " " << std::endl;
	int indexRow = 1;
	std::vector<int> playerScores;

	// Printing the rows with the data
	for (std::pair<std::string, std::vector<int>> printPlayer: playersToPrint) {

		playerScores = printPlayer.second;

		std::cout << std::left << std::setw(6) << std::to_string(indexRow) + "." << std::setw(maxLengthName + 4) << printPlayer.first;
		std::cout << std::left << std::setw(8) << playerScores.at(0) << std::setw(8) << playerScores.at(1);
		std::cout << std::left << std::setw(8) << std::setprecision(2) << std::fixed << (static_cast<double>(playerScores.at(0)) / (playerScores.at(1) + playerScores.at(0)) * 100);
		std::cout << std::left << std::setw(8) << playerScores.at(2) << std::setw(8) << playerScores.at(3) << std::endl;
		indexRow++;
	}

	++roundNumber;
}