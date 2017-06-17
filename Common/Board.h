#pragma once

#include "../submarinesGame/BoardCreator.h"

class Board : public BoardData
{

	std::unique_ptr<boardType> gameBoard;
	int _playerNumber;
	Board& operator=(const Board& b) = delete; //No assignment operator!

public:
	/*
	* Will get a board with Ships of playerA AND playerB,
	* But will return (when using charAt()) only letters that represent ships of "_playerNumber"
	* *****NOTE: uses std::move on board!!******
	*/
	explicit Board(int rows, int cols,int depth, std::unique_ptr<boardType> board, int playerNumber);
	
	/*
	 * Copy constructor:
	 *  returns a pointer to a *copy* of the board that is pointed by "ptrToboardToCopy"
	 */
	explicit Board(const std::unique_ptr<Board>& ptrToboardToCopy); 
	
	~Board();

	char charAt(Coordinate c) const override;

	void setPlayerNumber(int playerNumber);
};

