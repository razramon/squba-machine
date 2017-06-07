#include "submarinesGame.h"


using namespace std;


void runGameThread(GameInfo* gameInfo) {

	while (gameInfo->getPlayerWon != -1) {

		std::pair<std::shared_ptr<IBattleshipGameAlgo>, std::shared_ptr<IBattleshipGameAlgo>> algos = gameInfo->getPlayersAlgos();
		unique_ptr<Game> newGame = std::make_unique<Game>(algos.first,algos.second, gameInfo->getBoard());

		getGame(gameInfo);
	}

}

void getGame(GameInfo* game) {

	mutex lock;
	lock.lock();

	if (gameNumber > (*allGamesData).size()) {

		game->setPlayerWon(-1);
	}
	else {

		game = allGamesData[gameNumber];
	}
	gameNumber++;

	lock.unlock();
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	int threadsNum = THREADS_DEFAULT_NUMBER;

	shared_ptr<std::vector<std::string>> filesFound = make_shared<vector<string>>(Utilities::buildPath(argc, argv, threadsNum));
	shared_ptr<std::vector<std::string>> boardFiles;
	shared_ptr<std::vector<std::string>> DLLFiles;

	Utilities::divideToDLLAndBoard(filesFound, boardFiles, DLLFiles);

	if ((*filesFound).size() <= 2 || (*boardFiles).size() < 0 || (*DLLFiles).size() < 2)
	{
		return 1;
	}


	// TODO: init all the boards and dlls, add each of them to the vector of gameinfo.
	// each thread will run forever/ until we tell them to stop, getting a new game each time.
	BoardCreator::checkBoards(boardFiles);

	Utilities::loadAllDlls(DLLFiles); // In this function we will also count and tag them



	//GameInfo::divideToGames(vector of dlls, vector of boards, allGamesData);

	gameNumber = THREADS_DEFAULT_NUMBER;

	for (int indexThread = 0; indexThread < gameNumber; indexThread++) {

		std::thread gameThread(runGameThread, (*allGamesData)[indexThread]);
		gameThread.join();
	}

	try
	{
		////Game* firstGame = new Game(board, *filesFound);
		//firstGame->game();
		//delete firstGame;
	}
	catch (std::exception& e)
	{
		if (strcmp(e.what(), "PRINT_NOTHING") != 0)
		{
			std::cout << e.what() << std::endl;
		}
	}

	delete filesFound;
	delete DLLFiles;
	delete boardFiles;
	return 0;
}

