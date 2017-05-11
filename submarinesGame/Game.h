#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>
#include <iostream>
#include "Sizes.h"
#include "BoardCreator.h"

class Game
{
	mutable std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips;
	int playerPlaying;
	//char ** boardOfPlayerA;
	//char ** boardOfPlayerB;
	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;
	std::pair<int, int> points;
	std::pair<int, int> shipSunk;
	bool initPlayers(std::string& allErrors, int playerNum);
public:
	Game(char** board, std::vector<std::string>& filesFound);
	~Game();

	int isHit(int row, int col, char& letter) const;

	void game();

	int checkWin() const;


};

