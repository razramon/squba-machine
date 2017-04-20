#include "Game.h"

Game::Game(std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips, std::string* fullPathToAttackFileA, std::string* fullPathToAttackFileB)
{
	this->playersShips = playersShips;
	this->points.first = 0;
	this->points.second = 0;
	this->hasAttacks.first = true;
	this->hasAttacks.second = true;
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

int Game::isHit(int row, int col,char& letter) const
{
	Ship* s = nullptr;
	int** posArray;
	// Looping over all ships
	for (int j = 0; j < 2; ++j)
	{
		std::vector<Ship*>* ps = (j == 0) ? (*playersShips).first : (*playersShips).second;
		for (int i = 0; i < (*ps).size(); ++i)
		{
			s = (*ps).at(i);
			posArray = ((*s).getPosition());
			for (int pos = 0; pos < (*s).getShipSize(); ++pos)
			{
				if (row==(posArray)[pos][0] && col==(posArray)[pos][1])
				{
					letter = (*s).getLetter();
					switch (posArray[pos][2]){
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
	letter = 'a';
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

	if (playerPlaying == PLAYER_A)
	{
		attack = playerA->getAttack();
	}
	else //player B's turn..
	{
		attack = playerB->getAttack();
	}

	if (attack == nullptr)
	{
		return std::pair<int, int>(-1, -1);
	}
	return *attack;

}

void Game::game()
{
	int damaged = 0;;
	int win = 0;
	char letter = 'a';
	while ((hasAttacks.first || hasAttacks.second) && win == 0)
	{
		std::pair<int, int> curAttack = attack();
		AttackResult result = AttackResult::Miss;

		if(curAttack.first == -1 || curAttack.second == -1)
		{
			hasAttacks.first = curAttack.first == -1 ? false : true;
			hasAttacks.second = curAttack.second == -1 ? false : true;
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
			continue;
		}

		damaged = isHit(curAttack.first-1, curAttack.second-1, letter);

		// Destroy my own ship
		if(damaged == 4)
		{
			result = AttackResult::Sink;
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;

			if(playerPlaying == PLAYER_A)
			{
				points.second += Ship::pointsOfShip(letter);
				shipSunk.first++;
			}
			else
			{
				points.first += Ship::pointsOfShip(letter);
				shipSunk.second++;
			}
			win = checkWin();
			std::cout << std::to_string(curAttack.first) + "," + std::to_string(curAttack.second);
			std::cout << " suicide" << std::endl;

		}
		// Hit before \ hit myself
		else if (damaged == 3)
		{
			// Send hit, change playerPlaying
			result = AttackResult::Hit;
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
			std::cout << std::to_string(curAttack.first) + "," + std::to_string(curAttack.second);
			std::cout << " is self harm" << std::endl;
		}
		// Sink
		else if (damaged == 2)
		{
			result = AttackResult::Sink;

			if (playerPlaying == PLAYER_A)
			{
				points.first += Ship::pointsOfShip(letter);
				shipSunk.second++;
			}
			else
			{
				points.second += Ship::pointsOfShip(letter);
				shipSunk.first++;
			}

			std::cout << std::to_string(curAttack.first) + "," + std::to_string(curAttack.second);
			std::cout << " is dead" << std::endl;
			win = checkWin();
		}
		// Hit 
		else if (damaged == 1)
		{
			result = AttackResult::Hit;

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
		notifyOnAttackResult(PLAYER_A, curAttack.first, curAttack.second, result);
		notifyOnAttackResult(PLAYER_B, curAttack.first, curAttack.second, result);
	}

	if(win)
	{
		std::cout << "Player " << Player::getLetterByNumber(win);
		std::cout << " Won" << std::endl;
	}
	std::cout << "Points:" << std::endl;
	std::cout << "Player A: ";
	std::cout << points.first << std::endl;
	std::cout << "Player B: ";
	std::cout << points.second << std::endl;

}

int Game::checkWin()
{
	int whoWon = 0;
	whoWon = shipSunk.first == 5 ? 1 : 0;
	return whoWon = whoWon == 1 ? 1 : (shipSunk.second == 5 ? 2 : 0);
}