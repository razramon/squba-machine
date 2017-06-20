#pragma once
#include <vector>
#include <thread>
#include <iostream>
#include "Sizes.h"
#include "Ship.h"
#include "IBattleshipGameAlgo.h"
#include "GameInfo.h"
#include "Exception.h"
#include <mutex>

class PlayerInfo;

/*
 * As an item in a vector of Rounds, 
 * Round Will represents data until the current round of the tournament (all games)
 */
struct Round {
	int numberWin;
	int numberLose;
	int pointsFor; //points current player have earned during this round + earlier rounds
	int pointsAgainst; //points that were earned against this player during this round + earlier rounds
};

class PlayerInfo {
	std::string playerName; //playerName is the dll's name..
	std::vector<Round> rounds; //contains all rounds information of this player

public:

	explicit PlayerInfo(std::string playerName);
	~PlayerInfo();
	
	/*
	 * Adds a round which fits the results of "game" into the vector "rounds".
	 */
	void addNewGame(std::unique_ptr<GameInfo>& game);
	
	/*
	 * Returns true iff round number "roundNumber" is in "rounds" (the first round is numbered as 0 round)
	 */
	bool hasGameInRound(int roundNumber) const;
	
	/*
	 * Returns a vector of ints, representing round number i (index start at 0)
	 *	 vector[0] - number of winnings until round number i,
	 *	 vector[1] - number of loses until round number i,
	 *	 vector[2] - points earned by this player until (including) round i,
	 *	 vector[3] - points earned against this player until (including) round i
	 * NOTE: if no round is at index i / if i<0, returns an empty vector
	 */
	std::vector<int> getRoundPoints(int i);
	
	std::string getPlayerName() const;
};