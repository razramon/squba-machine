#include "Game.h"


/*
 * Deletes all memory allocations of playerShips
 */
void Game::deletePlayerShips() const
{
	if (playersShips != nullptr){
		if (((*playersShips).first) != nullptr)
		{
			for (int i = 0; i < (*(*playersShips).first).size(); ++i)
			{
				delete ((*((*playersShips).first)).at(i));
			}
			delete (*playersShips).first;
		}
		if (((*playersShips).second) != nullptr)
		{
			for (int i = 0; i < (*(*playersShips).second).size(); ++i)
			{
				delete ((*((*playersShips).second)).at(i));
			}
			delete (*playersShips).second;
		}
	}
	delete playersShips;
}

void Game::freeDlls() const
{
	for (std::vector<HINSTANCE, HINSTANCE>::iterator dllIter = dlls.begin(); dllIter != dlls.end(); ++dllIter)
	{
		FreeLibrary(*dllIter);
	}
}

char Game::getLetterByNumber(int number)
{
	return (number == 0 ? 'A' : 'B');
}

bool Game::noAttacksLeft(std::pair<int, int>& attackOfPlayer)
{
	return (attackOfPlayer.first == -1 && attackOfPlayer.second == -1);
}
/*
 * Returns true if attack is of format: ([1-10],[1-10]) or (-1,-1)
 */
bool Game::isValAttack(std::pair<int, int>& attackOfPlayer)
{
	return ((1 <= attackOfPlayer.first && attackOfPlayer.first <=10)&&
		(1 <= attackOfPlayer.second && attackOfPlayer.second <= 10)) ||
		noAttacksLeft(attackOfPlayer);
}


/*
* Returns TRUE if current playerPlaying has an attack, if he does - updates attackOfPlayer
* If he doesn't - updates playerPlaying to other player and if it(other) has an attack updates attackOfPlayer
* If BOTH players doesn't have an attack - returns FALSE.
* According to: http://moodle.tau.ac.il/mod/forum/discuss.php?d=52667
* If a player gave an attack that is not legit, Game should ignore it!
*/
bool Game::playersHaveAttack(std::pair<int, int>& attackOfPlayer)
{
	do {
		attackOfPlayer = playerPlaying == PLAYER_A ? playerA->attack() : playerB->attack();
	} while (!(isValAttack(attackOfPlayer)));

	if (noAttacksLeft(attackOfPlayer))
	{
		//if current player didn't have an attack, updates player to the other player and checks if he has attack
		playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
		do {
			attackOfPlayer = playerPlaying == PLAYER_A ? playerA->attack() : playerB->attack();
		} while (!(isValAttack(attackOfPlayer)));
		if (noAttacksLeft(attackOfPlayer))
		{
			return false;
		} 
	}
	return true;
}

/*
 * Notifies both players on last result of the attack,
 * Function should be called before updating "playerPlaying"! 
 */
void Game::notifyPlayers(std::pair<int, int>& currAttack, AttackResult & result) const
{
	if (currAttack.first == -1) return;
	(*playerA).notifyOnAttackResult(playerPlaying, currAttack.first, currAttack.second, result);
	(*playerB).notifyOnAttackResult(playerPlaying, currAttack.first, currAttack.second, result);
}

Game::Game(char ** board, std::vector<std::string>& filesFound): //large init list, don't panic! :)
	playersShips(BoardCreator::checkBoard(board, BOARD_LENGTH, BOARD_LENGTH)), playerPlaying(PLAYER_A),
	playerA(nullptr), playerB(nullptr), points(std::make_pair(0,0)),
	shipSunk(std::make_pair(0,0)), dlls(nullptr)
{
	if (playersShips == nullptr)
	{
		throw Exception("PRINT_NOTHING"); //Appropriate errors have already been printed in "checkBoard"
	}
	std::pair<char **,char**>boards = BoardCreator::getInitBoardForEachPlayer(playersShips);
	try
	{
		for (int i = 0; i < Utilities::NUMBER_DLLS; ++i)
		{
			// Load dynamic library
			HINSTANCE hDll = LoadLibraryA(filesFound.at(i).c_str());
			if (!hDll)
			{
				throw Exception(exceptionInfo(CANNOT_LOAD_DLL, filesFound.at(i)));
			} 
			dlls.push_back(hDll);
			GetAlgoFuncType getAlgoFunc = (GetAlgoFuncType)GetProcAddress(hDll, "GetAlgorithm");
			if (!getAlgoFunc)
			{
				throw Exception(exceptionInfo(CANNOT_LOAD_DLL, filesFound.at(i)));
			}

			IBattleshipGameAlgo* currPlayer = (i == PLAYER_A ? playerA : playerB);
			currPlayer = getAlgoFunc();
			char** currBoard = (i == PLAYER_A ? boards.first : boards.second);
			(*currPlayer).setBoard(i, const_cast<const char**>(currBoard), BOARD_LENGTH, BOARD_LENGTH);
			if (!((*currPlayer).init(filesFound.at(i))))
			{
				throw Exception(exceptionInfo(ALGO_INIT_FAILED, filesFound.at(i)));
			}
		}
		BoardCreator::freeBoard(boards.first, BOARD_LENGTH);
		BoardCreator::freeBoard(boards.second, BOARD_LENGTH);
	} catch(std::exception& e)
	{
		//free dlls, boards and ships
		freeDlls();
		BoardCreator::freeBoard(boards.first, BOARD_LENGTH);
		BoardCreator::freeBoard(boards.second, BOARD_LENGTH);
		deletePlayerShips();
		throw e;
	}
}

Game::~Game()
{
	deletePlayerShips();
	delete playerA;
	delete playerB;
	freeDlls();
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
	int damaged = 0;;
	int win = -1;
	char letter = 'a';
	std::pair<int, int> curAttack(-1, -1);
	while (win==-1 && playersHaveAttack(curAttack))
	{
		letter = 'a';
		AttackResult result = AttackResult::Miss;
		damaged = isHit(curAttack.first - 1, curAttack.second - 1, letter);
		switch (damaged)
		{
		case SELF_DESTRUCT: // Player destroyed his own ship
			result = AttackResult::Sink;
			if (playerPlaying == PLAYER_A)
			{
				points.second += Ship::pointsOfShip(letter);
				shipSunk.first += 1;
			}
			else
			{
				points.first += Ship::pointsOfShip(letter);
				shipSunk.second += 1;
			}
			notifyPlayers(curAttack, result);
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
			break;
		case BAD_HIT: // Hit before \ hit myself
			result = AttackResult::Hit; 
			notifyPlayers(curAttack, result); // Send hit, change playerPlaying
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
			break;
		case SUNK: // Sink
			result = AttackResult::Sink;
			if (playerPlaying == PLAYER_A)
			{
				points.first += Ship::pointsOfShip(letter);
				shipSunk.second += 1;
			}
			else
			{
				points.second += Ship::pointsOfShip(letter);
				shipSunk.first += 1;
			}
			notifyPlayers(curAttack, result);
			break;
		case HIT:
			result = AttackResult::Hit;
			notifyPlayers(curAttack, result);
			break;
		case MISS:
			notifyPlayers(curAttack, result);
			playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A; // Change playerPlaying
			break;
		default://never supposed to get here anyway
			throw Exception("Error: got error result");
		}
		win = checkWin();
	}

	if(win != -1)
	{
		std::cout << "Player " << getLetterByNumber(win);
		std::cout << " won" << std::endl;
	}

	std::cout << "Points:" << std::endl;
	std::cout << "Player A: ";
	std::cout << points.first << std::endl;
	std::cout << "Player B: ";
	std::cout << points.second << std::endl;

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