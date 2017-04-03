#include <iostream>
#include "Ship.h"
using namespace std;

class Ship {
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

	int ** position;
};