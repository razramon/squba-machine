#include "DllNaiveAlgo.h"

const int DllNaiveAlgo::NOT_INITIALIZED = -1;
const std::vector<std::pair<int, int>> DllNaiveAlgo::placesToCheck = { { -1, -1 },{ -1 ,1 },{ 1, -1 },{ 1, 1 },{ -1, 0 },{ 1, 0 },{ 0, 1 },{ 0, -1 } };

std::pair<int, int> DllNaiveAlgo::attack()
{
	bool found = false;

	for (int curRow = this->indexRow; curRow < BOARD_LENGTH; curRow++) {

		for (int curCol = this->indexCol; curRow < BOARD_LENGTH; curCol++) {

			// Checking if one of the neighbors is a ship or hit on enemy (still ship), if so, dont let it be an attack 
			if (!hasNeighbor(curRow, curCol)) {

				found = true;

				// Checking if still in the range of the board
				if (Ship::inBoard(curCol + 1)) {

					this->indexCol = curCol + 1;
					this->indexRow = curRow;
				}
				else {
					this->indexCol = 0;

					// Checking if still in the range of the board
					if (Ship::inBoard(curRow + 1))

						this->indexRow = curRow + 1;
					else {

						this->indexCol = -1;
						this->indexRow = -1;
					}
				}
				break;
			}
		}
		if (found)
			break;
	}

	return std::make_pair(this->indexRow, this->indexCol);
}

bool DllNaiveAlgo::hasNeighbor(int row, int col) {

	bool hasNeighbor = false;
	for (std::pair<int, int> pair : DllNaiveAlgo::placesToCheck) {

		// Checking if:
		// the position is in the board, after it, checking if is a ship / is a hit on the enemy. if one of them does not exist, returning false.
		hasNeighbor = Ship::inBoard(row + pair.first) && Ship::inBoard(col + pair.second)
			&& (Ship::isShip(this->board[row + pair.first][col + pair.second]) || this->board[row + pair.first][col + pair.second] != HIT_ENEMY);
		if (hasNeighbor)
			break;
	}
	return hasNeighbor;

}

void DllNaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	if (result == AttackResult::Hit || result == AttackResult::Sink) {

		this->board[row - 1][col - 1] = HIT_ENEMY;
	}
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
	this->board = new char*[numRows];
	for (int indexRow = 0; indexRow < numRows; indexRow++)
	{
		this->board[indexRow] = new char[numCols];
		for (int indexCol = 0; indexCol < numCols; indexCol++)
		{
			this->board[indexRow][indexCol] = board[indexRow][indexCol];
		}
	}
	this->player = player;
	this->numRows = numRows;
	this->numCols = numCols;
}

bool DllNaiveAlgo::init(const std::string& path)
{
	return true;
}

IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllNaiveAlgo();
	return ptrToAlg;
}

