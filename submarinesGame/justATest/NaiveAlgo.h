#pragma once
#include "IBattleshipGameAlgo.h"
#include <vector>
#include "Sizes.h"
#define HIT_ENEMY '@'

class NaiveAlgo : public IBattleshipGameAlgo
{
	char** board;
	int numRows;
	int numCols;
	int indexRow = 0;
	int indexCol = 0;
	int player = -1;
	std::vector<std::pair<int, int>> placesToCheck = { { -1, -1 },{ -1 ,1 },{ 1, -1 },{ 1, 1 },{ -1, 0 },{ 1, 0 },{ 0, 1 },{ 0, -1 } };

public:
	void setBoard(int player, const char** board, int numRows, int numCols) override;		// called once to notify player on his board
	bool init(const std::string& path) override;		// called once to allow init from files if needed returns whether the init succeeded or failed
	bool isShip(char c);
	std::pair<int, int> attack() override;
	bool inBoard(int place);
	bool hasNeighbor(int row, int col);
	// ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;	// notify on last move result

};
