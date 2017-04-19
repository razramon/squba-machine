#include "Game.h"
#include "Ship.h"
#include "Sizes.h"

Game::Game(std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips, std::string* fullPathToAttackFileA, std::string* fullPathToAttackFileB)
{
	// Creating new players for the test of the game
	Player* playerA = new Player(PLAYER_A, *fullPathToAttackFileA, (*playersShips).first);
	Player* playerB = new Player(PLAYER_B, *fullPathToAttackFileB, (*playersShips).second);

}

Game::~Game()
{
}

void Game::setBoard(const char** board, int numRows, int numCols)
{
	this->board = new char*[numRows];
	for (int indexRow = 0; indexRow < numRows; indexRow++)
	{
		this->board[indexRow] = new char[numCols];

		for (int indexCol = 0; indexCol < numCols; indexCol++)
		{
			this->board[indexRow][indexCol] = board[indexRow][indexCol];
		}
	}
}

int Game::isHit(int row, int col)
{
	Ship ships[5];
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
			if (ships[indexShip].getPosition()[pos][0] == row && ships[indexShip].getPosition()[pos][1] == col)
			{
				hitBefore = ships[indexShip].getPosition()[pos][2];
				hit = true;
				indexHitShip = indexShip;
				letterHit = ships[indexShip].getLetter();
			}

			numberOfHitInShip += ships[indexShip].getPosition()[pos][2];
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
void Game::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
}
//	switch (result)
//	{
//	case AttackResult::Miss:
//		break;
//	case AttackResult::Hit:
//		break;
//	case AttackResult::Sink:
//		break;
//	default:
//		break;
//
//	}
//	char letterSink = ' ';
//	// Looping over all the ships of the player
//	for (int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
//	{
//		// Looping over the position of each ship
//		for (int pos = 0; pos < Ship::sizeOfShip(ships[indexShip].getLetter()); pos++)
//		{
//			// If there is a match
//			if (ships[indexShip].getPosition()[pos][0] == row && ships[indexShip].getPosition()[pos][1] == col)
//			{
//				// Changing to hit
//				ships[indexShip].setPosition(pos, row, col, 1);
//				if (result == AttackResult::Sink && this->isHit(row, col) != 3)
//					totalNumberOfPoints += ships[indexShip].getNumberOfPoints();
//			}
//		}
//	}
//}
std::pair<int, int> Game::attack()
{
	//std::pair<int, int> attack = attacks[++attackNumber];

	//// Checking if the number of the attack equal the size of the vector, if so changing to -1 to say there aren't more moves
	//if (attackNumber == attacks.size() - 1)
	//{
	//	attackNumber = -1;
	//}
	//return attack;
}

//void game(Player* playerA, Player* playerB)
//{
//	int isHitA = 0;
//	int isHitB = 0;
//	int playerPlaying = 0;
//	//TODO: add points checker for the players
//	while ((*playerA).attackNumber != -1 || (*playerB).attackNumber != -1)
//	{
//		pair<int, int> attack;
//		AttackResult result = AttackResult::Miss;
//		// Inside player A and there are attacks left
//		if (playerPlaying == (*playerA).playerNum && (*playerA).attackNumber != -1)
//			attack = (*playerA).attack();
//		// Inside player B and there are attacks left
//		else if (playerPlaying == (*playerB).playerNum && (*playerB).attackNumber != -1)
//			attack = (*playerB).attack();
//
//		isHitA = (*playerA).isHit(attack.first, attack.second);
//		isHitB = (*playerB).isHit(attack.first, attack.second);
//
//		// Hit before
//		if (isHitA == 3 || isHitB == 3)
//		{
//			// Send hit, change playerPlaying
//			result = AttackResult::Hit;
//			playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//
//		}
//		// Sink
//		else if (isHitA == 2 || isHitB == 2)
//		{
//			result = AttackResult::Sink;
//			// Self sink and change playerPlaying to the other
//			if ((isHitA == 2 && playerPlaying == (*playerA).playerNum) || (isHitB == 2 && playerPlaying == (*playerB).playerNum))
//			{
//				playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//			}
//			std::cout << std::to_string(attack.first) + "," + std::to_string(attack.second);
//			std::cout << " is dead" << std::endl;
//
//		}
//		// Hit 
//		else if (isHitA == 1 || isHitB == 1)
//		{
//			result = AttackResult::Hit;
//			// Self hit and change playerPlaying to the other
//			if ((isHitA == 1 && playerPlaying == (*playerA).playerNum) || (isHitB == 1 && playerPlaying == (*playerB).playerNum))
//			{
//				playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//			}
//			std::cout << std::to_string(attack.first) + "," + std::to_string(attack.second);
//			std::cout << " is hit" << std::endl;
//		}
//		// Miss
//		else
//		{
//			// Change playerPlaying
//			playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//		}
//
//		// Notify players on the result
//		(*playerA).notifyOnAttackResult(playerPlaying, attack.first, attack.second, result);
//		(*playerB).notifyOnAttackResult(playerPlaying, attack.first, attack.second, result);
//	}
//}