#include "GameBasicData.h"

GameBasicData::GameBasicData(std::pair<std::string, GetAlgoFuncType>& algoA, std::pair<std::string, GetAlgoFuncType>& algoB,
			std::pair< std::shared_ptr<Board>,std::shared_ptr<Board>> board, int indexOfShips) :
			board(std::make_shared<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board>>>(board)), indexOfGameShips(indexOfShips)
{
	std::string dllNameA(algoA.first);
	std::unique_ptr<IBattleshipGameAlgo> playerA_algo((algoA.second()));
	dllA = std::make_pair<std::string , std::unique_ptr<IBattleshipGameAlgo>>(std::move(dllNameA),std::move(playerA_algo));
	std::string dllNameB(algoB.first);
	std::unique_ptr<IBattleshipGameAlgo> playerB_algo((algoB.second()));
	dllB = std::make_pair<std::string, std::unique_ptr<IBattleshipGameAlgo>>(std::move(dllNameB), std::move(playerB_algo));
}

GameBasicData::~GameBasicData()
{
}
