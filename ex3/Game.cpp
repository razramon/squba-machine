#include "Game.h"


char Game::getLetterByNumber(int number)
{
	return (number == 0 ? 'A' : 'B');
}

bool Game::noAttacksLeft(Coordinate& attackOfPlayer)
{
	return (attackOfPlayer.row == -1 && attackOfPlayer.col == -1 && attackOfPlayer.depth == -1);
}

/*
 * Returns true if attack is of format: ([1-10],[1-10],[1-10]) or (-1,-1,-1)
 */
bool Game::isValAttack(Coordinate& attackOfPlayer) const
{
	return ((1 <= attackOfPlayer.row && attackOfPlayer.row <= numRows) &&
			(1 <= attackOfPlayer.col && attackOfPlayer.col <= numCols) &&
			(1 <= attackOfPlayer.depth && attackOfPlayer.depth <= numDepth)) ||
		noAttacksLeft(attackOfPlayer);
}

std::shared_ptr<Ship> Game::getShipAtPosition(int row, int col, int depth) const
{
	if (row < 0 || row >= numRows || col < 0 || col >= numCols || depth < 0 || depth >= numDepth) return nullptr;

	for (auto i = 0; i < 2; ++i)
	{
		ptrToShipsVector ps = (i == 0) ? (*playersShips).first : (*playersShips).second;
		for (std::vector<std::shared_ptr<Ship>>::iterator iter = (*ps).begin(); iter!=(*ps).end(); ++iter)
		{
			for (auto j = 0; j < (**iter).getShipSize(); ++j)
			{
				if (row == (*(**iter).getPosition())[j][Ship::INDEX_3D::row_index] && 
					col == (*(**iter).getPosition())[j][Ship::INDEX_3D::column_index] && 
					depth == (*(**iter).getPosition())[j][Ship::INDEX_3D::depth_index])
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
bool Game::playersHaveAttack(Coordinate& attackOfPlayer)
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
void Game::notifyPlayers(Coordinate& currAttack, AttackResult& result) const
{
	if (currAttack.row == -1) return;
	(*playerA).notifyOnAttackResult(playerPlaying, currAttack, result);
	(*playerB).notifyOnAttackResult(playerPlaying, currAttack, result);
}

void Game::initializePlayer(IBattleshipGameAlgo * algo, int p_Number, BoardData& b)
{
	algo->setPlayer(p_Number); //Notifies player his number
	algo->setBoard(b); //sets the player's board
}

void Game::resetPlayersShips() const
{
	for (auto ship_to_reset = (*(this->playersShips->first)).begin(); ship_to_reset!= (*(this->playersShips->first)).end(); ++ship_to_reset)
	{
		(**ship_to_reset).resetShip();
	}
	for (auto ship_to_reset = (*(this->playersShips->second)).begin(); ship_to_reset != (*(this->playersShips->second)).end(); ++ship_to_reset)
	{
		(**ship_to_reset).resetShip();
	}
}

//large init list, don't panic! :)
Game::Game(std::string dllAName, std::string dllBName, std::shared_ptr<GetAlgoFuncType> playerA,
	std::shared_ptr<GetAlgoFuncType> playerB, std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> > board,
	std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector>> playersShips, int numRows, int numCols, int numDepth) :
	playersShips(playersShips), playerPlaying(PLAYER_A), playerA((*playerA)()), playerB((*playerB)()),
	points(std::make_pair(0, 0)), shipSunk(std::make_pair(0, 0)), numRows(numRows), numCols(numCols), numDepth(numDepth),
	dllNames(std::make_pair(dllAName, dllBName)), board(board)
{
	if (playersShips == nullptr)
	{
		throw Exception("PRINT_NOTHING"); //Appropriate errors have already been printed in "checkBoard"
	}
	try
	{
		initializePlayer(this->playerA.get(), PLAYER_A, *((this->board).first.get())  );
		initializePlayer(this->playerB.get(), PLAYER_B, *((this->board).second.get()) );
	}
	catch (std::exception& e)
	{
		throw e;
	}
	resetPlayersShips(); //make sure all ships are "alive" when starting the game. 
}


int Game::isHit(int row, int col, int depth, char& letter) const
{
	std::shared_ptr<Ship> s = nullptr;
	std::shared_ptr<std::vector<std::vector<int>>> posArray;
	// Looping over all ships
	for (int j = 0; j < 2; ++j)
	{
		ptrToShipsVector ps = (j == 0) ? (*playersShips).first : (*playersShips).second;
		for (auto i = 0; i < (*ps).size(); ++i)
		{
			s = (*ps)[i];
			if (s->isSunk())
				continue;
			posArray = ((*s).getPosition());
			for (auto pos = 0; pos < (*s).getShipSize(); ++pos)
			{
				if (row == (*posArray)[pos][Ship::INDEX_3D::row_index] &&
					col == (*posArray)[pos][Ship::INDEX_3D::column_index] &&
					depth == (*posArray)[pos][Ship::INDEX_3D::depth_index])
				{
					letter = (*s).getLetter();
					switch ((*posArray)[pos][Ship::INDEX_3D::is_hit_index])
					{
					case 0:
						(*s).setPosition(pos, row, col, depth, HIT);
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
	letter = DEFAULT_LETTER;
	return MISS;
}

std::unique_ptr<GameInfo> Game::game()
{
	int damaged = 0;;
	int win = -1;
	char letter = DEFAULT_LETTER;
	Coordinate curAttack(-1, -1, -1);
	int row, col, depth;

	int maxNumberOfMoves = this->numCols * this->numDepth * this->numRows * 5; // An arbitrary number, just to put maximum to a game.
	int numberMoves = 0;

	while (win == -1 && playersHaveAttack(curAttack) && numberMoves < maxNumberOfMoves)
	{
		numberMoves++;

		letter = DEFAULT_LETTER;
		AttackResult result = AttackResult::Miss;
		row = curAttack.row - 1;
		col = curAttack.col - 1;
		depth = curAttack.depth - 1;
		damaged = isHit(row, col, depth, letter);
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

	if(numberMoves >= maxNumberOfMoves && win == -1)
	{
		Logger::instance().log("Too much game moves, ended with no winners", Logger::LogLevelError);
	} else if(win == -1)
	{
		Logger::instance().log("Game ended with a tie: doesn't count as win/lose for both players", Logger::LogLevelInfo);
		//Note: a tie is considered when counting game rounds!
	}

	return std::make_unique<GameInfo>(win, dllNames, points);

}

int Game::checkWin() const
{
	for (auto j = 0; j < 2; ++j)
	{
		ptrToShipsVector ps = (j == 0) ? (*playersShips).first : (*playersShips).second;
		int count = 0;
		for (auto i = 0; i < (*ps).size(); i++)
		{
			count += ps->at(i)->isSunk() ? 1 : 0;
		}
		if (count == ps->size()){
			return (j == 0 ? PLAYER_B : PLAYER_A); //if j==0, we're checking A's sanked ships, and if there are 5 of those - B won.
		}
	}
	return -1;
}