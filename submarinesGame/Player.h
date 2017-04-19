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
	Ship* ships;
	int playerNum;
	int attackNumber;
	std::vector<std::pair<int, int>> attacks;
	static std::vector<std::pair<int, int>> getAttackFile(const char* attackFile);
	//For future use: add here a vector of indexes where player's attacks hit
	
	public:
	Player(int playerNum, std::string fullPathToAttackFile, Ship* ships);
	~Player();
	int getAttackNumber() const;
	void attackResOnPlayer(char shipLetter, int row, int col, AttackResult result); //this function is called by Game to announce this Player he's been hit
	std::pair<int, int> getAttack(int i); //returns the i'th players attack 
};

