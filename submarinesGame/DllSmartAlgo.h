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
	 * All 3 vectors: possibleMoves,shipPositionHit,enemyHitSelf 
	 * Contain Coordinates indexed from 0!! 
	 */
	std::vector<Coordinate> possibleMoves;
	std::vector<Coordinate> shipPositionHit;
	std::vector<Coordinate> enemyHitSelf;


	/*
	 * Returns a random attack, Indexed from 0
	 */
	std::unique_ptr<Coordinate>& getRandomAttack() const;
	/*
	* Returns vector of all Coordinates that we CAN attack, Indexed from 0
	*/
	std::unique_ptr<std::vector<Coordinate>>& getAllPossiblePoints() const;

	void addToPossibleMove(std::vector<Coordinate> pm);
	int getPositionOfMove(const Coordinate& c);
	void changeSurrounding(Coordinate& c, bool initBoard);

	void sinkBigShip(Coordinate c);

	void sinkSmallShip(Coordinate c);

	void hitShip(Coordinate c);

	void firstHit(Coordinate c);

	std::vector<Coordinate> getPossibleMoves(Coordinate c) const;

	bool checkPosition(const Coordinate& c) const;
	bool inBoardBorders(const Coordinate& c) const;
	void deleteFromPossibleMoves(const Coordinate& c);

public:

	DllSmartAlgo();
	~DllSmartAlgo();
	void setPlayer(int player) override;
	void setBoard(const BoardData& board) override;		// called once to notify player on his board
	Coordinate attack() override;						// ask player for his move
	void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override;	// notify on last move result
};
