#pragma once

#include "IBattleshipGameAlgo.h"
#include "GameInfo.h"
#include "Game.h"
#include <thread>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <memory>
#include <mutex>


typedef std::vector<std::vector<std::vector<char>>> boardType;



class GameManager
{
	int gameNumber;
	std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData;

public:
	void runGameThread(std::shared_ptr<GameInfo> gameInfo);
	void getGame(std::shared_ptr<GameInfo> game);
	void setNumberGame(int gameNumber);
	void startGames();
	GameManager(std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData);
};