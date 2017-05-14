#pragma once
#include "../submarinesGame/IBattleshipGameAlgo.h"
#include "../submarinesGame/BoardCreator.h"
#include "../submarinesGame/Sizes.h"
#include "../submarinesGame/Ship.h"
#include <stdlib.h>
#include <vector>
#define HIT_WRONG 'c'
#define HIT_ENEMY '@'


class DllSmartAlgo : public IBattleshipGameAlgo
{
	static const int DllSmartAlgo::NOT_INITIALIZED;
	static const std::vector<std::pair<int, int>> DllSmartAlgo::placesToCheckBoard;
	static const std::vector<std::pair<int, int>> DllSmartAlgo::placesToDelete;
	char** board;
	int numRows;
	int numCols;
	int player;
	std::vector<std::pair<int, int>> possibleMoves;
	std::vector<std::pair<int, int>> shipPositionHit;


public:

	DllSmartAlgo();
	~DllSmartAlgo();
	void changeSurrounding(int row, int col, bool initBoard);
	void setBoard(int player, const char** board, int numRows, int numCols) override;		// called once to notify player on his board
	bool init(const std::string& path) override;		// called once to allow init from files if needed returns whether the init succeeded or failed

	std::pair<int, int> attack() override;													// ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;	// notify on last move result

	void sinkBigShip(int row, int col);

	void sinkSmallShip(int row, int col);

	void hitShip(int row, int col);

	void firstHit(int row, int col);

	std::vector<std::pair<int, int>> getPossibleMoves(int row, int col);

	std::pair<int, int> checkPosition(int col, int row);

};
