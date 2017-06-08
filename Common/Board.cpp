#include "Board.h"

Board::Board(int rows, int cols, int depth, std::shared_ptr<boardType> board)
{
	_rows = rows;
	_cols = cols;
	_depth = depth;
	gameBoard = board;
}

Board::~Board()
{
}

char Board::charAt(Coordinate c) const
{
	return (*gameBoard)[c.depth][c.row][c.col];
}

