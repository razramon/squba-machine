#pragma once

#include <vector>
#include <thread>
#include <iostream>
#include "Sizes.h"


// define function of the type we expect
typedef std::vector<std::vector<std::vector<char>>> boardType;

class GameInfo
{
	static bool moreGamesLeft; //default is true, will be updated to false when all games were played (by GameManager::getGame())
	int playerWon; //0 if playerA won, 1 if playerB, **-1** if no player won!!
	std::pair<std::string, std::string> dllNames; //.first is the dll (name) of playerA, .second is playerB's dll (name)
	std::pair<int, int> playersScore; //.first is playerA's score, .second is playerB's score


public:

	/*
	 * Returns the relevant player's score
	 */
	int getPlayerScore(int player) const;

	/*
	* Returns the relevant player's score,
	* Returns -1 if the name of the player doesn't fit dllNames.first nor dllNames.second
	*  (=doesn't fit both players' names)
	*/
	int getPlayerScore(std::string nameOfPlayer) const;

	/*
	 * Returns the score of player who's name ISN'T(!) nameOfCurrentPlayer.
	 * NOTE: If both player's names are different from nameOfCurrentPlayer, returns -1
	 */
	int getOtherPlayerScore(std::string nameOfCurrentPlayer) const;
	
	/*
	* Returns which player was nameOfPlayer (PLAYER_A, PLAYER_B, -1 if it wasn't a player of this game)
	*/
	int getNumberOfPlayer(std::string nameOfPlayer) const;

	/*
	 * Returns the number (0 for PLAYER_A/1 for PLAYER_B) of player who won the game 
	 */
	int getPlayerWon() const;

	/*
	* Returns: -if there was a winner: a pointer to a string containing the name of the winning player,
	*		   -nullptr if no player won 
	*/
	std::unique_ptr<std::string> getPlayerWonName() const;

	/*
	 * Returns the DLLs names
	 */
	std::pair<std::string, std::string> getPlayerNames() const;

	/*
	 * Returns value of moreGamesLeft
	 */
	static bool getMoreGamesLeft();

	/*
	 * Sets value of moreGamesLeft
	 */
	static void setMoreGamesLeft(bool val);

	GameInfo(int playerWon,const std::pair<std::string, std::string>& dllNames, const std::pair<int, int>& playersScore);
	~GameInfo();
};