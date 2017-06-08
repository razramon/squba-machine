#include "Board.h"

Board::Board(int rows, int cols, int depth, std::shared_ptr<boardType> board, int playerNum)
{
	_rows = rows;
	_cols = cols;
	_depth = depth;
	gameBoard = board;
	_playerNumber = playerNum;
}

Board::~Board()
{
}

char Board::charAt(Coordinate c) const
{
	char letter = (*gameBoard)[c.depth][c.row][c.col];
	if (Ship::isShip(letter))
	{
		//player B's letter (small letters), player A's letters are big letters
		if ((islower(letter) && _playerNumber==PLAYER_B) || ((!islower(letter)) && _playerNumber==PLAYER_A )) {
			return letter;
		}
	}
	return ' ';
}

