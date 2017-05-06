#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>
#include "Player.h"
#include <iostream>
#include "Sizes.h"
#include "BoardCreator.h"

class Game
{
	mutable std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips;
	//char ** boardOfPlayerA;
	//char ** boardOfPlayerB;
	Player* playerA;
	Player* playerB;
	int playerPlaying;
	std::pair<int, int> points;
	std::pair<int, int> shipSunk;

public:
	Game(char** board, std::string& fullPathToAttackFileA, std::string& fullPathToAttackFileB);
	~Game();

	int isHit(int row, int col, char& letter) const;

	void game();

	int checkWin() const;


};

