#include "Utilities.h"
#include <stdlib.h>
#include "Game.h"
#include "BoardCreator.h"
//#include <windows.h>
//#include "Ship.h"
//#include "Sizes.h"
//#include <iostream>
//#include "Exception.h"
//#include "Macros.h"
//#include <fstream>
//#include <set>
//#include <string>



using namespace std;

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	std::string path;
	if (argc == 1)
	{
		path = Utilities::workingDirectory();
	} else
	{
		path = argv[1];
	}

	char* boardFilePtr = nullptr;
	char* attackFileAPtr = nullptr;
	char* attackFileBPtr = nullptr;

	bool pathIsValid = false;

	try
	{
		pathIsValid = Utilities::isValidPath(path.c_str(), &boardFilePtr, &attackFileAPtr, &attackFileBPtr);
	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	if(!pathIsValid)
	{
		Utilities::printNotFoundFileErrors(path.c_str(), boardFilePtr, attackFileAPtr, attackFileBPtr);
		return 1;
	}

	std::string fullPathToBoard = path +"\\"+ boardFilePtr;
	char** board = nullptr;
	try
	{
		board = BoardCreator::getBoardFromFile(fullPathToBoard.c_str());
	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	std::string fullPathToAttackFileA = path + "\\" + attackFileAPtr;
	std::string fullPathToAttackFileB = path + "\\" + attackFileBPtr;

	try
	{
		//firstGame->setBoard(const_cast<const char**>(board), BOARD_LENGTH, BOARD_LENGTH);
		Game* firstGame = new Game(board, fullPathToAttackFileA, fullPathToAttackFileB);
		firstGame->game();
		delete firstGame;

	} catch (std::exception& e)
	{
		if (strcmp(e.what(), "PRINT_NOTHING") != 0)
		{
			std::cout << e.what() << std::endl;
		}
	}

	//Frees memory of allocated board
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		delete[] board[i];
	} 
	delete[] board;
	delete[] boardFilePtr;
	delete[] attackFileAPtr;
	delete[] attackFileBPtr;

	return 0;
}
