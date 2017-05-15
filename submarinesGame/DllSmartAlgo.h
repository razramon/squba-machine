#pragma once
#include "../submarinesGame/IBattleshipGameAlgo.h"
#include "../submarinesGame/BoardCreator.h"
#include "../submarinesGame/Sizes.h"
#include "../submarinesGame/Ship.h"
#include <stdlib.h>
#include <vector>
#include <memory>
#define HIT_WRONG 'c'
#define HIT_ENEMY '@'


class DllSmartAlgo : public IBattleshipGameAlgo
{
	static const int NOT_INITIALIZED;
	static const std::vector<std::pair<int, int>> placesToCheckBoard;
	static const std::vector<std::pair<int, int>> placesToDelete;
	char** board;
	int numRows;
	int numCols;
	int player;
	/*
	 * Contains pairs of variables 1-10! 
	 */
	std::vector<std::pair<int, int>> possibleMoves;
	std::vector<std::pair<int, int>> shipPositionHit;
	std::vector<std::pair<int, int>> enemyHitSelf;

	std::unique_ptr<std::pair<int, int>> getRandomAttack();
	std::unique_ptr<std::vector<std::pair<int, int>>> getAllPossiblePoints();

	void addToPossibleMove(std::vector<std::pair<int, int>> pm);
	int getPositionOfMove(int p1, int p2);
	void changeSurrounding(int row, int col, bool initBoard);

	void sinkBigShip(int row, int col);

	void sinkSmallShip(int row, int col);

	void hitShip(int row, int col);

	void firstHit(int row, int col);

	std::vector<std::pair<int, int>> getPossibleMoves(int row, int col);

	bool checkPosition(int col, int row) const;

public:

	DllSmartAlgo();
	~DllSmartAlgo();
	
	void setBoard(int player, const char** board, int numRows, int numCols) override;		// called once to notify player on his board
	bool init(const std::string& path) override;		// called once to allow init from files if needed returns whether the init succeeded or failed

	std::pair<int, int> attack() override;													// ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;	// notify on last move result
};
