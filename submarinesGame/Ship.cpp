#include "Ship.h"
#include "Sizes.h"
#include <iostream>

using namespace std;
	
Ship::Ship(char letter)
{
	this->letter = letter;
	this->numberOfPoints = Ship::pointsOfShip(letter);
	this->position = new int*[Ship::sizeOfShip(letter)];
}

Ship::Ship()
{
}
Ship::~Ship()
{
	for (int i = 0; i < Ship::sizeOfShip(this->letter);++i)
	{
		delete[] this->position[i];
	}
	delete[] this->position;
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

int** Ship::getPosition()
{
	return position;
}

int Ship::getShipSize()
{
	return Ship::sizeOfShip(letter);
}