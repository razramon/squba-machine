#pragma once
#include "Utilities.h"
#include "IBattleshipGameAlgo.h"
#include "Board.h"
#include "Game.h"

class GameBasicData
{
	friend class GameManager;

	std::pair<std::string, std::unique_ptr<IBattleshipGameAlgo>> dllA;
	std::pair<std::string, std::unique_ptr<IBattleshipGameAlgo>> dllB;
	std::shared_ptr<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> >> board;
	int indexOfGameShips;

public:

	GameBasicData(std::pair<std::string, GetAlgoFuncType>& algoA, std::pair<std::string, GetAlgoFuncType>& algoB,
		std::pair< std::shared_ptr<Board>, std::shared_ptr<Board>> board, int indexOfShips);
	~GameBasicData();
};

