#include "DllSmartAlgo.h"

std::pair<int, int> DllSmartAlgo::attack()
{
	std::pair<int, int> hit;

	// Checking if there is already possible move - if so, rand over the moves, if not, rand over the board till found place that can place a ship
	if(this->possibleMoves.size() > 0)
	{
		bool hitFound = false;

		// Looping on the board with rand in order to find a position to hit
		while (!hitFound) {

			hit = std::make_pair(rand() % 10, rand() % 10);

			// Checking if wasn't already hit or is my ship
			if (board[hit.first][hit.second] != HIT_WRONG || !isShip(board[hit.first][hit.second]) || board[hit.first][hit.second] != HIT_ENEMY) {
				hitFound = true;
			}
		}
	}
	// Existing possible moves, choose randomly between them
	else {

		hit = this->possibleMoves[rand() % (this->possibleMoves.size() - 1)];
	}

	return hit;
}

void DllSmartAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	switch (result)
	{
	case AttackResult::Hit:

		this->shipPositionHit.push_back(std::make_pair(row, col));
		this->board[row][col] = HIT_ENEMY;

		// If first hit of the ship
		if (this->possibleMoves.size() != 0) {

			// Create the vector for possible moves
			DllSmartAlgo::firstHit(row, col);
		}
		else {

			// Continue the vector 
			DllSmartAlgo::hitShip(row, col);
		}
		break;

	case AttackResult::Sink:
		this->shipPositionHit.clear();
		this->board[row][col] = HIT_ENEMY;

		// First hit first sink, small ship
		if (this->possibleMoves.size() != 0) {

			DllSmartAlgo::sinkSmallShip(row, col);
		}
		else {
			// Big ship, need to check all the ship
			DllSmartAlgo::sinkBigShip(row, col);
		}

		this->possibleMoves.clear();
		break;

	case AttackResult::Miss:

		this->board[row][col] = HIT_WRONG;
		break;

	default:
		break;
	}
}

void DllSmartAlgo::sinkBigShip(int row, int col) {

	for (std::pair<int, int> toSink : this->shipPositionHit) {

		DllSmartAlgo::changeSurrounding(toSink.first, toSink.second, true);
	}
	this->possibleMoves.clear();

}

void DllSmartAlgo::sinkSmallShip(int row, int col)
{
	this->possibleMoves.clear();

	DllSmartAlgo::changeSurrounding(row, col, true);
}

void DllSmartAlgo::hitShip(int row, int col) {

	DllSmartAlgo::changeSurrounding(row, col, false);
	std::vector<std::pair<int, int>> attacks;
	this->possibleMoves = DllSmartAlgo::getPossibleMoves(row, col);

	// Getting all the possible moves after the board change, suppose to be only 2 possible moves - left right or up down
	for (std::pair<int, int> attacked : this->shipPositionHit) {

		attacks = getPossibleMoves(attacked.first, attacked.second);
		this->possibleMoves.insert(this->possibleMoves.end(), attacks.begin(), attacks.end());
	}
}

void DllSmartAlgo::firstHit(int row, int col) {

	this->possibleMoves = DllSmartAlgo::getPossibleMoves(row, col);

	DllSmartAlgo::changeSurrounding(row, col, false);
}

// Checking all the possible places for a move to be
std::vector<std::pair<int, int>> DllSmartAlgo::getPossibleMoves(int row, int col) {

	std::vector<std::pair<int, int>> positions;
	std::pair<int, int> positionToHit;

	for (std::pair<int, int> pair : this->placesToCheckBoard)
	{
		positionToHit = DllSmartAlgo::checkPosition(row + pair.first, col + pair.second);
		if (positionToHit.first != -1 && positionToHit.second != -1) {

			positions.push_back(positionToHit);
		}
	}

	return positions;
}

// Checking if the position in range and was not hit before
std::pair<int,int> DllSmartAlgo::checkPosition(int col, int row) {

	std::pair<int, int> positionToHit;

	if (DllSmartAlgo::inBoard(row) && DllSmartAlgo::inBoard(col) && this->board[row][col] != HIT_WRONG) { // TODO: add if the position is my ship
		positionToHit = std::make_pair(row, col);

	}
	else {
		positionToHit = std::make_pair(-1, -1);
	}
	return positionToHit;
}

// Check if the position is in the range of the board
bool DllSmartAlgo::inBoard(int place) {

	return place < BOARD_LENGTH && place >= 0;
}


// To do: להעיף את כל מה שלא צריך להיות פה לקומון - כולל IsShip וכו
//TODO : add ctor dtor

DllSmartAlgo::DllSmartAlgo()
{
}

DllSmartAlgo::~DllSmartAlgo()
{
}

// Changing the board to the new surrounding
void DllSmartAlgo::changeSurrounding(int row, int col, bool sink) {

	// If sink - place all around the ship , include init
	if (sink) {

		for (std::pair<int, int> toDelete : this->placesToCheckBoard) {

			// Checking in range and not my ship
			if (DllSmartAlgo::inBoard(row + toDelete.first) && DllSmartAlgo::inBoard(col + toDelete.second)
				&& !DllSmartAlgo::isShip(this->board[row + toDelete.first][col +toDelete.second])
				&& this->board[row + toDelete.first][col + toDelete.second] != HIT_ENEMY){

				this->board[row + toDelete.first][col + toDelete.second] = HIT_WRONG;
			}
		}
	}
	// Delete from the board all the wrong position if found a hit
	for (std::pair<int, int> toDelete : this->placesToDelete) {

		if (DllSmartAlgo::inBoard(row + toDelete.first) && DllSmartAlgo::inBoard(col + toDelete.second)) {

			this->board[row + toDelete.first][col + toDelete.second] = HIT_WRONG;
		}
	}
	
}

void DllSmartAlgo::setBoard(int player, const char** board, int numRows, int numCols)
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

bool DllSmartAlgo::init(const std::string& path)
{
	bool success = true;

	for (int indexRow = 0; indexRow < numRows; indexRow++)
	{
		this->board[indexRow] = new char[numCols];
		for (int indexCol = 0; indexCol < numCols; indexCol++)
		{
			if (isShip(this->board[indexRow][indexCol]))
			{
				DllSmartAlgo::changeSurrounding(indexRow, indexCol, true);
			}
		}
	}

	return success;
}

bool DllSmartAlgo::isShip(char c)
{
	c = tolower(c);
	return (c == 'm' || c == 'b' || c == 'd' || c == 'p') ? true : false;
}


IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllSmartAlgo();
	return ptrToAlg;
}