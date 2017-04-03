#include <iostream>
#include "Ship.h"
#include "Sizes.h"

using namespace std;

class Player {
	Ship ships[NUMBER_SHIPS];
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
					bool letterInPosition = false;
					int indexInShip = 0;

					// Looping on the ships and their positions
					while(ships[indexShip].position[indexInShip] != NULL)
					{
						// Checking if there is a match in ship position
						if(indexRow == ships[indexShip].position[indexInShip][0] &&
							indexColumn == ships[indexShip].position[indexInShip][1])
						{
							cout << ships[indexShip].getLetter();
							letterInPosition = true;
							break;
						}

						indexInShip++;
					}
					
					if(letterInPosition)
						continue;
					else
						cout << ' ';
				}
			}
			cout << endl;
		}
	}

};