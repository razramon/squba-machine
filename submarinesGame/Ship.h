#pragma once

class Ship {

	char letter;
	int numberOfPoints;
	int ** position; // Array of size of the ship, has 3 properties: 1. row ; 2. column ; 3. is hit - 1 for hit, 0 otherwise
	Ship& operator=(const Ship& s) = delete;//assignments operator - disabeled
	
	public:

	Ship(char letter);
	Ship();
	~Ship();
	Ship(const Ship& s);//copy constructor
	char getLetter();
	void setLetter(char l);
	int getNumberOfPoints();
	void setNumberOfPoints(int nop);
	static bool isShip(char c);
	static int pointsOfShip(char c);
	static int sizeOfShip(char c);
	bool isSunk();
	int** getPosition();
	int getShipSize();
	void setPosition(int pos, int row, int col, int state);
	void printShipInfo();
};