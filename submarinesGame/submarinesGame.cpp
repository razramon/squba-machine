#include "Utilities.h"
#include <stdlib.h>
#include "Game.h"
#include "BoardCreator.h"

using namespace std;

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	int threadsNum = DEFAULT_DELAY;
	std::vector<std::string>* filesFound = Utilities::buildPath(argc, argv, threadsNum);
	if ((*filesFound).size() != 2)
	{
		delete filesFound;
		return 1;
	}

	char** board = nullptr;
	try
	{
		board = BoardCreator::getBoardFromFile(((*filesFound).at(Utilities::INDEX_BOARD_PATH)).c_str());
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		delete filesFound;
		return 1;
	}

	try
	{
		Game* firstGame = new Game(board, *filesFound, threadsNum);
		firstGame->game();
		delete firstGame;
	}
	catch (std::exception& e)
	{
		if (strcmp(e.what(), "PRINT_NOTHING") != 0)
		{
			std::cout << e.what() << std::endl;
		}
	}
	delete filesFound;
	return 0;
}
