#include "GameManager.h"

void GameManager::runGameThread(std::shared_ptr<GameInfo> gameInfo) {

	// Infinite loop - only stop when there is no more games in the game manager
	while (gameInfo->getPlayerWon() != -1) {

		// Create a new game, running it and then requesting another
		std::pair<std::shared_ptr<IBattleshipGameAlgo>, std::shared_ptr<IBattleshipGameAlgo>> algos = gameInfo->getPlayersAlgos();
		std::unique_ptr<Game> newGame = std::make_unique<Game>(algos.first, algos.second, gameInfo->getBoard());

		this->getGame(gameInfo);
	}
}

void GameManager::getGame(std::shared_ptr<GameInfo> game) {

	// Lock for the threads, get the game from all the games
	std::mutex lock;
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

GameManager::GameManager(std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData) {

	this->allGamesData = allGamesData;
}

void GameManager::loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles, std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls) {

	try
	{
		std::string directoryPath = ((*dllsFiles).at(Utilities::NUMBER_DLLS)).substr(0, ((*dllsFiles).at(Utilities::NUMBER_DLLS)).find_last_of("/\\"));
		for (int i = 0; i < Utilities::NUMBER_DLLS; ++i)
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

		}
	}
	catch (std::exception& e)
	{
		throw e;
	}
}