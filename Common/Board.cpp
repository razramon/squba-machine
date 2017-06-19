#include "Board.h"

Board::Board(int rows, int cols, int depth, std::shared_ptr<boardType>& board, int playerNum):
	gameBoard(std::move(board)), _playerNumber(playerNum)
{
	_rows = rows;
	_cols = cols;
	_depth = depth;
}

Board::Board(const std::unique_ptr<Board>& ptrToboardToCopy):
	gameBoard(BoardCreator::copyBoard(((*ptrToboardToCopy).gameBoard), ptrToboardToCopy->_rows,
	ptrToboardToCopy->_cols, ptrToboardToCopy->_depth)), _playerNumber(ptrToboardToCopy->_playerNumber)
{
	_rows = ptrToboardToCopy->_rows;
	_cols = ptrToboardToCopy->_cols;
	_depth = ptrToboardToCopy->_depth;	
}

Board::~Board()
{
}

//Gets coordinated indexed from 1!!
char Board::charAt(Coordinate c) const
{
	if (1<=c.row && c.row <=_rows &&
		1<=c.col && c.col <=_cols &&
		1<=c.depth && c.depth<=_depth) //checks its a valid coordinate
	{
		char letter = (*gameBoard)[c.depth-1][c.row-1][c.col-1];
		if (Ship::isShip(letter))
		{
			//player B's letter (small letters), player A's letters are big letters
			if ((islower(letter) && _playerNumber == PLAYER_B) || ((!islower(letter)) && _playerNumber == PLAYER_A)) {
				return letter;
			}
		}
	}

	return blank_space;
}

void Board::setPlayerNumber(int playerNumber)
{
	_playerNumber = playerNumber;
}

