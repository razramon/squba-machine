#include "GameManager.h"
std::mutex lockMutex;

bool GameManager::sortPlayers(std::pair<std::string, std::vector<int>> playerA, std::pair<std::string, std::vector<int>> playerB) {

	double playerAWinRate = playerA.second.at(0) / (playerA.second.at(0) + playerA.second.at(1));
	double playerBWinRate = playerB.second.at(0) / (playerB.second.at(0) + playerB.second.at(1));
	return playerAWinRate > playerBWinRate;
}

/*
 * TODO:: edit function, change signature: because GameInfo doesn't contain GameData (to create a new Game)
 */
void GameManager::runGameThread(std::shared_ptr<GameInfo> gameInfo) {

	// Infinite loop - only stop when there is no more games in the game manager
	while (gameInfo->getMoreGamesLeft()) {

		// Create a new game, running it and then requesting another
		std::pair<std::shared_ptr<IBattleshipGameAlgo>, std::shared_ptr<IBattleshipGameAlgo>> algos = gameInfo->getPlayersAlgos();
		std::unique_ptr<Game> newGame = std::make_unique<Game>(algos.first, algos.second, gameInfo->getBoard(), );

		// Lock for the threads, get the game from all the games
		lockMutex.lock();

		this->addNewGameInfo(newGame->game());
		this->printRound();
		this->getGame(gameInfo);

		lockMutex.unlock();
	}
}


void GameManager::getGame(std::shared_ptr<GameInfo> game) {

	// Check if there are games left unplayed
	if (gameNumber >= allGamesData.size()) { //no more games left!
		game->setMoreGamesLeft(false);
	}
	else {

		game = std::move(allGamesData[gameNumber]);
	}

	gameNumber++;
}

void GameManager::addNewGameInfo(std::unique_ptr<GameInfo> game) {

	for (int indexPlayer = 0; indexPlayer < this->allPlayersInfo.size(); indexPlayer++) {

		if (this->allPlayersInfo.at(indexPlayer)->getPlayerName() == game->getPlayerNames().first || this->allPlayersInfo.at(indexPlayer)->getPlayerName() == game->getPlayerNames().second) {

			this->allPlayersInfo.at(indexPlayer)->addNewGame(std::move(game));
		}
	}
	
}

void GameManager::setNumberThreads(int numberThreads) {

	this->numberThreads = numberThreads;
}

//TODO:: "startGames()" might be problamatic when there are more threads than games
void GameManager::startGames() {

	// For the getGame - will know what game wasn't played yet
	this->gameNumber = this->numberThreads;

	// Creating threads according to the number of threads
	for (int indexThread = 0; indexThread < this->numberThreads; indexThread++) {

		std::thread gameThread(&GameManager::runGameThread, allGamesData[indexThread]);

		gameThread.join();
	}
}

GameManager::GameManager(std::shared_ptr<std::vector<std::string>> dllsFiles, std::shared_ptr<std::vector<std::string>> boardFiles) :
			numberThreads(NOT_INIT), gameNumber(NOT_INIT), roundNumber(1),
			allGamesData(), allGamesResults(), allPlayersInfo(),
			dlls(), boards()
{
		/*TODO:: 
		* might need a signature change (get number of threads as an argument?)
		* create all dlls using loadAllDlls(),
		* create all boards using loadAllBoards()
		* create combination using divideToGames()
		* run the tournament!
		*/

}

void GameManager::loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles) {

	std::string directoryPath = ((*dllsFiles).at((*dllsFiles).size())).substr(0, ((*dllsFiles).at((*dllsFiles).size())).find_last_of("/\\"));

	for (int i = 0; i < (*dllsFiles).size(); ++i)
	{
		// Load dynamic library
		HINSTANCE hDll = LoadLibraryA((*dllsFiles).at(i).c_str());
		if (!hDll)
		{
			Logger::instance().log("Could not load DLL", Logger::kLogLevelError);
			continue;
		}
		GetAlgoFuncType getAlgoFunc = reinterpret_cast<GetAlgoFuncType>(GetProcAddress(hDll, "GetAlgorithm"));
		if (!getAlgoFunc)
		{
			Logger::instance().log("Could not load DLL", Logger::kLogLevelError);
			continue;
		}
		std::string playerName = ((*dllsFiles).at((*dllsFiles).size())).substr(((*dllsFiles).at((*dllsFiles).size())).find_last_of("/\\"), ((*dllsFiles).at((*dllsFiles).size())).length());
		dlls.push_back(std::make_unique<std::pair<std::string, GetAlgoFuncType>>(std::make_pair(playerName,getAlgoFunc)));
		allPlayersInfo.push_back(std::make_shared<PlayerInfo>(playerName));
	}
}

void GameManager::loadAllBoards(std::shared_ptr<std::vector<std::string>> boardFiles)
{
	for (std::vector<std::string>::iterator boardPath = (*boardFiles).begin(); boardPath != (*boardFiles).end(); ++boardPath)
	{
		try
		{
			int rows, cols, depth;
			std::unique_ptr<boardType> baseBoard = std::move(BoardCreator::getBoardFromFile((*boardPath).c_str(), rows, cols, depth));
			std::unique_ptr<Board> b0 = std::make_unique<Board>(rows, cols, depth, baseBoard, NOT_INIT);

			(*b0).setPlayerNumber(PLAYER_A); //so that the first board of the pair will match playerA
			std::unique_ptr<Board> b1 = std::make_unique<Board>(b0);
			(*b1).setPlayerNumber(PLAYER_B); //so that the second board of the pair will match playerB

			auto pairBoards = std::make_pair<std::shared_ptr<Board>, std::shared_ptr<Board>>(std::move(b0), std::move(b1));
			boards.push_back(std::make_unique<std::pair<std::shared_ptr<Board>, std::shared_ptr<Board>>>(std::move(pairBoards)));

		} 
		catch(std::exception& e)
		{
			Logger::instance().log("Could not load Board", Logger::kLogLevelError);
			continue;
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
				allGamesData.push_back(std::make_unique<GameBasicData>((*dlls[i]), (*dlls[j]), (*boards[indexBoard])));
				allGamesData.push_back(std::make_unique<GameBasicData>((*dlls[j]), (*dlls[i]), (*boards[indexBoard])));
			}
		}
	}
}



void GameManager::printRound() {

	bool canPrint = true;
	std::vector<std::pair<std::string,std::vector<int>>> playersToPrint;
	int maxLengthName = 0;

	// Checking if all the players have played the round. if not, don't print. if so, print.
	for (std::shared_ptr<PlayerInfo> player : allPlayersInfo) {

		if (!player->hasGameInRound(this->roundNumber))
		{
			canPrint = false;
			break;
		}
		//otherwise:
		playersToPrint.push_back(std::make_pair(player->getPlayerName(), player->getRoundPoints(this->roundNumber)));
		maxLengthName = maxLengthName > player->getPlayerName().length() ? maxLengthName : player->getPlayerName().length();
	}

	std::sort(playersToPrint.begin(), playersToPrint.end(), sortPlayers);
		
	if (canPrint) {

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
			std::cout << std::left << std::setw(6) << indexRow << std::setw(maxLengthName + 4) << printPlayer.first;
			std::cout << std::left << std::setw(8) << playerScores.at(0) << std::setw(8) << playerScores.at(1);
			std::cout << std::left << std::setw(8) << (playerScores.at(0) / (playerScores.at(1) + playerScores.at(0)) * 100);
			std::cout << std::left << std::setw(8) << playerScores.at(2) << std::setw(8) << playerScores.at(3) << std::endl;
		}
		this->roundNumber++;
	}

}