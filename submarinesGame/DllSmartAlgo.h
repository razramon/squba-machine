#pragma once
#include "IBattleshipGameAlgo.h"
#include <vector>
#include "Sizes.h"
#define HIT_WRONG 'c'
#define HIT_ENEMY '@'


class DllSmartAlgo : public IBattleshipGameAlgo
{
	char** board;
	int numRows;
	int numCols;
	int moveNum = 0;
	int player = -1;
	std::vector<std::pair<int, int>> possibleMoves;
	std::vector<std::pair<int, int>> shipPositionHit;
	std::vector<std::pair<int, int>> placesToCheckBoard = { {-1, 0}, {1, 0}, {0, 1},{0, -1} };
	std::vector<std::pair<int, int>> placesToDelete = { {-1, -1}, {-1 ,1}, {1, -1}, {1, 1} };

public:

	DllSmartAlgo();
	~DllSmartAlgo();
	void changeSurrounding(int row, int col, bool initBoard);
	void setBoard(int player, const char** board, int numRows, int numCols) override;		// called once to notify player on his board
	bool init(const std::string& path) override;		// called once to allow init from files if needed returns whether the init succeeded or failed
	bool isShip(char c);
	std::pair<int, int> attack() override;													// ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;	// notify on last move result

	void sinkBigShip(int row, int col);

	void sinkSmallShip(int row, int col);

	void hitShip(int row, int col);

	void firstHit(int row, int col);

	std::vector<std::pair<int, int>> getPossibleMoves(int row, int col);


	std::pair<int, int> checkPosition(int col, int row);

	static bool inBoard(int place);

};
