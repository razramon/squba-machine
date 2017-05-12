#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>
#include <iostream>
#include "Sizes.h"
#include "BoardCreator.h"

// define function of the type we expect
typedef IBattleshipGameAlgo*(*GetAlgoFuncType)();


class Game
{
	mutable std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips;
	int playerPlaying;
	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;
	std::pair<int, int> points;
	std::pair<int, int> shipSunk;
	std::vector<HINSTANCE, HINSTANCE> dlls;
//	bool initPlayers(int playerNum, std::pair<char**, char**> boards);
public:
	Game(char** board, std::vector<std::string>& filesFound);
	~Game();

	int isHit(int row, int col, char& letter) const;

	void game();

	int checkWin() const;


};

