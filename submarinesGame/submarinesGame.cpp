#include <windows.h>
#include "Ship.h"
#include "Sizes.h"
#include <iostream>
#include "Exception.h"
#include "Macros.h"

//returns true if the path exists && it's a DIRECTORY
bool doesPathExist(const char* path)
{
	DWORD dirAttr = GetFileAttributesA(path);
	if (dirAttr == INVALID_FILE_ATTRIBUTES)
	{
		std::string s = exceptionInfo(WRONG_PATH, path);
		Exception* e = new Exception(s);
		throw &e; 
	}
	return ((dirAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

//checks if the path exists and  that the 3 files are there - NOT FINISHED YET!
bool isValidPath(const char* path)
{
	bool doesExist;
	try
	{
		doesExist = doesPathExist(path);
		if (doesExist)
		{
			std::cout << "path is a directory!!" << std::endl;
			return true;
		}
		else
		{
			std::cout << "path exists, but it's not a directory!!" << std::endl;
			return false;
		}

	}
	catch (std::exception& e) {
		std::cout << "Exception accured: " << e.what() << std::endl;
		return false;
	}

	//char* attackFileA = NULL;
	//char* attackFileB = NULL;
	//char* boardFile = NULL;


	//while()
	//{
	//	
	//}
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//for memory leaks! :)
	std::cout <<"and the path is: "<< isValidPath(argv[1]) << std::endl;
	return 0;
}

bool checkBoard(char ** board)
{
	Ship shipA[NUMBER_SHIPS];
	Ship shipB[NUMBER_SHIPS];
	int indexShipA = 0;
	int indexShipB = 0;
	int numberOfShipsA = 0;
	int numberOfShipsB = 0;

	for(int indexRow = 0; indexRow < BOARD_LENGTH - 1; indexRow++)
	{
		for(int indexColumn = 0; indexColumn < BOARD_LENGTH -1; indexColumn++)
		{
			if(islower(board[indexRow][indexColumn]) && isLetterOfShip(board[indexRow][indexColumn]))
			{


			}



		}

	}
	//islower
	return true;
}
bool isLetterOfShip(char c)
{
	c = tolower(c);
	if (c == 'b' || c == 'p' || c == 'm' || c == 'd')
		return true;
	return false;
}
