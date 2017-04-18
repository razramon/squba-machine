#include "Player.h"
#include <iostream>
#include "Ship.h"
#include "Sizes.h"
#include <vector>

using namespace std;

Player::Player(int playerNum, char* pathToAttackFile, Ship* ships)
{
	//TODO::complete
}
	
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
int Player::isHit(int row, int col)
{
	int numberOfHitInShip = 0;
	char letterHit = ' ';
	int indexHitShip = 0;
	bool hit = false;
	int hitBefore = 0;

	// Looping over all the ships of the player
	for (int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
	{
		numberOfHitInShip = 0;
		// Looping over the position of each ship
		for (int pos = 0; pos < Ship::sizeOfShip(ships[indexShip].getLetter()); pos++)
		{
			// If there is a match, return true - there is a hit
			if (ships[indexShip].position[pos][0] == row && ships[indexShip].position[pos][1] == col)
			{
				hitBefore = ships[indexShip].position[pos][2];
				hit = true;
				indexHitShip = indexShip;
				letterHit = ships[indexShip].getLetter();
			}
				
			numberOfHitInShip += ships[indexShip].position[pos][2];
		}
		if (hit)
			break;
	}
		
	// If was hit
	if (hitBefore != 0)
		return 3;
	// If there is a hit and all the ship was hit - sink
	else if (hit && numberOfHitInShip == Ship::sizeOfShip(letterHit) - 1 && !ships[indexHitShip].isSunk())
		return 2;
	// Check if there was atleast a hit
	else if (hit)
		return 1;
	// Miss
	else 
		return 0;
}
void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	switch (result)
	{
		case AttackResult::Miss:
			break;
		case AttackResult::Hit:
			break;
		case AttackResult::Sink:
			break;
		default:
			break;
			
	}
	char letterSink = ' ';
	// Looping over all the ships of the player
	for (int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
	{
		// Looping over the position of each ship
		for (int pos = 0; pos < Ship::sizeOfShip(ships[indexShip].getLetter()); pos++)
		{
			// If there is a match
			if (ships[indexShip].position[pos][0] == row && ships[indexShip].position[pos][1] == col)
			{
				// Changing to hit
				ships[indexShip].position[pos][2] = 1;
				if(result == AttackResult::Sink && this->isHit(row,col) != 3)
					totalNumberOfPoints += ships[indexShip].getNumberOfPoints();
			}
		}
	}
}
std::pair<int, int> Player::attack()
{
	pair<int, int> attack = attacks[++attackNumber];

	// Checking if the number of the attack equal the size of the vector, if so changing to -1 to say there aren't more moves
	if(attackNumber == attacks.size() -1)
	{
		attackNumber = -1;
	}
	return attack;
}

