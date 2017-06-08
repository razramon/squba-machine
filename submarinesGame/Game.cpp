#include "Game.h"


//void Game::freeDlls()
//{
//	for (std::vector<HINSTANCE>::iterator dllIter = dlls.begin(); dllIter != dlls.end(); ++dllIter)
//	{
//		FreeLibrary(*dllIter);
//	}
//}

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
	return ((1 <= attackOfPlayer.first && attackOfPlayer.first <= BOARD_LENGTH) &&
			(1 <= attackOfPlayer.second && attackOfPlayer.second <= BOARD_LENGTH)) ||
		noAttacksLeft(attackOfPlayer);
}

Ship * Game::getShipAtPosition(int row, int col) const
{
	if (row < 0 || row >= BOARD_LENGTH || col < 0 || col >= BOARD_LENGTH) return nullptr;

	for (int i = 0; i < 2; ++i)
	{
		std::vector<Ship*>* ps = (i == 0) ? (*playersShips).first : (*playersShips).second;
		for (std::vector<Ship*>::iterator iter = (*ps).begin(); iter!=(*ps).end(); ++iter)
		{
			for (int j = 0; j < (**iter).getShipSize(); ++j)
			{
				if (row == (**iter).getPosition()[j][0] && col == (**iter).getPosition()[j][1])
				{
					return *iter;
				}
			}
		}
	}

	return nullptr;
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
	do
	{
		attackOfPlayer = playerPlaying == PLAYER_A ? playerA->attack() : playerB->attack();
	}
	while (!(isValAttack(attackOfPlayer)));

	if (noAttacksLeft(attackOfPlayer))
	{
		//if current player didn't have an attack, updates player to the other player and checks if he has attack
		playerPlaying = playerPlaying == PLAYER_A ? PLAYER_B : PLAYER_A;
		do
		{
			attackOfPlayer = playerPlaying == PLAYER_A ? playerA->attack() : playerB->attack();
		}
		while (!(isValAttack(attackOfPlayer)));
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
void Game::notifyPlayers(std::pair<int, int>& currAttack, AttackResult& result) const
{
	if (currAttack.first == -1) return;
	(*playerA).notifyOnAttackResult(playerPlaying, currAttack.first, currAttack.second, result);
	(*playerB).notifyOnAttackResult(playerPlaying, currAttack.first, currAttack.second, result);
}


Game::Game(std::shared_ptr<IBattleshipGameAlgo> playerA, std::shared_ptr<IBattleshipGameAlgo> playerB, std::shared_ptr<boardType> board): //large init list, don't panic! :)
	playersShips(BoardCreator::checkBoard(board, BOARD_LENGTH, BOARD_LENGTH)), playerPlaying(PLAYER_A),
	playerA(nullptr), playerB(nullptr), points(std::make_pair(0, 0)),
	shipSunk(std::make_pair(0, 0))
{
	if (playersShips == nullptr)
	{
		throw Exception("PRINT_NOTHING"); //Appropriate errors have already been printed in "checkBoard"
	}
	std::pair<char **, char**> boards = BoardCreator::getInitBoardForEachPlayer(playersShips);
	commonBoard = BoardCreator::createCommonBoard(playersShips->first, playersShips->second);
	
}

Game::~Game()
{
	deletePlayerShips();
	delete playerA;
	delete playerB;
	BoardCreator::freeBoard(commonBoard, BOARD_LENGTH);
}



int Game::isHit(int row, int col, char& letter) const
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
				if (row == (posArray)[pos][0] && col == (posArray)[pos][1])
				{
					letter = (*s).getLetter();
					switch (posArray[pos][2])
					{
					case 0:
						(*s).setPosition(pos, row, col, HIT);
						if ((playerPlaying == PLAYER_A && (*s).shipOfPlayer() == PLAYER_A) || (playerPlaying == PLAYER_B && (*s).shipOfPlayer() != PLAYER_A))
						{
							return (s->numOfHits() == s->getShipSize()) ? SELF_DESTRUCT : BAD_HIT;
						}
						else
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
	int row, col;
	while (win == -1 && playersHaveAttack(curAttack))
	{
		letter = 'a';
		AttackResult result = AttackResult::Miss;
		row = curAttack.first - 1;
		col = curAttack.second - 1;
		damaged = isHit(row, col, letter);
		GUIBoard::updateGUIBoard(commonBoard, damaged, row, col, getShipAtPosition(row, col), quiet, delayMS, playerPlaying);
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
		default: //never supposed to get here anyway
			throw Exception("Error: got error result");
		}
		win = checkWin();
	}

	if (win != -1)
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
	for (int i = 0; i < NUMBER_SHIPS; i++)
	{
		count += ps->at(i)->isSunk() ? 1 : 0;
	}
	if (count == 5)
		return 1;
	else
	{
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
