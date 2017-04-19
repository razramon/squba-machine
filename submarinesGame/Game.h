#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>

class Game : public IBattleshipGameAlgo
{
public:
	Game();
	~Game();

	void setBoard(const char ** board, int numRows, int numCols) override;

	int isHit(int row, int col);

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;

	std::pair<int, int> attack() override;

private:
	std::vector<Ship> ships[2];
};

