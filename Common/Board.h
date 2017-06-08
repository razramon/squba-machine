#pragma once

#include "../submarinesGame/BoardCreator.h"

class Board : public BoardData
{
private:
	std::shared_ptr<boardType> gameBoard;
	int _playerNumber;
public:
	/*
	 * Will get a board with Ships of playerA AND playerB,
	 * But will return only letters that represent ships of "_playerNumber"  
	 */
	explicit Board(int rows, int cols,int depth, std::shared_ptr<boardType> board, int playerNumber);
	~Board();
	char charAt(Coordinate c) const override;
};

