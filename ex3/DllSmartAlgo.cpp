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

/*
 * Returns an attack,
 * Note: start index is 1 and not 0 - as clarified here:
 * http://moodle.tau.ac.il/mod/forum/discuss.php?d=62549
 */
Coordinate DllSmartAlgo::attack()
{
	std::unique_ptr<Coordinate> hit;

	// Checking if there is already possible move - if so, rand over the moves, if not, rand over the board till found place that can place a ship
	if((this->possibleMoves).size() == 0)
	{
		hit = std::move(getRandomAttack());
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

/*
 * Note: Coordinate "move" is scaled from index 1 and not 0!
 */
void DllSmartAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result)
{
	Coordinate coordInGameFormat = Coordinate(move.row - 1, move.col - 1, move.depth - 1); //Same coordinate as "move", just with indexes from 0!
	if (player == this->player) {
		switch (result)
		{
		case AttackResult::Hit:
			this->shipPositionHit.push_back(coordInGameFormat);
			(*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col] = HIT_ENEMY;

			// If first hit of the ship
			if (this->shipPositionHit.size() == 1) {

				// Create the vector for possible moves
				firstHit(coordInGameFormat);
			}
			else {

				// Update the vector "possible moves"
				hitShip(coordInGameFormat);
			}
			break;

		case AttackResult::Sink:
			this->shipPositionHit.push_back(coordInGameFormat);
			(*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col] = HIT_ENEMY;

			// First hit first sink, than this is a small ship
			if (this->possibleMoves.size() != 0) {
				sinkSmallShip(coordInGameFormat);
			}
			else {
				// Big ship, need to check all the ship
				sinkBigShip(coordInGameFormat);
			}
			break;

		case AttackResult::Miss:

			(*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col] = HIT_WRONG;
			break;

		default:
			break;
		}
	}
	else {
		switch (result)
		{
		case AttackResult::Hit:

			//Checks if enemy hit himself
			if (!Ship::isShip((*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col])){
				(*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col] = HIT_ENEMY;
				enemyHitSelf.push_back(coordInGameFormat);
				if (getPositionOfMove(coordInGameFormat) != -1) //means that this was a suspected coordinate
				{
					shipPositionHit.push_back(coordInGameFormat);
					hitShip(coordInGameFormat);
				} else //it wasn't a suspected coordinate
				{
					changeSurrounding(coordInGameFormat, false);
					//TODO: we might check if coordInGameFormat is a neighbour of suspected coords,
					// and add here prioritation to attack-moves using it... 
				}
			}
			break;

		case AttackResult::Sink:
			//Checks if enemy sank himself
			if (!Ship::isShip((*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col]))
			{
				(*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col] = HIT_ENEMY;
				
				//if enemy sank himself using a coordinate that is in our possibleMoves,
				//we update the surroundings and empty possible moves because this is the ship we tried to sank (using sink sinkBigShip)
				if (getPositionOfMove(coordInGameFormat) != -1)  
				{
					sinkBigShip(coordInGameFormat);
				} else
				{
					changeSurrounding(coordInGameFormat, false); //it's false on purpse! so it won't delete our "possibleMoves"
					//TODO:: take care of edge cases...
				}
			}

			break;

		case AttackResult::Miss:

			(*board)[coordInGameFormat.depth][coordInGameFormat.row][coordInGameFormat.col] = HIT_WRONG;
			break;

		default:
			break;
		}
	} 
}

void DllSmartAlgo::sinkBigShip(Coordinate c) {
	for (Coordinate toSink : this->shipPositionHit) {
		changeSurrounding(toSink, true);
	}
	this->possibleMoves.clear();
	this->shipPositionHit.clear();
}


void DllSmartAlgo::sinkSmallShip(Coordinate c)
{	
	changeSurrounding(c, true);
	this->possibleMoves.clear();
	this->shipPositionHit.clear();
}

/*
 * Note: call hitShip() AFTER updating shipPositionHit to contain Coordinate c!
 */
void DllSmartAlgo::hitShip(Coordinate c) {
	DllSmartAlgo::changeSurrounding(c, false);
	std::vector<Coordinate> attacks;
	this->possibleMoves.clear(); //Deletes all current possible moves

	// Getting all the possible moves after the board changed, suppose to be only 2 possible moves - left & right / up & down / depth-up & depth-down
	for (Coordinate attacked : this->shipPositionHit) {
		addToPossibleMove(getPossibleMoves(attacked));
	}
}

void DllSmartAlgo::firstHit(Coordinate c) {

	this->possibleMoves = DllSmartAlgo::getPossibleMoves(c);

	DllSmartAlgo::changeSurrounding(c, false);
}

/*
 * Gets a coordinate (where an enemy ship was hit),
 * Returnes a vector of all possible places for the rest of this ship to be
 */
std::vector<Coordinate> DllSmartAlgo::getPossibleMoves(Coordinate c) const{
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

/*
 *  Returns true if coordinate c is:
 *  1. inside board's range
 *  2. wasn't hit before
 *  3. doesn't contain our own ship 
 */
bool DllSmartAlgo::checkPosition(const Coordinate& c) const {

	if (inBoardBorders(c))
	{
		char ch = (*board)[c.depth][c.row][c.col];
		if(ch != HIT_WRONG)
		{
			if (!(Ship::isShip(ch))) //makes sure it isn't our ship
			{
				if (ch != HIT_ENEMY) //makes sure it wasn't hit before
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*
 * Returns true if coordinate c is inside this.board 's range
 */
bool DllSmartAlgo::inBoardBorders(const Coordinate & c) const
{
	return (0 <= c.row && c.row < numRows && 0 <= c.col && c.col < numCols && 0 <= c.depth && c.depth < numDepth);
}

/*
 * Searches this.possibleMoves for a coordinate with the same values as c's,
 * If finds one - deletes it. 
 */
void DllSmartAlgo::deleteFromPossibleMoves(const Coordinate & c)
{
	int k = getPositionOfMove(c);
	if (k != -1)
	{
		possibleMoves.erase(possibleMoves.begin() + k);
	}
}

std::unique_ptr<Coordinate> DllSmartAlgo::getRandomAttack() const
{
	std::unique_ptr<std::vector<Coordinate>> retVector = std::move(getAllPossiblePoints());
	if(retVector->size()==0) //No possible moves
	{
		return std::make_unique<Coordinate>(Coordinate(-1, -1, -1));
	}
	int k = rand() % (retVector->size());
	return std::make_unique<Coordinate>(retVector->at(k));
}

 std::unique_ptr<std::vector<Coordinate>> DllSmartAlgo::getAllPossiblePoints() const
 {
	std::unique_ptr<std::vector<Coordinate>> retVector = std::make_unique<std::vector<Coordinate>>();
	for (int depth = 0; depth < numDepth; ++depth)
	{
		for (int row = 0; row < numRows; ++row)
		{
			for (int col = 0; col < numCols; ++col)
			{
				if ((*board)[depth][row][col] != HIT_WRONG && !Ship::isShip((*board)[depth][row][col]) && (*board)[depth][row][col] != HIT_ENEMY)
				{
					retVector->push_back(Coordinate(row, col, depth)); //returning values indexed from 0!
				}
			}
		}
	}

	return std::move(retVector);
 }

/*
 * Adds all coordinates in pm to this.possibleMoves
 */
 void DllSmartAlgo::addToPossibleMove(std::vector<Coordinate> pm)
 {
	for (Coordinate p: pm)
	{
		if ((this->getPositionOfMove(p))==-1)//meaning it isn't in already possibleMoves
		{
			(this->possibleMoves).push_back(p);
		}
	}

 }

/*
 * Returns the index of coordinate c inside this.possibleMoves,
 *			-1 if it isn't in possibleMoves
 */
 int DllSmartAlgo::getPositionOfMove(const Coordinate& c)
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

/*
 * Note: didn't initialize: possibleMoves, shipPositionHit, enemyHitSelf
 * Because according to: https://stackoverflow.com/a/11725466/5928769, 
 * there's no need to, because they are vectors..
 */
DllSmartAlgo::DllSmartAlgo():board(nullptr), numRows(NOT_INITIALIZED), numCols(NOT_INITIALIZED),
	numDepth(NOT_INITIALIZED), player(NOT_INITIALIZED)
{
}

DllSmartAlgo::~DllSmartAlgo()
{
}

/*
 * Update this.board to the new surroundings
 */
void DllSmartAlgo::changeSurrounding(Coordinate& c, bool sink) {
	// If sink - then update all relevant places on board
	if (sink) {
		for (Coordinate toDelete : this->placesToCheckBoard) {
			Coordinate temp = Coordinate(c.row + toDelete.row, c.col + toDelete.col, c.depth + toDelete.depth);
			// Checks that coordinate "c+toDelete" is in board's range, that it doesn't contain my ship
			// and wasn't hit before.
			// note: checkPosition() also checks that coordinate c isn't "hit_wrong",
			//		 but if it's already "hit_wrong" we won't need to change it again to "hit_wrong".. 
			if (checkPosition(temp)){
				(*board)[c.depth+toDelete.depth][c.row + toDelete.row][c.col + toDelete.col] = HIT_WRONG;
				//Note: if a ship sank - possibleMoves will be emptied later, so no need to call deleteFromPossibleMoves(temp);
			}
		}
	}
	// Updates board's positions - all the positions that can't contain a ship by the rules of the game
	for (Coordinate toDelete : this->placesToDelete) {
		Coordinate temp = Coordinate(c.row + toDelete.row, c.col + toDelete.col, c.depth + toDelete.depth);
		if (checkPosition(temp)) {
			(*board)[c.depth + toDelete.depth][c.row + toDelete.row][c.col + toDelete.col] = HIT_WRONG;
			deleteFromPossibleMoves(temp); //This line is here in case this function is used after the enemy hit himself
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
	this->board = std::move(BoardCreator::createBoard(numRows, numCols, numDepth)); //Cretes an empty board of size:[depth][rows][cols]
	
	//updates *our* board to contain all information from "board"
	for(int dep = 0; dep < numDepth; ++dep)
	{
		for (int row = 0; row < numRows; ++row)
		{
			for (int col = 0; col < numCols; ++col)
			{
				(*(this->board))[dep][row][col] = board.charAt(Coordinate(row + 1, col + 1, dep + 1)); //Adds 1's because indexes start at 1
			}
		}
	}
}

IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllSmartAlgo();
	return ptrToAlg;
}