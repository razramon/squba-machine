#include <windows.h>
#include "Ship.h"
#include "Sizes.h"

//returns true if the path exists
bool doesPathExist(const char* path)
{
	DWORD fileAttr = GetFileAttributesA(path);
	if (fileAttr == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	} else
	{
		return true;
	}
}

//checks if the path exists and  that the 3 files are there
bool isValidPath(const char* path, size_t pathLen)
{

}

int main(int argc, char* argv[])
{


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
