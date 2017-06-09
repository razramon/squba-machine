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
	static const std::vector<Coordinate> placesToCheckBoard;
	static const std::vector<Coordinate> placesToDelete;
	std::unique_ptr<boardType> board;
	int numRows;
	int numCols;
	int numDepth;
	int player;
	/*
	 * Contains pairs of variables 1-10! 
	 */
	std::vector<Coordinate> possibleMoves;
	std::vector<Coordinate> shipPositionHit;
	std::vector<Coordinate> enemyHitSelf;

	std::unique_ptr<Coordinate> getRandomAttack();
	std::unique_ptr<std::vector<Coordinate>> getAllPossiblePoints();

	void addToPossibleMove(std::vector<Coordinate> pm);
	int getPositionOfMove(Coordinate c);
	void changeSurrounding(Coordinate c, bool initBoard);

	void sinkBigShip(Coordinate c);

	void sinkSmallShip(Coordinate c);

	void hitShip(Coordinate c);

	void firstHit(Coordinate c);

	std::vector<Coordinate> getPossibleMoves(Coordinate c);

	bool checkPosition(Coordinate c) const;
	bool inBoardBoarders(Coordinate& c) const;

public:

	DllSmartAlgo();
	~DllSmartAlgo();
	void setPlayer(int player) override;
	void setBoard(const BoardData& board) override;		// called once to notify player on his board
	Coordinate attack() override;						// ask player for his move
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override;	// notify on last move result
};
