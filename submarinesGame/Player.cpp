#include <iostream>
#include "Ship.h"
#include "Sizes.h"
#include "ISubmarinesGameAlgo.h"
#include <vector>

using namespace std;


class Player {
	Ship ships[NUMBER_SHIPS];
	int playerNum;
	int totalNumberOfPoints;
	int rowIndexInFile;
	char* pathToAttackFile;
	vector<pair<int,int>> attacks;

public:
	
	void Player::setBoard(const char** board, int numRows, int numCols)
	{
		// Looping over all the ships of the player
		for (int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
		{
			// Looping over the position of each ship
			for (int pos = 0; pos < Ship::sizeOfShip(ships[indexShip].getLetter()); pos++)
			{
				int row = ships[indexShip].position[pos][0];
				int col = ships[indexShip].position[pos][1];
				ships[indexShip].position[pos][2] = board[row][col];
			}
		}
	}
	bool Player::isHit(int row, int col)
	{
		// Looping over all the ships of the player
		for (int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
		{
			// Looping over the position of each ship
			for (int pos = 0; pos < Ship::sizeOfShip(ships[indexShip].getLetter()); pos++)
			{
				// If there is a match, return true- there is a hit
				if (ships[indexShip].position[pos][0] == row && ships[indexShip].position[pos][1] == col)
				{
					return true;
				}
			}
		}

		return false;
	}
	void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result)
	{
		

	}
	std::pair<int, int> Player::attack()
	{
		std::pair <int, int> attack;



		attack.first = 0;
		attack.second = 1;

		return attack;
	}
};

