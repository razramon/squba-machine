#include "GameManager.h"

void GameManager::runGameThread(std::shared_ptr<GameInfo> gameInfo) {

	while (gameInfo->getPlayerWon != -1) {

		std::pair<std::shared_ptr<IBattleshipGameAlgo>, std::shared_ptr<IBattleshipGameAlgo>> algos = gameInfo->getPlayersAlgos();
		std::unique_ptr<Game> newGame = std::make_unique<Game>(algos.first, algos.second, gameInfo->getBoard());

		this->getGame(gameInfo);
	}
}

void GameManager::getGame(std::shared_ptr<GameInfo> game) {

	std::mutex lock;
	lock.lock();

	if (this->gameNumber > (*this->allGamesData).size()) {

		game->setPlayerWon(-1);
	}
	else {

		game = (*this->allGamesData)[this->gameNumber];
	}
	gameNumber++;

	lock.unlock();
}

void GameManager::setNumberGame(int gameNumber) {

	this->gameNumber = gameNumber;
}

void GameManager::startGames() {

	for (int indexThread = 0; indexThread < this->gameNumber; indexThread++) {

		std::thread gameThread(runGameThread, (*this->allGamesData)[indexThread]);
		gameThread.join();
	}
}

GameManager::GameManager(std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData) {
	this->allGamesData = allGamesData;
	this->gameNumber = (*allGamesData).size();
}