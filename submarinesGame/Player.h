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
	int playerNum = -1;
	int totalNumberOfPoints = 0;
	int attackNumber = 0;
	char* pathToAttackFile = "";
	std::vector<std::pair<int,int>> attacks;

	Player();

	void setBoard(const char ** board, int numRows, int numCols) override;

	int isHit(int row, int col);

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	std::pair<int, int> attack() override;

};

