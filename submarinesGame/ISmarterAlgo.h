#pragma once
#include "IBattleshipGameAlgo.h"
#include "BoardCreator.h"

class ISmarterAlgo : public IBattleshipGameAlgo
{
private:
	char** myBoard;
	int rows;
	int cols;
	int playerNum;

public:
	/* Called once (by the Game) to notify (this) player on his board
	* by: http://moodle.tau.ac.il/mod/forum/discuss.php?d=54032,
	* The convention is that a space - ' ', represents "sea",
	* And Letters Represent Ships.
	*/
	inline void setBoard(int player, const char** board, int numRows, int numCols) override
	{
		if (board == nullptr) return;
		playerNum = player;
		rows = numRows;
		cols = numCols;
		myBoard = BoardCreator::copyBoard(board, rows, cols);
	}
};
