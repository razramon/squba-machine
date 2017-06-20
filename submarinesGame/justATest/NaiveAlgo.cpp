#include "NaiveAlgo.h"

std::pair<int, int> NaiveAlgo::attack()
{
	bool found = false;

	for (int curRow = this->indexRow; curRow < BOARD_LENGTH; curRow++) {

		for (int curCol = this->indexCol; curRow < BOARD_LENGTH; curCol++) {

			// Checking if one of the neighbors is a ship or hit on enemy (still ship), if so, dont let it be an attack 
			if (!hasNeighbor(curRow, curCol)) {

				found = true;

				// Checking if still in the range of the board
				if (NaiveAlgo::inBoard(curCol + 1)) {

					this->indexCol = curCol + 1;
					this->indexRow = curRow;
				}
				else {
					this->indexCol = 0;

					// Checking if still in the range of the board
					if(NaiveAlgo::inBoard(curRow + 1))

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

// Check if the position is in the range of the board
bool NaiveAlgo::inBoard(int place) {

	return place < BOARD_LENGTH && place >= 0;
}

bool NaiveAlgo::hasNeighbor(int row, int col) {

	bool hasNeighbor = false;
	for (std::pair<int, int> pair: this->placesToCheck) {

		// Checking if:
		// the position is in the board, after it, checking if is a ship / is a hit on the enemy. if one of them does not exist, returning false.
		hasNeighbor = NaiveAlgo::inBoard(row + pair.first) && NaiveAlgo::inBoard(col + pair.second) 
			&& (NaiveAlgo::isShip(this->board[row + pair.first][col + pair.second]) || this->board[row + pair.first][col + pair.second] != HIT_ENEMY);
		if (hasNeighbor)
			break;
	}
	return hasNeighbor;

}

void NaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	if (result == AttackResult::Hit || result == AttackResult::Sink) {

		this->board[row][col] = HIT_ENEMY;
	}
}

void NaiveAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	this->board = new char*[numRows];
	for(int indexRow = 0; indexRow < numRows; indexRow++)
	{
		this->board[indexRow] = new char[numCols];
		for(int indexCol = 0; indexCol < numCols; indexCol++)
		{
			this->board[indexRow][indexCol] = board[indexRow][indexCol];
		}
	}
	this->player = player;
	this->numRows = numRows;
	this->numCols = numCols;
}

bool NaiveAlgo::init(const std::string& path)
{
	return true;
}

bool NaiveAlgo::isShip(char c)
{
	c = tolower(c);
	return (c == 'm' || c == 'b' || c == 'd' || c == 'p') ? true : false;
}
