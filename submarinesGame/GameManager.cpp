#include "GameManager.h"
std::mutex lock;


bool GameManager::sortPlayers(std::pair<std::string, std::vector<int>> playerA, std::pair<std::string, std::vector<int>> playerB) {

	double playerAWinRate = playerA.second.at(0) / (playerA.second.at(0) + playerA.second.at(1));
	double playerBWinRate = playerB.second.at(0) / (playerB.second.at(0) + playerB.second.at(1));
	return playerAWinRate > playerBWinRate;
}


void GameManager::runGameThread(std::shared_ptr<GameInfo> gameInfo) {

	// Infinite loop - only stop when there is no more games in the game manager
	while (gameInfo->getPlayerWon() != -1) {

		// Create a new game, running it and then requesting another
		std::pair<std::shared_ptr<IBattleshipGameAlgo>, std::shared_ptr<IBattleshipGameAlgo>> algos = gameInfo->getPlayersAlgos();
		std::unique_ptr<Game> newGame = std::make_unique<Game>(algos.first, algos.second, gameInfo->getBoard(), );

		this->printRound();
		this->getGame(gameInfo);
	}
}

void GameManager::getGame(std::shared_ptr<GameInfo> game) {

	// Lock for the threads, get the game from all the games

	lock.lock();

	// Check if there are games left unplayed
	if (this->gameNumber >= (*this->allGamesData).size()) {

		game->setPlayerWon(-1);
	}
	else {

		game = (*this->allGamesData)[this->gameNumber];
	}
	gameNumber++;

	lock.unlock();
}

void GameManager::setNumberThreads(int numberThreads) {

	this->numberThreads = numberThreads;
}

void GameManager::startGames() {

	// For the getGame - will know what game wasn't played yet
	this->gameNumber = this->numberThreads;

	// Creating threads according to the number of threads
	for (int indexThread = 0; indexThread < this->numberThreads; indexThread++) {

		std::thread gameThread(&GameManager::runGameThread, (*this->allGamesData)[indexThread]);
		gameThread.join();
	}
}

GameManager::GameManager(ptrToVecOfGameInfoPtrs allGamesData, ptrToVecOfPlayerInfoPtrs allPlayersInfo) :
			numberThreads(NOT_INIT), gameNumber(NOT_INIT), roundNumber(1), allGamesData(allGamesData),
			allPlayersInfo(allPlayersInfo){
}

void GameManager::loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles, std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls, ptrToVecOfPlayerInfoPtrs allPlayersInfo) {

	try
	{
		std::string directoryPath = ((*dllsFiles).at((*dllsFiles).size())).substr(0, ((*dllsFiles).at((*dllsFiles).size())).find_last_of("/\\"));
		for (int i = 0; i < (*dllsFiles).size(); ++i)
		{
			// Load dynamic library
			HINSTANCE hDll = LoadLibraryA((*dllsFiles).at(i).c_str());
			if (!hDll)
			{
				throw Exception(exceptionInfo(CANNOT_LOAD_DLL, (*dllsFiles).at(i)));
			}
			std::unique_ptr<IBattleshipGameAlgo> getAlgoFunc = std::make_unique<IBattleshipGameAlgo>((GetAlgoFuncType)GetProcAddress(hDll, "GetAlgorithm"));
			if (!getAlgoFunc)
			{
				throw Exception(exceptionInfo(CANNOT_LOAD_DLL, (*dllsFiles).at(i)));
			}
			
			(*dlls).push_back(getAlgoFunc);

			std::string playerName = (*dllsFiles).at(i).substr((*dllsFiles).at(i).find_first_of('.')).substr(0, (*dllsFiles).at(i).find_first_of('.'));
			(*allPlayersInfo).push_back(std::make_shared<PlayerInfo>(i, playerName));
		}
	}
	catch (std::exception& e)
	{
		throw e;
	}
}

void GameManager::divideToGames(std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls, std::shared_ptr<std::vector<Board>> boards) {

	// Loop over the first dll to enters
	for (int i = 0; i < (*dlls).size(); i++) {

		// Loop over the second dll to enter
		for (int j = i + 1; j < (*dlls).size(); j++) {

			// Loop over the boards to enter
			for (int indexBoard = 0; indexBoard < (*boards).size(); indexBoard++) {

				// Creating a new pointer, pushing the games to the collection
				
				std::unique_ptr<GameInfo> game = std::make_unique<GameInfo>((*dlls)[i], (*dlls)[j], (*boards)[indexBoard]);
				allGamesData.push_back(std::move(game));
				game = std::make_unique<GameInfo>((*dlls)[j], (*dlls)[i], (*boards)[indexBoard]);
				allGamesData.push_back(std::move(game));
			}
		}
	}
}



void GameManager::printRound() {

	lock.lock();

	bool canPrint = true;
	std::vector<std::pair<std::string,std::vector<int>>> playersToPrint;
	int maxLengthName = 0;

	// Checking if all the players have played the round. if not, don't print. if so, print.
	for (std::shared_ptr<PlayerInfo> player : *(this->allPlayersInfo)) {

		if (!player->hasGameInRound(this->roundNumber))
		{
			canPrint = false;
			break;
		}
		else {

			playersToPrint.push_back(std::make_pair(player->getPlayerName(), player->getRoundPoints(this->roundNumber)));
			maxLengthName = maxLengthName > player->getPlayerName().length() ? maxLengthName : player->getPlayerName().length();
		}
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

	lock.unlock();
}