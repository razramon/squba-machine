#include "Game.h"


bool Game::initPlayers(std::string & allErrors, int playerNum)
{
	IBattleshipGameAlgo* currentPlayer = (playerNum==PLAYER_A? playerA : playerB);

	return false;
}

Game::Game(char ** board, std::vector<std::string>& filesFound):
	playersShips(BoardCreator::checkBoard(board, BOARD_LENGTH, BOARD_LENGTH)), playerPlaying(PLAYER_A)
{
	/**TODO:: don't forget to insert these to initialize list!**/
	//IBattleshipGameAlgo* playerA;
	//IBattleshipGameAlgo* playerB;
	//std::pair<int, int> points;
	//std::pair<int, int> shipSunk;

	if (playersShips == nullptr)
	{
		throw Exception("PRINT_NOTHING"); //Appropriate errors have already been printed in "checkBoard"
	}
	
	try
	{
		std::string allErrors;
		//// Creating new players for the game
		//playerA = new Player(PLAYER_A, fullPathToAttackFileA, (*playersShips).first);
		//playerB = new Player(PLAYER_B, fullPathToAttackFileB, (*playersShips).second);
		for (int i = 0; i < Utilities::NUMBER_DLLS; ++i)
		{
			// Load dynamic library
			HINSTANCE hDll = LoadLibraryA(filesFound.at(i).c_str()); // Notice: Unicode compatible version of LoadLibrary
			if (!hDll)
			{
				allErrors.append("Cannot load dll: ");
				allErrors.append(filesFound.at(i)+"\n");
				initPlayers(allErrors);
			}
		}

		


	} catch(std::exception& e)
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
		throw e;
	}
	
}

Game::~Game()
{
	//delete playerA;
	//delete playerB;
	//if (playersShips != nullptr)
	//{
	//	//deletes ships to free allocated space
	//	for (int i = 0; i < (*(*playersShips).first).size(); ++i)
	//	{
	//		delete ((*((*playersShips).first)).at(i));
	//	}
	//	delete (*playersShips).first;
	//	for (int i = 0; i < (*(*playersShips).second).size(); ++i)
	//	{
	//		delete ((*((*playersShips).second)).at(i));
	//	}
	//	delete (*playersShips).second;
	//	delete playersShips;
	//}
	//Frees memory of allocated board
	//for (int i = 0; i < BOARD_LENGTH; ++i)
	//{
	//	delete[] board[i];
	//}
	//delete[] board;
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
			if (s->isSunk())
				continue;
			posArray = ((*s).getPosition());
			for (int pos = 0; pos < (*s).getShipSize(); ++pos)
			{
				if (row==(posArray)[pos][0] && col==(posArray)[pos][1])
				{
					letter = (*s).getLetter();
					switch (posArray[pos][2]){
					case 0:
						(*s).setPosition(pos, row, col, HIT);
						if((playerPlaying==PLAYER_A && (*s).shipOfPlayer()==PLAYER_A) || (playerPlaying == PLAYER_B && (*s).shipOfPlayer() != PLAYER_A))
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

void Game::game()
{
	//int damaged = 0;;
	//int win = -1;
	//char letter = 'a';
	//while (((*playerA).hasAttack() || (*playerB).hasAttack()) && win == -1)
	//{
	//	letter = 'a';
	//	std::pair<int, int> curAttack;
	//	
	//	if (playerPlaying == PLAYER_A)
	//	{
	//		curAttack = playerA->attack();
	//	}
	//	else //player B's turn..
	//	{
	//		curAttack = playerB->attack();
	//	}
	//	 
	//	AttackResult result = AttackResult::Miss;

	//	if(curAttack.first == -1 || curAttack.second == -1)
	//	{
	//		playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
	//		continue;
	//	}

	//	damaged = isHit(curAttack.first-1, curAttack.second-1, letter);

	//	// Destroy my own ship
	//	if(damaged == 4)
	//	{
	//		result = AttackResult::Sink;


	//		if(playerPlaying == PLAYER_A)
	//		{
	//			points.second += Ship::pointsOfShip(letter);
	//			shipSunk.first += 1;
	//		}
	//		else
	//		{
	//			points.first += Ship::pointsOfShip(letter);
	//			shipSunk.second += 1;
	//		}
	//		playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;

	//	}
	//	// Hit before \ hit myself
	//	else if (damaged == 3)
	//	{
	//		// Send hit, change playerPlaying
	//		result = AttackResult::Hit;
	//		playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
	//	}
	//	// Sink
	//	else if (damaged == 2)
	//	{
	//		result = AttackResult::Sink;

	//		if (playerPlaying == PLAYER_A)
	//		{
	//			points.first += Ship::pointsOfShip(letter);
	//			shipSunk.second +=1;
	//		}
	//		else
	//		{
	//			points.second += Ship::pointsOfShip(letter);
	//			shipSunk.first +=1;
	//		}
	//	}
	//	// Hit 
	//	else if (damaged == 1)
	//	{
	//		result = AttackResult::Hit;
	//	}
	//	// Miss
	//	else
	//	{
	//		// Change playerPlaying
	//		playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
	//	}
	//	win = checkWin();
	//	// Notify players on the result
	//	//notifyOnAttackResult(PLAYER_A, curAttack.first, curAttack.second, result);
	//	//notifyOnAttackResult(PLAYER_B, curAttack.first, curAttack.second, result);
	//}

	//if(win != -1)
	//{
	//	std::cout << "Player " << Player::getLetterByNumber(win);
	//	std::cout << " won" << std::endl;
	//}

	//std::cout << "Points:" << std::endl;
	//std::cout << "Player A: ";
	//std::cout << points.first << std::endl;
	//std::cout << "Player B: ";
	//std::cout << points.second << std::endl;

}

int Game::checkWin() const
{
	int count = 0;
	std::vector<Ship*>* ps = (*playersShips).first;
	for(int i =0 ; i < NUMBER_SHIPS; i++)
	{
		count += ps->at(i)->isSunk() ? 1 : 0;
	}
	if (count == 5)
		return 1;
	else {
		ps = (*playersShips).second;
		count = 0;
		for (int i = 0; i < NUMBER_SHIPS; i++)
		{
			count += ps->at(i)->isSunk() ? 1 : 0;
		}
		if (count == 5)
			return 0;
	}
	return -1;

}