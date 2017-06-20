#pragma once
#include "Utilities.h"
#include "IBattleshipGameAlgo.h"
#include "../Common/Board.h"
#include "Game.h"

class GameBasicData
{
	friend class GameManager;

	std::pair<std::string, std::shared_ptr<GetAlgoFuncType>> dllA;
	std::pair<std::string, std::shared_ptr<GetAlgoFuncType>> dllB;
	std::shared_ptr<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> >> board;
	int indexOfGameShips;

public:

	GameBasicData(std::pair<std::string, GetAlgoFuncType> algoA, std::pair<std::string, GetAlgoFuncType> algoB,
		std::pair< std::shared_ptr<Board>, std::shared_ptr<Board>> board, int indexOfShips);
	~GameBasicData();
};

