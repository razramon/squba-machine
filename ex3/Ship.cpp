#include "Ship.h"

using namespace std;

Ship::Ship(char letter) : letter(letter), numberOfPoints(pointsOfShip(letter)),
			position(std::vector<vector<int>>(sizeOfShip(letter)))
{

	for (auto i = 0; i < sizeOfShip(letter); ++i)
	{
		position[i].resize(LEN_OF_POS_AND_RESULT);
	}

}


Ship::Ship(const Ship& s)
{
	this->letter = s.letter;
	this->numberOfPoints = pointsOfShip(letter);
	this->position = std::vector<vector<int>>(sizeOfShip(letter));
	for (auto i = 0; i < sizeOfShip(letter); ++i)
	{
		this->position[i].resize(LEN_OF_POS_AND_RESULT);
	}
	for (auto i = 0; i < sizeOfShip(letter); ++i)
	{
		for (auto j = 0; j < LEN_OF_POS_AND_RESULT; ++j)
		{
			this->position[i][j] = s.position[i][j];
		}
	}
}

char Ship::getLetter() const
{
	return letter;
}

void Ship::setLetter(char l)
{
	letter = l;
}

int Ship::getNumberOfPoints() const
{
	return numberOfPoints;
}

void Ship::setNumberOfPoints(int nop)
{
	numberOfPoints = nop;
}


bool Ship::isShip(char c)
{
	c = static_cast<char>(tolower(c));
	return (c == 'm' || c == 'b' || c == 'd' || c == 'p') ? true : false;
}



int Ship::pointsOfShip(char c)
{
	c = static_cast<char>(tolower(c));
	switch (c)
	{
	case 'b': return B_POINTS;

	case 'p': return P_POINTS;

	case 'm': return M_POINTS;

	case 'd': return D_POINTS;

	default: return -1;
	}
}

int Ship::sizeOfShip(char c)
{
	c = static_cast<char>(tolower(c));
	switch (c)
	{
	case 'b': return B_SIZE;

	case 'p': return P_SIZE;

	case 'm': return M_SIZE;

	case 'd': return D_SIZE;

	default: return -1;
	}
}

bool Ship::isSunk() const
{
	for (int i = 0; i < sizeOfShip(this->getLetter()); i++)
	{
		if (this->position[i][3] == 0)
			return false;
	}
	return true;
}

std::shared_ptr<std::vector<std::vector<int>>> Ship::getPosition()
{
	return std::make_shared<std::vector<std::vector<int>>>(position);
}

int Ship::getShipSize() const
{
	return Ship::sizeOfShip(letter);
}

void Ship::setPosition(int pos, int row, int col, int dep, int state)
{
	if (pos < 0 || pos >= this->getShipSize() || row < 0 || col < 0) return;

	this->position[pos][0] = row;
	this->position[pos][1] = col;
	this->position[pos][2] = dep;
	this->position[pos][3] = state;
}

/* Testing Only */
void Ship::printShipInfo() const
{
	std::cout << "This is ship: " << letter << " of player " << (islower(letter) ? "B" : "A") << std::endl;
	std::cout << "Printing format of positions is: (depth, row, column) + <HIT ?YES, NO>" << std::endl;
	for (int k = 0; k < Ship::sizeOfShip(letter) - 1; ++k)
	{
		std::cout << "(" << position[k][2] << "," << position[k][0] << "," << position[k][1] << ")  <"<< (position[k][3] ==1? "YES":"NO") << "> ,  ";
	}
	std::cout << "(" << position[Ship::sizeOfShip(letter) - 1][2] << "," << position[Ship::sizeOfShip(letter) - 1][0] << "," << position[Ship::sizeOfShip(letter) - 1][1] << ")  <" << (position[Ship::sizeOfShip(letter) - 1][3] == 1 ? "YES" : "NO") << "> " << std::endl;
}


int Ship::numOfHits() const
{
	int hits = 0;
	//counts number of hits in ship s
	for (int i = 0; i < this->getShipSize(); ++i)
	{
		if (position[i][3] == 1)
		{
			hits++;
		}
	}
	return hits;
}

int Ship::shipOfPlayer() const
{
	return ((islower(letter)) ? PLAYER_B : PLAYER_A);
}

void Ship::resetShip()
{
	for (int k = 0; k < Ship::sizeOfShip(letter); ++k)
	{
		this->position[k][3] = 0;
	}
}
