#include "DllSmartAlgo.h"

const int DllSmartAlgo::NOT_INITIALIZED = -1;
const std::vector<std::pair<int, int>> DllSmartAlgo::placesToCheckBoard = { { -1, 0 },{ 1, 0 },{ 0, 1 },{ 0, -1 } };
const std::vector<std::pair<int, int>> DllSmartAlgo::placesToDelete = { { -1, -1 },{ -1 ,1 },{ 1, -1 },{ 1, 1 } };
std::pair<int, int> DllSmartAlgo::attack()
{
	std::pair<int, int> hit;

	// Checking if there is already possible move - if so, rand over the moves, if not, rand over the board till found place that can place a ship
	if((this->possibleMoves).size() == 0)
	{
		bool hitFound = false;

		// Looping on the board with rand in order to find a position to hit
		while (!hitFound) {

			hit = std::make_pair(rand() % numRows, rand() % numCols);

			// Checking if wasn't already hit or is my ship
			if (board[hit.first][hit.second] != HIT_WRONG && !Ship::isShip(board[hit.first][hit.second]) && board[hit.first][hit.second] != HIT_ENEMY) {
				
				hitFound = true;
			}
		}
	}
	// Existing possible moves, choose randomly between them
	else {
		hit = this->possibleMoves[rand() % (this->possibleMoves).size()];
	}

	return std::make_pair(hit.first + 1, hit.second + 1);
}

void DllSmartAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	row--;
	col--;
	if (player == this->player) {
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
	else {
		switch (result)
		{
		case AttackResult::Hit:

			if (!Ship::isShip(this->board[row][col])) {

				this->board[row][col] = HIT_ENEMY;
				enemyHitSelf.push_back(std::make_pair(row, col));
				DllSmartAlgo::changeSurrounding(row, col, false);
			}
			break;

		case AttackResult::Sink:

			this->board[row][col] = HIT_ENEMY;
			DllSmartAlgo::changeSurrounding(row, col, true);
			break;

		case AttackResult::Miss:

			this->board[row][col] = HIT_WRONG;
			break;

		default:
			break;
		}
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
		this->possibleMoves.reserve((this->possibleMoves).size() + attacks.size());
		(this->possibleMoves).insert(std::end(this->possibleMoves), std::begin(attacks), std::end(attacks));
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

	if (Ship::inBoard(row) && Ship::inBoard(col) && this->board[row][col] != HIT_WRONG && !Ship::isShip(this->board[row][col]) && this->board[row][col] != HIT_ENEMY) {

		positionToHit = std::make_pair(row, col);
	}
	else {
		positionToHit = std::make_pair(-1, -1);
	}
	return positionToHit;
}

DllSmartAlgo::DllSmartAlgo():board(nullptr), numRows(NOT_INITIALIZED), numCols(NOT_INITIALIZED),
	player(NOT_INITIALIZED)
{
}

DllSmartAlgo::~DllSmartAlgo()
{
	BoardCreator::freeBoard(this->board, numRows);
}

// Changing the board to the new surrounding
void DllSmartAlgo::changeSurrounding(int row, int col, bool sink) {

	// If sink - place all around the ship , include init
	if (sink) {

		for (std::pair<int, int> toDelete : this->placesToCheckBoard) {

			// Checking in range and not my ship
			if (Ship::inBoard(row + toDelete.first) && Ship::inBoard(col + toDelete.second)
				&& !Ship::isShip(this->board[row + toDelete.first][col +toDelete.second])
				&& this->board[row + toDelete.first][col + toDelete.second] != HIT_ENEMY){

				this->board[row + toDelete.first][col + toDelete.second] = HIT_WRONG;
			}
		}
	}
	// Delete from the board all the wrong position if found a hit
	for (std::pair<int, int> toDelete : this->placesToDelete) {

		if (Ship::inBoard(row + toDelete.first) && Ship::inBoard(col + toDelete.second)) {

			this->board[row + toDelete.first][col + toDelete.second] = HIT_WRONG;
		}
	}
	
}

void DllSmartAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	this->board = BoardCreator::copyBoard(board, numRows, numCols);
	this->player = player;
	this->numRows = numRows;
	this->numCols = numCols;
}

bool DllSmartAlgo::init(const std::string& path)
{
	bool success = true;

	for (int indexRow = 0; indexRow < numRows; indexRow++)
	{
		for (int indexCol = 0; indexCol < numCols; indexCol++)
		{
			if (Ship::isShip(this->board[indexRow][indexCol]))
			{
				DllSmartAlgo::changeSurrounding(indexRow, indexCol, true);
			}
		}
	}

	return success;
}

IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllSmartAlgo();
	return ptrToAlg;
}