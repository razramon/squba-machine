#pragma once
#include <vector>
#include <thread>
#include <iostream>
#include "Sizes.h"
#include "Ship.h"
#include "IBattleshipGameAlgo.h"
#include "GameInfo.h"
class PlayerInfo;

struct Round {
	
	int numberWin;
	int numberLose;
	int pointsFor;
	int pointsAgainst;
};

class PlayerInfo {
	int playerNumber;
	std::string playerName;
	std::vector<Round> Rounds;

public:

	PlayerInfo(int playerNumber, std::string playerName);
	void addNewGame(std::shared_ptr<GameInfo> game);
	bool hasGameInRound(int Round);
	std::vector<int> getRoundPoints(int Round);
	std::string getPlayerName();
};