#include "DllSmartAlgo.h"

const int DllSmartAlgo::NOT_INITIALIZED = -1;
const std::vector<std::pair<int, int>> DllSmartAlgo::placesToCheckBoard = { std::make_pair(-1, 0),std::make_pair(1, 0),std::make_pair(0, 1),std::make_pair(0, -1)};
const std::vector<std::pair<int, int>> DllSmartAlgo::placesToDelete = { std::make_pair(-1, -1),std::make_pair(-1 ,1), std::make_pair(1, -1), std::make_pair(1, 1 ) };

std::pair<int, int> DllSmartAlgo::attack()
{
	std::unique_ptr<std::pair<int, int>> hit;

	// Checking if there is already possible move - if so, rand over the moves, if not, rand over the board till found place that can place a ship
	if((this->possibleMoves).size() == 0)
	{
		hit = getRandomAttack();
		//std::cout << "used getRandomAttack(): ";
	}
	// Existing possible moves, choose randomly between them
	else {
		std::cout << "possible moves are: ";
		for (int i = 0; i <(this->possibleMoves).size(); ++i)
		{
			 std::cout << "("<< ((this->possibleMoves).at(i)).first + 1 << "," << ((this->possibleMoves).at(i)).second + 1 << "), ";
		}
		std::cout << std::endl;
		int r = rand() % (this->possibleMoves).size();
		hit = std::make_unique<std::pair<int,int>>(this->possibleMoves[r]);
		possibleMoves.erase(possibleMoves.begin() + r);
	}
	if((*hit).first!=-1)
	{
		std::cout << "hit returned from smartDLL is: (" << (*hit).first+1 << "," << (*hit).second+1 << ") "<<std::endl;
		return std::make_pair((*hit).first + 1, (*hit).second + 1);
	}
	return std::make_pair(-1, -1);
}

void DllSmartAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
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
	this-> addToPossibleMove(DllSmartAlgo::getPossibleMoves(row, col));

	//// Getting all the possible moves after the board change, suppose to be only 2 possible moves - left right or up down
	//for (std::pair<int, int> attacked : this->shipPositionHit) {

	//	attacks = getPossibleMoves(attacked.first, attacked.second);
	//	this->possibleMoves.reserve((this->possibleMoves).size() + attacks.size());
	//	(this->possibleMoves).insert(std::end(this->possibleMoves), std::begin(attacks), std::end(attacks));
	//}
}

void DllSmartAlgo::firstHit(int row, int col) {

	this->possibleMoves = DllSmartAlgo::getPossibleMoves(row, col);

	DllSmartAlgo::changeSurrounding(row, col, false);
}

// Checking all the possible places for a move to be
std::vector<std::pair<int, int>> DllSmartAlgo::getPossibleMoves(int row, int col) {

	std::vector<std::pair<int, int>> positions;
	std::pair<int, int> positionToHit;

	std::cout << "INSIDE getPossibleMoves()" << std::endl;
	for (std::pair<int, int> pair : placesToCheckBoard)
	{
		std::cout << "\nrow + pair.first, col + pair.second are: " << row + pair.first << "," << col + pair.second << " \n";
		positionToHit = (checkPosition(row + pair.first, col + pair.second))? std::make_pair(row + pair.first, col + pair.second) : std::make_pair(-1,-1);
		std::cout << "position checked to add is: (" << positionToHit.first << "," << positionToHit.second << ")" << std::endl;
		if (positionToHit.first != -1 && positionToHit.second != -1) {
			std::cout << "position added: (" << positionToHit.first << "," << positionToHit.second << ")" << std::endl;
			positions.push_back(positionToHit);
		}
	}
	std::cout << "positions are: ";
	for (std::pair<int, int> pair: positions)
	{
		std::cout << "(" << pair.first << "," << pair.second << ")    " << std::endl;
	}
	std::cout << std::endl;
	return positions;
}

// Checking if the position in range and was not hit before
bool DllSmartAlgo::checkPosition(int row, int col) const {

	std::cout << "*****************************\n";
	BoardCreator::printBoard(const_cast<const char**>(board), numRows, numCols);
	if (Ship::inBoard(row) && Ship::inBoard(col))
	{
		std::cout << "REAL (0-9) row and col are: " << row << "," << col << " and board[row][col] is: " << board[row][col] << std::endl;
		if(board[row][col] != HIT_WRONG)
		{
			if (!(Ship::isShip(board[row][col])))
			{
				std::cout << "got here!" << std::endl;
				if (board[row][col] != HIT_ENEMY)
				{
					std::cout << "returning true!" << std::endl;
					return true;
				}
			}
		}
	}

	return false;
}

std::unique_ptr<std::pair<int, int>> DllSmartAlgo::getRandomAttack()
{
	std::unique_ptr<std::vector<std::pair<int, int>>> retVector = getAllPossiblePoints();
	//std::cout << "potential moves: ";
	//for (std::vector<std::pair<int, int>>::iterator iter = retVector->begin(); iter != retVector->end(); ++iter) {
	//	 std::cout <<"("<< (*iter).first <<","<< (*iter).second <<"), ";
	//}
	//std::cout << std::endl;
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

 int DllSmartAlgo::getPositionOfMove(int p1, int p2)
 {
	 std::pair<int, int> p;
	for(int i = 0; i < possibleMoves.size(); ++i)
	{
		p = possibleMoves.at(i);
		if (p.first==p1 && p.second==p2)
		{
			return i;
		}
	}
	return -1;
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
	int k = -1;
	for (std::pair<int, int> toDelete : this->placesToDelete) {

		if (Ship::inBoard(row + toDelete.first) && Ship::inBoard(col + toDelete.second)) {

			this->board[row + toDelete.first][col + toDelete.second] = HIT_WRONG;
			k = getPositionOfMove(row + toDelete.first, col + toDelete.second);
			if (k!=-1)
			{
				std::cout << "Deleting position: " << row + toDelete.first << "," << col + toDelete.second << std::endl;
				possibleMoves.erase(possibleMoves.begin() + k);
			}
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