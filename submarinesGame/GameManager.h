#pragma once

#include "IBattleshipGameAlgo.h"
#include "GameInfo.h"
#include "Game.h"
#include <thread>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <mutex>
#include "PlayerInfo.h"
#include <iostream>
#include <iomanip>

typedef std::vector<std::vector<std::vector<char>>> boardType;



class GameManager
{
	int numberThreads;
	int gameNumber;
	int roundNumber;
	std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData;
	std::shared_ptr<std::vector<std::shared_ptr<PlayerInfo>>> allPlayersInfo;

public:
	void runGameThread(std::shared_ptr<GameInfo> gameInfo);
	void getGame(std::shared_ptr<GameInfo> game);
	void setNumberThreads(int numberThreads);
	void startGames();
	
	GameManager(std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData, std::shared_ptr<std::vector<std::shared_ptr<PlayerInfo>>> allPlayersInfo);

	static void loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles, std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls, std::shared_ptr<std::vector<std::shared_ptr<PlayerInfo>>> allPlayersInfo);

	static void divideToGames(std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls, std::shared_ptr<std::vector<std::shared_ptr<boardType>>> boards, std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGames);

	void printRound();

	
};