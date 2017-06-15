#include "submarinesGame.h"

typedef std::vector<std::vector<std::vector<char>>> boardType;
using namespace std;


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	int threadsNum = THREADS_DEFAULT_NUMBER;

	shared_ptr<std::vector<std::string>> filesFound = make_shared<vector<string>>(Utilities::buildPath(argc, argv, threadsNum));
	shared_ptr<std::vector<std::string>> boardFiles;
	shared_ptr<std::vector<std::string>> DLLFiles;
	shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData;

	Utilities::divideToDLLAndBoard(filesFound, boardFiles, DLLFiles);

	if ((*filesFound).size() <= 2 || (*boardFiles).size() < 0 || (*DLLFiles).size() < 2)
	{
		return 1;
	}


	// TODO: init all the boards and dlls, add each of them to the vector of gameinfo.
	// each thread will run forever/ until we tell them to stop, getting a new game each time.
	std::shared_ptr<std::vector<std::shared_ptr<boardType>>> boards;

	/*BoardCreator::checkBoards(boardFiles, boards);*/


	std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls;
	ptrToVecOfPlayerInfoPtrs allPlayersInfo;

	GameManager::loadAllDlls(DLLFiles, dlls, allPlayersInfo); // In this function we will also count and tag them

	GameManager::divideToGames(dlls, boards, allGamesData);


	shared_ptr<GameManager> manager = make_shared<GameManager>(allGamesData, allPlayersInfo);

	// Saving thread for the logger
	manager->setNumberThreads(threadsNum - 1);

	manager->startGames();

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

	return 0;
}

