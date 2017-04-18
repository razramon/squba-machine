#pragma once

#include "ISubmarinesGameAlgo.h"
#include "Ship.h"
#include "Sizes.h"
#include <vector>

#define END_READ_ATTACK_FILE -1
#define HIT '*'
#define SINK '@'

class Player : public IBattleshipGameAlgo
{
public:
	Ship ships[NUMBER_SHIPS];
	int playerNum;
	int totalNumberOfPoints;
	int attackNumber;
	char* pathToAttackFile;
	std::vector<std::pair<int,int>> attacks;

	int isHit(int row, int col);
	void setBoard(const char** board, int numRows, int numCols);
	void notifyOnAttackResult(int player, int row, int col, AttackResult result);
	std::pair<int, int> attack();
};

