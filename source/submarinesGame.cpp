#include "Utilities.h"
#include <stdlib.h>
#include "Game.h"
#include "BoardCreator.h"

using namespace std;

int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	bool delay = false;
	int delayMS = DEFAULT_DELAY;
	bool quiet = false;
	std::vector<std::string>* filesFound = Utilities::buildPath(argc, argv, delay, delayMS, quiet);
	if ((*filesFound).size() != 3)
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
		Game* firstGame = new Game(board, *filesFound, delay, delayMS, quiet);
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
