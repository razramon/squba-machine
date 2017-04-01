#pragma once
class Ship {
private:
	char letter;
	int startPosition;
	int endPosition;
	int numberOfPoints;
public:
	char getLetter();
	void setLetter(char l);
	int getStartPosition();
	void setStartPosition(int sp);
	int getEndPosition();
	void setEndPosition(int ep);
	int getNumberOfPoints();
	void setNumberOfPoints(int nop);
};
