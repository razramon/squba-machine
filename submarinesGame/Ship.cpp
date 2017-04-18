#include "Ship.h"
#include "Sizes.h"
#include <iostream>

using namespace std;
	
Ship::Ship(char letter)
{
	this->letter = letter;
	this->numberOfPoints = Ship::pointsOfShip(letter);
	this->position = new int*[sizeOfShip(letter)];
	for (int i = 0; i < sizeOfShip(letter); ++i)
	{
		position[i] = new int[3];
	}
}
Ship::Ship()
{
}
Ship::~Ship()
{
	for(int i = 0; i < this->getShipSize(); ++i)
	{
		delete position[i];
	}
	delete[] position;
}
char Ship::getLetter() {
	return letter;
}
void Ship::setLetter(char l) {
	letter = l;
}
int Ship::getNumberOfPoints() {
	return numberOfPoints;
}
void Ship::setNumberOfPoints(int nop) {
	numberOfPoints = nop;
}

// Array of size of the ship, 
// has 3 properties: 1. row ; 2. column ; 3. is hit - 1 for hit, 0 otherwise
int ** position;

bool Ship::isShip(char c)
{
	c = tolower(c);
	return (c == 'm' || c == 'b' || c == 'd' || c == 'p') ? true : false;
}

int Ship::pointsOfShip(char c)
{
	c = tolower(c);
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
	c = tolower(c);
	switch (c)
	{
	case 'b': return B_SIZE;

	case 'p': return P_SIZE;

	case 'm': return M_SIZE;

	case 'd': return D_SIZE;

	default: return -1;
	}
}

bool Ship::isSunk()
{
	for(int i = 0; i < sizeOfShip(this->getLetter()); i++)
	{
		if (this->position[i][2] == 0)
			return false;
	}
	return true;
}

int** Ship::getPosition()
{
	return position;
}
int Ship::getShipSize()
{
	return Ship::sizeOfShip(letter);
}

void Ship::setPosition(int pos, int row, int col, int state)
{
	if (pos < 0 || pos >= this->getShipSize() || row < 0 || col < 0) return;
	
	this->position[pos][0] = row;
	this->position[pos][1] = col;
	this->position[pos][2] = state;	
}

void Ship::printShipInfo()
{
	std::cout << "This is ship: " << letter << " of player " << (islower(letter) ? "B" : "A") << std::endl;
	for (int k = 0; k < Ship::sizeOfShip(letter) - 1; ++k)
	{
		std::cout << "(" << position[k][0] << "," << position[k][1] << ")  ,  ";
	}
	std::cout << "(" << position[Ship::sizeOfShip(letter) - 1][0] << "," << position[Ship::sizeOfShip(letter) - 1][1] << ")" << std::endl;
}
