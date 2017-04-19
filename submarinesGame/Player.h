#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include "Sizes.h"
#include <vector>
#include "Exception.h"
#include <algorithm>
#include <fstream>

#define END_READ_ATTACK_FILE -1
#define HIT '*'
#define SINK '@'

class Player
{
	mutable Ship** ships;
	int playerNum;
	int attackNumber;
	std::vector<std::pair<int, int>>* attacks;
	static std::vector<std::pair<int, int>>* getAttackFile(const char* attackFile);
	//For future use: add here a vector of indexes where player's attacks hit (the other player)
	
	public:
	Player(int playerNum, std::string fullPathToAttackFile, std::vector<Ship*>* ships);
	~Player();
	int getAttackNumber() const;
	void attackResOnPlayer(char shipLetter, int row, int col, AttackResult result) const; //this function is called by Game to announce this Player he's been hit
	std::pair<int, int>* getAttack(); //returns (a copy of) the i'th players attack: IN RANGE 1-10!! (not 0-9)
	Ship** getShips();
};

