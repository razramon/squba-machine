#include <iostream>
#include "Ship.h"
using namespace std;

class Ship {
private:
	char letter;
	int startPosition;
	int endPosition;
	int numberOfPoints;
	
public:
	char getLetter() {
		return letter;
	}
	void setLetter(char l) {
		letter = l;
	}
	int getStartPosition() {
		return startPosition;
	}
	void setStartPosition(int sp) {
		startPosition = sp;
	}
	int getEndPosition() {
		return endPosition;
	}
	void setEndPosition(int ep) {
		endPosition = ep;
	}
	int getNumberOfPoints() {
		return numberOfPoints;
	}
	void setNumberOfPoints(int nop) {
		numberOfPoints = nop;
	}
};