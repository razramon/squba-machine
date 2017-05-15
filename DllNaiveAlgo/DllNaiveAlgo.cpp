#include "DllNaiveAlgo.h"

const int DllNaiveAlgo::NOT_INITIALIZED = -1;
const std::vector<std::pair<int, int>> DllNaiveAlgo::placesToCheck = { { -1, -1 },{ -1 ,1 },{ 1, -1 },{ 1, 1 },{ -1, 0 },{ 1, 0 },{ 0, 1 },{ 0, -1 } };

std::pair<int, int> DllNaiveAlgo::attack()
{
	bool found = false;
	std::pair<int, int> posToAttack;
	// Looping over the indexes of the row and col
	while (Ship::inBoard(this->indexRow)) {

		while (Ship::inBoard(this->indexCol)) {

			// Searching where there isn't a neighbor or position that was attacked
			if (this->board[this->indexRow][this->indexCol] != HIT_ENEMY && !DllNaiveAlgo::hasNeighbor(this->indexRow, this->indexCol) 
				&& !Ship::isShip(this->board[this->indexRow][this->indexCol])) {

				// Saving the position to attack, we need that all will be + 1 for the gameManager
				posToAttack = std::make_pair(this->indexRow + 1, this->indexCol + 1);
				this->indexCol += 1;
				found = true;
				break;
			}
			this->indexCol += 1;
		}

		// Checking if still in range, if not, reset the col and going down a row
		if (this->indexCol >= BOARD_LENGTH) {

			this->indexCol = 0;
			this->indexRow += 1;
		}
		if (found)
			break;

	}
	// Was not found - meaning we went over the whole board and there isn't a play left
	if (!found)
		return std::make_pair(-1, -1);

	return posToAttack;
}

bool DllNaiveAlgo::hasNeighbor(int row, int col) {

	bool hasNeighbor = false;
	for (std::pair<int, int> pair : DllNaiveAlgo::placesToCheck) {

		// Checking if:
		// the position is in the board, after it, checking if is a ship / is a hit on the enemy. if one of them does not exist, returning false.
		hasNeighbor = Ship::inBoard(row + pair.first) && Ship::inBoard(col + pair.second)
			&& Ship::isShip(this->board[row + pair.first][col + pair.second]);
		if (hasNeighbor)
			break;
	}
	return hasNeighbor;

}

void DllNaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	this->board[row - 1][col - 1] = HIT_ENEMY;
}


DllNaiveAlgo::DllNaiveAlgo() :board(nullptr), numRows(NOT_INITIALIZED), numCols(NOT_INITIALIZED),
				indexRow(0), indexCol(0), player(NOT_INITIALIZED)
{

}

DllNaiveAlgo::~DllNaiveAlgo()
{
	BoardCreator::freeBoard(board, numRows);
}

void DllNaiveAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	this->board = BoardCreator::copyBoard(board, numRows, numCols);
	this->player = player;
	this->numRows = numRows;
	this->numCols = numCols;
}

bool DllNaiveAlgo::init(const std::string& path)
{
	for (int indexRow = 0; indexRow < numRows; indexRow++)
	{
		for (int indexCol = 0; indexCol < numCols; indexCol++)
		{
			if (DllNaiveAlgo::hasNeighbor(indexRow, indexCol) && !Ship::isShip(this->board[indexRow][indexCol]) ){

				this->board[indexRow][indexCol] = HIT_ENEMY;
			}
		}
	}
	return true;
}

IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllNaiveAlgo();
	return ptrToAlg;
}

