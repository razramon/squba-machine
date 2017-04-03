#include <iostream>
#include "Ship.h"
#include "Sizes.h"
using namespace std;

class Player {
	Ship ships[5];
	int playerNum;
	int totalNumberOfPoints;

public:
	void printBoard()
	{
		for(int indexRow = 0; indexRow < BOARD_LENGTH; indexRow++)
		{
			for(int indexColumn = 0; indexColumn < BOARD_LENGTH; indexColumn++)
			{
				for(int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
				{

				}


			}


		}

	}

};