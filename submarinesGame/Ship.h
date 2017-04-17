#pragma once

class Ship {
private:
	char letter;
	int numberOfPoints;
public:
	Ship(char letter);
	Ship();
	~Ship();
	char getLetter();
	void setLetter(char l);
	int getNumberOfPoints();
	void setNumberOfPoints(int nop);
	static bool isShip(char c);
	static int pointsOfShip(char c);
	static int sizeOfShip(char c);
	int ** position; // Array of size of the ship, has 3 properties: 1. row ; 2. column ; 3. is hit - 1 for hit, 0 otherwise
	int** getPosition();
	int getShipSize();
};