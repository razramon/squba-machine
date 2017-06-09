#include "DllSmartAlgo.h"

const int DllSmartAlgo::NOT_INITIALIZED = -1;
const std::vector<Coordinate> DllSmartAlgo::placesToCheckBoard = { Coordinate(0, -1, 0),Coordinate(0, 1, 0), Coordinate(0, 0, -1),
																	Coordinate(0, 0, 1),Coordinate(-1, 0, 0), Coordinate(1, 0, 0) };
const std::vector<Coordinate> DllSmartAlgo::placesToDelete = { Coordinate(0, 1, -1),Coordinate(0, 1, 1),
																Coordinate(0, -1, -1),Coordinate(0, -1, 1), 
																Coordinate(-1, 1, -1),Coordinate(-1, 1, 0), 
																Coordinate(-1, 1, 1),Coordinate(-1, 0, -1), 
																Coordinate(-1, 0, 1),Coordinate(-1, -1, -1), 
																Coordinate(-1, -1, 0),Coordinate(-1, -1, 1), 
																Coordinate(1, 1, -1),Coordinate(1, 1, 0), 
																Coordinate(1, 1, 1),Coordinate(1, -1, -1), 
																Coordinate(1, -1, 0),Coordinate(1, -1, 1), 
																Coordinate(1, 0, -1),Coordinate(1, 0, 1), };

Coordinate DllSmartAlgo::attack()
{
	std::unique_ptr<Coordinate> hit;

	// Checking if there is already possible move - if so, rand over the moves, if not, rand over the board till found place that can place a ship
	if((this->possibleMoves).size() == 0)
	{
		hit = getRandomAttack();
	}
	// Existing possible moves, choose randomly between them
	else {

		int r = rand() % (this->possibleMoves).size();
		hit = std::make_unique<Coordinate>(this->possibleMoves[r]);
		possibleMoves.erase(possibleMoves.begin() + r);
	}
	if((*hit).row!=-1)
	{
		return Coordinate((*hit).row + 1, (*hit).col + 1, (*hit).depth + 1);
	}
	return Coordinate(-1, -1,-1);
}

void DllSmartAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result)
{
	row -= 1;
	col -= 1;
	if (player == this->player) {
		switch (result)
		{
		case AttackResult::Hit:
			
			this->shipPositionHit.push_back(std::make_pair(row, col));
			this->board[row][col] = HIT_ENEMY;

			// If first hit of the ship
			if (this->shipPositionHit.size() == 1) {

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

void DllSmartAlgo::sinkBigShip(Coordinate c) {

	for (Coordinate toSink : this->shipPositionHit) {

		DllSmartAlgo::changeSurrounding(toSink, true);
	}
	this->possibleMoves.clear();

}

void DllSmartAlgo::sinkSmallShip(Coordinate c)
{
	this->possibleMoves.clear();

	DllSmartAlgo::changeSurrounding(c, true);
}


void DllSmartAlgo::hitShip(Coordinate c) {

	DllSmartAlgo::changeSurrounding(c, false);
	std::vector<Coordinate> attacks;
	this-> addToPossibleMove(DllSmartAlgo::getPossibleMoves(c));
}

void DllSmartAlgo::firstHit(Coordinate c) {

	this->possibleMoves = DllSmartAlgo::getPossibleMoves(c);

	DllSmartAlgo::changeSurrounding(c, false);
}

// Checking all the possible places for a move to be
std::vector<Coordinate> DllSmartAlgo::getPossibleMoves(Coordinate c) {

	std::vector<Coordinate> positions;
	for (Coordinate trio : placesToCheckBoard)
	{
		Coordinate temp = Coordinate(c.row + trio.row, c.col + trio.col, c.depth + trio.depth);
		if(checkPosition(temp))
		{
			positions.push_back(temp);
		}
	}
	return positions;
}

// Checking if the position in range and was not hit before
bool DllSmartAlgo::checkPosition(Coordinate c) const {

	if (inBoardBoarders(c))
	{
		char ch = (*board)[c.depth][c.row][c.col];
		if( ch != HIT_WRONG)
		{
			if (!(Ship::isShip(ch)))
			{
				if (ch != HIT_ENEMY)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool DllSmartAlgo::inBoardBoarders(Coordinate & c) const
{
	return (0 <= c.row && c.row < numRows && 0 <= c.col && c.col < numCols && 0 <= c.depth && c.depth < numDepth);
}

std::unique_ptr<Coordinate> DllSmartAlgo::getRandomAttack()
{
	std::unique_ptr<std::vector<Coordinate>> retVector = std::move(getAllPossiblePoints());
	if(retVector->size()==0)
	{
		return std::make_unique<std::pair<int, int>>(std::make_pair(-1, -1));
	}
	int k = rand() % (retVector->size());
	return std::make_unique<std::pair<int, int>>(retVector->at(k)) ;
}

 std::unique_ptr<std::vector<std::pair<int, int>>> DllSmartAlgo::getAllPossiblePoints()
 {
	std::unique_ptr<std::vector<std::pair<int, int>>> retVector = std::make_unique<std::vector<std::pair<int, int>>>();
	for (int row = 0; row < numRows; ++row)
	{
		for (int col = 0; col < numCols; ++col)
		{
			if (board[row][col]!= HIT_WRONG && !Ship::isShip(board[row][col]) && board[row][col] != HIT_ENEMY)
			{
				retVector->push_back(std::make_pair(row, col)); //returning values 0-9!
			}
		}
	}
	return retVector;
 }



 void DllSmartAlgo::addToPossibleMove(std::vector<std::pair<int, int>> pm)
 {

	for (std::pair<int, int> p: pm)
	{
		if ((this->getPositionOfMove(p.first,p.second))==-1)//meaning it isn't in possibleMoves
		{
			(this->possibleMoves).push_back(std::make_pair(p.first, p.second));
		}
	}

 }

 int DllSmartAlgo::getPositionOfMove(Coordinate c)
 {
	int i = 0;
	for(std::vector<Coordinate>::iterator iter = possibleMoves.begin(); iter != possibleMoves.end(); ++iter)
	{
		if ((*iter).row==c.row && (*iter).col == c.col && (*iter).depth == c.depth)
		{
			return i;
		}
		++i;
	}
	return -1;
 }

DllSmartAlgo::DllSmartAlgo():board(nullptr), numRows(NOT_INITIALIZED), numCols(NOT_INITIALIZED),
	numDepth(NOT_INITIALIZED), player(NOT_INITIALIZED)
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
			if (Ship::inBoard(row + toDelete.first) && Ship::inBoard(col + toDelete.second)
				&& !Ship::isShip(this->board[row + toDelete.first][col +toDelete.second])
				&& this->board[row + toDelete.first][col + toDelete.second] != HIT_ENEMY){

				this->board[row + toDelete.first][col + toDelete.second] = HIT_WRONG;
			}
		}
	}
	// Delete from the board all the wrong position if found a hit
	int k = -1;
	for (std::pair<int, int> toDelete : this->placesToDelete) {

		if (Ship::inBoard(row + toDelete.first) && Ship::inBoard(col + toDelete.second)) {

			this->board[row + toDelete.first][col + toDelete.second] = HIT_WRONG;
			k = getPositionOfMove(row + toDelete.first, col + toDelete.second);
			if (k!=-1)
			{
				possibleMoves.erase(possibleMoves.begin() + k);
			}
		}
		
	}
	
}

void DllSmartAlgo::setPlayer(int player)
{
	this->player = player;
}

void DllSmartAlgo::setBoard(const BoardData& board)
{
	
	this->numRows = board.rows();
	this->numCols = board.cols();
	this->numDepth = board.depth();
	this->board = std::move(BoardCreator::createBoard(numRows, numCols, numDepth));
	//TODO:: create a nested loop to investigate and update our board!
}

IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllSmartAlgo();
	return ptrToAlg;
}