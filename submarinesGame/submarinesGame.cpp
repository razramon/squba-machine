#include <windows.h>
#include "Ship.h"
#include "Sizes.h"
#include <iostream>
#include <exception>

using namespace std;
CONST char DEFAULT_LETTER='a';
//returns true if the path exists && it's a DIRECTORY
//bool doesPathExist(const char* path)
//{
//	DWORD dirAttr = GetFileAttributesA(path);
//	if (dirAttr == INVALID_FILE_ATTRIBUTES)
//	{
//		throw GetLastError(); //change thisto create a new exception
//	}
//	return ((dirAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
//}
//
////checks if the path exists and  that the 3 files are there - NOT FINISHED YET!
//bool isValidPath(const char* path)
//{
//	bool doesExist;
//	try
//	{
//		doesExist = doesPathExist(path);
//		if (doesExist)
//		{
//			std::cout << "path is a directory!!" << std::endl;
//			return true;
//		}
//		else
//		{
//			std::cout << "path exists, but it's not a directory!!" << std::endl;
//			return false;
//		}
//
//	}
//	catch (std::exception& e) {
//		std::cout << "Exception accured: " << e.what() << std::endl;
//		return false;
//	}
//
//	char* attackFileA = NULL;
//	char* attackFileB = NULL;
//	char* boardFile = NULL;
//
//
//	while()
//	{
//		
//	}
//}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);//for memory leaks! :)
	//std::cout <<"and the path is: "<< isValidPath(argv[1]) << std::endl;
	return 0;
}
bool isShip(char c)
{
	c = tolower(c);
	return (c == 'm' || c == 'b' || c == 'd' || c == 'p') ? true : false;
}
int pointsOfShip(char c)
{
	c = tolower(c);
	switch (c)
	{
	case 'b': return B_POINTS;

	case 'p': return P_POINTS;

	case 'm': return M_POINTS;

	case 'd': return D_POINTS;

	default: return -1;
	}
}
int sizeOfShip(char c)
{
	c = tolower(c);
	switch (c)
	{
	case 'b': return B_SIZE;

	case 'p': return P_SIZE;

	case 'm': return M_SIZE;

	case 'd': return D_SIZE;

	default: return -1;
	}
}
void createShip(Ship * ship, char c)
{
	int shipPoints = 0;
	if ((shipPoints = pointsOfShip(c)) != -1)
	{
		int shipSize = 0;
		if ((shipSize = sizeOfShip(c)) != -1)
		{
			(*ship).setLetter(c);
			(*ship).setNumberOfPoints(shipPoints);
			(*ship).position = new int*[shipSize];
			for (int i = 0; i < shipSize; i++)
			{
				(*ship).position[i] = new int[3];
			}

		}
	}
}
void printBoard(Ship *ships)
{
	for (int indexRow = 0; indexRow < BOARD_LENGTH; indexRow++)
	{
		for (int indexColumn = 0; indexColumn < BOARD_LENGTH; indexColumn++)
		{
			for (int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
			{
				bool letterInPosition = false;
				int indexInShip = 0;

				// Looping on the ships and their positions
				while (ships[indexShip].position[indexInShip] != NULL)
				{
					// Checking if there is a match in ship position
					if (indexRow == ships[indexShip].position[indexInShip][0] &&
						indexColumn == ships[indexShip].position[indexInShip][1])
					{
						cout << ships[indexShip].getLetter();
						letterInPosition = true;
						break;
					}

					indexInShip++;
				}

				if (letterInPosition)
					continue;
				else
					cout << ' ';
			}
		}
		cout << endl;
	}
}

void setBoard(char ** board, int numRows, int numCols)
{
	Ship shipA[NUMBER_SHIPS];
	Ship shipB[NUMBER_SHIPS];
	int indexShipA = 0;
	int indexShipB = 0;
	int indexInShip = 0;
	for(int indexRow = 0; indexRow < numRows; indexRow++)
	{
		for(int indexColumn = 0; indexColumn < numCols; indexColumn++)
		{
			char letter = board[indexRow][indexColumn];
			if(isShip(letter))
			{
				if (islower(letter))
				{
					createShip((&shipB[indexShipB]), letter);
					shipB[indexShipB].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}
				else
				{
					createShip((&shipA[indexShipA]), letter);
					shipA[indexShipA].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}

				int indexInShip = 1;
				int indexShip = indexColumn++;
				
				// Searching column for the rest of the ship
				while (board[indexRow][indexShip] == letter)
				{
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexRow, indexShip, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}

				indexShip = indexRow++;
				// Searching row for the rest of the ship
				while (board[indexShip][indexColumn] == letter)
				{	
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}
				if (islower(letter))
					indexShipB++;
				else
					indexShipA++;
				
			}
		}
	}
	printBoard(shipA);
	printBoard(shipB);

}

bool checkBoard(char ** board)
{
	Ship shipA[NUMBER_SHIPS];
	Ship shipB[NUMBER_SHIPS];
	int indexShipA = 0;
	int indexShipB = 0;
	int indexInShip = 0;
	for (int indexRow = 0; indexRow < BOARD_LENGTH - 1; indexRow++)
	{
		for (int indexColumn = 0; indexColumn < BOARD_LENGTH - 1; indexColumn++)
		{
			char letter = board[indexRow][indexColumn];
			if (isShip(letter))
			{
				if (islower(letter))
				{
					createShip((&shipB[indexShipB]), letter);
					shipB[indexShipB].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}
				else
				{
					createShip((&shipA[indexShipA]), letter);
					shipA[indexShipA].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}

				int indexInShip = 1;
				int indexShip = indexColumn++;

				// Searching column for the rest of the ship
				while (board[indexRow][indexShip] == letter)
				{
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexRow, indexShip, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}

				indexShip = indexRow++;
				// Searching row for the rest of the ship
				while (board[indexShip][indexColumn] == letter)
				{
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}
				if (islower(letter))
					indexShipB++;
				else
					indexShipA++;

			}
		}
	}





}
