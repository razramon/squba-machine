#pragma once

#include <vector>
#include <thread>
#include <iostream>
#include "Sizes.h"


// define function of the type we expect
typedef std::vector<std::vector<std::vector<char>>> boardType;

class GameInfo
{
	int playerWon; //0 if playerA won, 1 if playerB, **-1** if no player won!!
	std::pair<std::string, std::string> dllNames; //.first is the dll of playerA, .second is playerB's dll
	std::pair<int, int> playersScore; ////.first is playerA's score, .second is playerB's score

public:

	/*
	 * Gets score to contain the relevant player's score
	 */
	int getPlayerScore(int player) const;
	
	/*
	 * Returns the number (0/1) of player who won the game 
	 */
	int getPlayerWon() const;

	/*
	 * Returns the DLLs names
	 */
	std::pair<std::string, std::string> getPlayerNames() const;

	GameInfo(int playerWon,const std::pair<std::string, std::string>& dllNames, const std::pair<int, int>& playersScore);
	~GameInfo();
};