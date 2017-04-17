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

	bool isHit(int row, int col);

};

