#pragma once
#include "Sizes.h"
#include <iostream>

class Ship
{
	static const int LEN_OF_POS_AND_RESULT = 4;
	char letter;
	int numberOfPoints;
	int** position; // Array of size of the ship, has 4 properties: 0. row ; 1. column ; 2. depth ; 3. is hit - 1 for hit, 0 otherwise
	Ship& operator=(const Ship& s) = delete;//assignments operator - disabeled

public:

	explicit Ship(char letter);
	~Ship();
	Ship(const Ship& s);//copy constructor
	char getLetter() const;
	void setLetter(char l);
	int getNumberOfPoints() const;
	void setNumberOfPoints(int nop);
	static bool isShip(char c);
	static int pointsOfShip(char c);
	static int sizeOfShip(char c);
	bool isSunk() const;
	int** getPosition();
	int getShipSize() const;
	void setPosition(int pos, int row, int col, int dep, int state);
	void printShipInfo() const;
	int numOfHits() const;
	int shipOfPlayer() const;
	enum INDEX_3D
	{
		row_index = 0,
		column_index = 1,
		depth_index = 2,
		is_hit_index = 3
	};
};