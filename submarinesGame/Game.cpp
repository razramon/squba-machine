#include "Game.h"

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

int Game::isHit(int row, int col) const
{
	Ship* s = nullptr;
	// Looping over all ships
	for (int j = 0; j < 2; j++)
	{
		std::vector<Ship*>* ps = (j == 0) ? (*playersShips).first : (*playersShips).second;
		for (int i = 0; i < (*ps).size(); ++i)
		{
			s = (*ps).at(i);
			for (int pos = 0; pos < (*s).getShipSize(); ++pos)
			{
				if (row==(*s).getPosition()[pos][0] && col== (*s).getPosition()[pos][1])
				{
					switch ((*s).getPosition()[pos][2]){
					case 0:
						(*s).setPosition(pos, row, col, HIT);
						if((playerPlaying==PLAYER_A && (*s).shipOfPlayer()==PLAYER_A) || (playerPlaying == PLAYER_B && (*s).shipOfPlayer() == PLAYER_B))
						{
							return (s->numOfHits() == s->getShipSize())? SELF_DESTRUCT:BAD_HIT;
						} else
						{
							return (s->numOfHits() == s->getShipSize()) ? SUNK : HIT;
						}
					case 1:
						return BAD_HIT;
					default:
						break;
					}
				}
			}
		}

	}
	return MISS;
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
	while (hasAttacks.first || hasAttacks.second)
	{
		std::pair<int, int> curAttack = attack();
		AttackResult result = AttackResult::Miss;

		if(curAttack.first == -1 || curAttack.second == -1)
		{
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
			continue;
		}

		isHitA = isHit(curAttack.first, curAttack.second);
		isHitB = isHit(curAttack.first, curAttack.second);

		// Hit before
		if (isHitA == 3 || isHitB == 3)
		{
			// Send hit, change playerPlaying
			result = AttackResult::Hit;
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
		}
		// Sink
		else if (isHitA == 2 || isHitB == 2)
		{
			result = AttackResult::Sink;
			// Self sink and change playerPlaying to the other
			if ((isHitA == 2 && playerPlaying == PLAYER_A) || (isHitB == 2 && playerPlaying == PLAYER_B))
			{
				playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
			}
			std::cout << std::to_string(curAttack.first) + "," + std::to_string(curAttack.second);
			std::cout << " is dead" << std::endl;

		}
		// Hit 
		else if (isHitA == 1 || isHitB == 1)
		{
			result = AttackResult::Hit;
			// Self hit and change playerPlaying to the other
			if ((isHitA == 1 && playerPlaying == PLAYER_A) || (isHitB == 1 && playerPlaying == PLAYER_B))
			{
				playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
			}
			std::cout << std::to_string(curAttack.first) + "," + std::to_string(curAttack.second);
			std::cout << " is hit" << std::endl;
		}
		// Miss
		else
		{
			// Change playerPlaying
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
		}

		// Notify players on the result
		notifyOnAttackResult(playerPlaying, curAttack.first, curAttack.second, result);
		notifyOnAttackResult(playerPlaying, curAttack.first, curAttack.second, result);
	}
}


