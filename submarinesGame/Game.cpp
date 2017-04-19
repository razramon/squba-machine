#include "Game.h"
#include "Ship.h"
#include "Sizes.h"
#include <iostream>

Game::Game(std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips, std::string* fullPathToAttackFileA, std::string* fullPathToAttackFileB)
{
	this->playersShips = playersShips;
	// Creating new players for the test of the game
	try
	{
		this->playerA = new Player(PLAYER_A, *fullPathToAttackFileA, (*playersShips).first);
		this->playerB = new Player(PLAYER_B, *fullPathToAttackFileB, (*playersShips).second);
	}
	catch (std::exception&e)
	{
		//deletes ships to free allocated space
		for (int i = 0; i < (*(*playersShips).first).size(); ++i)
		{
			delete ((*((*playersShips).first)).at(i));
		}
		delete (*playersShips).first;
		for (int i = 0; i < (*(*playersShips).second).size(); ++i)
		{
			delete ((*((*playersShips).second)).at(i));
		}
		delete (*playersShips).second;
		delete playersShips;
		throw &e;

	}
	playerPlaying = PLAYER_A;
}

Game::~Game()
{
	delete playerA;
	delete playerB;
	if (playersShips != nullptr)
	{
		//deletes ships to free allocated space
		for (int i = 0; i < (*(*playersShips).first).size(); ++i)
		{
			delete ((*((*playersShips).first)).at(i));
		}
		delete (*playersShips).first;
		for (int i = 0; i < (*(*playersShips).second).size(); ++i)
		{
			delete ((*((*playersShips).second)).at(i));
		}
		delete (*playersShips).second;
		delete playersShips;
	}
	//Frees memory of allocated board
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		delete[] board[i];
	}
	delete[] board;
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
	std::pair<int, int>* attack = nullptr;

	if (playerPlaying==PLAYER_A)
	{
		attack = playerA->getAttack();
	} else //player B's turn..
	{
		attack = playerB->getAttack();
	}

	if (attack == nullptr)
	{
		return std::pair<int, int>(-1, -1);
	}
	return *attack;

}

void Game::game(Player* playerA, Player* playerB)
{
	int isHitA = 0;
	int isHitB = 0;
	//TODO: add points checker for the players
	while ((*playerA).getAttackNumber() != -1 || (*playerB).getAttackNumber() != -1)
	{
		std::pair<int, int> curAttack;
		AttackResult result = AttackResult::Miss;
		// Inside player A and there are attacks left
		if (playerPlaying == (*playerA).getPlayerNumber() && (*playerA).getAttackNumber() != -1)
			curAttack = attack();
		// Inside player B and there are attacks left
		else if (playerPlaying == (*playerB).getPlayerNumber() && (*playerB).getAttackNumber() != -1)
			curAttack = attack();

		isHitA = isHit(curAttack.first, curAttack.second);
		isHitB = isHit(curAttack.first, curAttack.second);

		// Hit before
		if (isHitA == 3 || isHitB == 3)
		{
			// Send hit, change playerPlaying
			result = AttackResult::Hit;
			playerPlaying = playerPlaying == (*playerB).getPlayerNumber() ? (*playerA).getPlayerNumber() : (*playerB).getPlayerNumber();

		}
		// Sink
		else if (isHitA == 2 || isHitB == 2)
		{
			result = AttackResult::Sink;
			// Self sink and change playerPlaying to the other
			if ((isHitA == 2 && playerPlaying == (*playerA).getPlayerNumber()) || (isHitB == 2 && playerPlaying == (*playerB).getPlayerNumber()))
			{
				playerPlaying = playerPlaying == (*playerB).getPlayerNumber() ? (*playerA).getPlayerNumber() : (*playerB).getPlayerNumber();
			}
			std::cout << std::to_string(curAttack.first) + "," + std::to_string(curAttack.second);
			std::cout << " is dead" << std::endl;

		}
		// Hit 
		else if (isHitA == 1 || isHitB == 1)
		{
			result = AttackResult::Hit;
			// Self hit and change playerPlaying to the other
			if ((isHitA == 1 && playerPlaying == (*playerA).getPlayerNumber()) || (isHitB == 1 && playerPlaying == (*playerB).getPlayerNumber()))
			{
				playerPlaying = playerPlaying == (*playerB).getPlayerNumber() ? (*playerA).getPlayerNumber() : (*playerB).getPlayerNumber();
			}
			std::cout << std::to_string(curAttack.first) + "," + std::to_string(curAttack.second);
			std::cout << " is hit" << std::endl;
		}
		// Miss
		else
		{
			// Change playerPlaying
			playerPlaying = playerPlaying == (*playerB).getPlayerNumber() ? (*playerA).getPlayerNumber() : (*playerB).getPlayerNumber();
		}

		// Notify players on the result
		notifyOnAttackResult(playerPlaying, curAttack.first, curAttack.second, result);
		notifyOnAttackResult(playerPlaying, curAttack.first, curAttack.second, result);
	}
}