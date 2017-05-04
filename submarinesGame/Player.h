#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include "Sizes.h"
#include <vector>
#include "Exception.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include "Utilities.h"

#define END_READ_ATTACK_FILE -1

class Player : public IBattleshipGameAlgo
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
	static char getLetterByNumber(int number);
	int getPlayerNumber() const;
	void attackResOnPlayer(char shipLetter, int row, int col, AttackResult result) const; //this function is called by Game to announce this Player he's been hit
	bool Player::hasAttack() const;
	Ship** getShips();
	void setBoard(const char ** board, int numRows, int numCols) override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	std::pair<int, int> attack() override;//returns (a copy of) the i'th players attack: IN RANGE 1-10!! (not 0-9)
};

