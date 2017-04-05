#include "Ship.h"
#include <iostream>

using namespace std;
	
Ship::Ship(char letter, int numberOfPoints)
{
	this->letter = letter;
	this->numberOfPoints = numberOfPoints;
}
Ship::Ship()
{
}
Ship::~Ship()
{
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
