#include "GameBasicData.h"

GameBasicData::GameBasicData(std::pair<std::string, GetAlgoFuncType> algoA, std::pair<std::string, GetAlgoFuncType> algoB,
			std::pair< std::shared_ptr<Board>,std::shared_ptr<Board>> board, int indexOfShips) :
			board(std::make_shared<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board>>>(board)), indexOfGameShips(indexOfShips)
{
	std::string dllNameA(algoA.first);
	dllA = std::make_pair(std::move(dllNameA), std::make_shared<GetAlgoFuncType>(algoA.second));
	std::string dllNameB(algoB.first);
	dllB = std::make_pair(std::move(dllNameB), std::make_shared<GetAlgoFuncType>(algoB.second));
}

GameBasicData::~GameBasicData()
{
}
