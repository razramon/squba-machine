#include "Player.h"

using namespace std;

std::vector<std::pair<int, int>>* Player::getAttackFile(const char * attackFile)
{
	vector<pair<int, int>>* attacks = new vector<pair<int, int>>();

	//opening attackFile:
	std::ifstream bfile(attackFile);
	if (!bfile) {
		delete attacks;
		bfile.close();
		throw Exception("Error: failed opening attack file.");
	}
	std::string line;
	while (!bfile.eof())
	{
		try
		{
			Utilities::getAllKindsOfLine(bfile, line);
			if (line == "" || line.find(",") == std::string::npos || line.find_first_of(",") != line.find_last_of(","))
				continue;
		}
		catch (std::exception& e)
		{
			delete attacks;
			std::string s("Error: failed when reading attack file: ");
			s.append(e.what());
			bfile.close();
			throw Exception(s.c_str());
		}

		// Getting each number, deleting spaces and turning to integer
		std::string temp = line.substr(0, line.find(","));
		std::string row = Utilities::delSpaces(temp);
		temp.clear();
		temp = line.substr(line.find(",") + 1, line.length());
		std::string col = Utilities::delSpaces(temp);
		pair<int, int> attack;

		attack.first = stoi(row);
		attack.second = stoi(col);

		// Check if in range
		if (attack.first > 10 || attack.first < 1 || attack.second > 10 || attack.second < 1)
		{
			continue;
		}
		(*attacks).push_back(attack);
	}
	bfile.close();
	return attacks;
}

Player::Player(int playerNum, std::string fullPathToAttackFile, std::vector<Ship*>* ships)
{
	if((playerNum!=PLAYER_A)&&(playerNum!=PLAYER_B))
	{
		throw Exception("Error: trying to create a player with a wrong player-number.");
	}
	this->playerNum = playerNum;

	try
	{
		attacks = getAttackFile(fullPathToAttackFile.c_str());
	} catch (exception &e)
	{
		throw e;
	}
	this->attackNumber = 0;

	this->ships = new Ship*[NUMBER_SHIPS];
	for (int i = 0; i < NUMBER_SHIPS; ++i)
	{
		this->ships[i] =new Ship(*((*ships).at(i)));//using assignment - creates a copy
	}
}

Player::~Player()
{
	for (int i = 0; i < NUMBER_SHIPS; ++i)
	{
		delete this->ships[i]; //TODO:: check this.. also make sure no memory leaks accure!
	}
	delete[] ships;
	delete attacks;
}

void Player::attackResOnPlayer(char shipLetter, int row, int col, AttackResult result) const
{		
	switch (result)
	{
	case AttackResult::Miss:
		return;
	case AttackResult::Hit:
	case AttackResult::Sink:
		for (int i = 0; i < NUMBER_SHIPS; ++i) {
			if ((*this->ships[i]).getLetter() == shipLetter) {
				int** pos = (*(*this).ships[i]).getPosition();
				for (int j = 0; j < (*(*this).ships[i]).getShipSize(); ++j) {
					if (pos[j][0] == row && pos[j][1] == col) {
						(*(*this).ships[i]).setPosition(j, row, col, 1);
					}
				}
			}

		}
		break;
	default:
		break;
	}

}

std::pair<int, int> Player::attack()
{
	if (attackNumber < 0 || attackNumber>=(*attacks).size())
	{
		return std::pair<int, int>(-1, -1);
	}
	++attackNumber;
	if (attackNumber == (*attacks).size())
	{
		attackNumber = -1;
		return ((*attacks).at(((*attacks).size()) - 1));
	}
	return ((*attacks).at(attackNumber-1));
}
/*
 * Returns true if this player still has an attack.
 */
bool Player::hasAttack() const
{
	if (attackNumber < 0 || attackNumber >= (*attacks).size())
	{
		return false;
	}
	return true;
}

Ship ** Player::getShips()
{
	return this->ships;
}

int Player::getAttackNumber() const
{
	return attackNumber;
}

char Player::getLetterByNumber(int number)
{
	return (number == 0 ? 'A' : 'B');
}

int Player::getPlayerNumber() const
{
	return playerNum;
}

void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result)
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

void Player::setBoard(const char** board, int numRows, int numCols)
{
	//this->board = new char*[numRows];
	//for (int indexRow = 0; indexRow < numRows; indexRow++)
	//{
	//	this->board[indexRow] = new char[numCols];

	//	for (int indexCol = 0; indexCol < numCols; indexCol++)
	//	{
	//		this->board[indexRow][indexCol] = board[indexRow][indexCol];
	//	}
	//}
}