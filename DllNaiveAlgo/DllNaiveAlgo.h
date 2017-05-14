#pragma once
#include "../submarinesGame/IBattleshipGameAlgo.h"
#include "../submarinesGame/BoardCreator.h"
#include "../submarinesGame/Sizes.h"
#include "../submarinesGame/Ship.h"
#include <vector>
#define HIT_ENEMY '@'

class DllNaiveAlgo : public IBattleshipGameAlgo
{
	static const int DllNaiveAlgo::NOT_INITIALIZED;
	static const std::vector<std::pair<int, int>> placesToCheck;
	char** board;
	int numRows;
	int numCols;
	int indexRow;
	int indexCol;
	int player;
	bool hasNeighbor(int row, int col);

public:
	DllNaiveAlgo();
	~DllNaiveAlgo();
	void setBoard(int player, const char** board, int numRows, int numCols) override;		// called once to notify player on his board
	bool init(const std::string& path) override;		// called once to allow init from files if needed returns whether the init succeeded or failed
	std::pair<int, int> attack() override;
	// ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;	// notify on last move result

};
