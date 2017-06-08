#pragma once

#include "../submarinesGame/BoardCreator.h"

class Board : public BoardData
{
private:
	std::shared_ptr<boardType> gameBoard;

public:
	/*
	 * Will get a board with Ships of (only) playerA XOR playerB.  
	 */
	explicit Board(int rows, int cols,int depth, std::shared_ptr<boardType> board);
	~Board();
	char charAt(Coordinate c) const override;
};

