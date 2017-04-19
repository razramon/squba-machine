#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>
#include "Player.h"

class Game : public IBattleshipGameAlgo
{
public:
	Game(std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips, std::string* fullPathToAttackFileA, std::string* fullPathToAttackFileB);
	~Game();

	void setBoard(const char ** board, int numRows, int numCols) override;

	int isHit(int row, int col);

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	std::pair<int, int> attack() override;

private:
	std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips;
	char ** board;
};

