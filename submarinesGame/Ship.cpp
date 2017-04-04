#include <iostream>
#include "Ship.h"
using namespace std;

class Ship{
private:
	char letter;
	int numberOfPoints;
	
public:
	char getLetter() {
		return letter;
	}
	void setLetter(char l) {
		letter = l;
	}
	int getNumberOfPoints() {
		return numberOfPoints;
	}
	void setNumberOfPoints(int nop) {
		numberOfPoints = nop;
	}

	// Array of size of the ship, 
	// has 3 properties: 1. row ; 2. column ; 3. is hit - 1 for hit, 0 otherwise
	int ** position;
};