#include "GameInfo.h"


int GameInfo::getPlayerScore(int player) const
{
	if (player == PLAYER_A)
		return playersScore.first;
	return playersScore.second;
}

int GameInfo::getPlayerWon() const
{
	return playerWon;
}

// Getting the players played- for future use
std::pair<std::string, std::string> GameInfo::getPlayerNames() const
{
	return this->dllNames;
}

GameInfo::GameInfo(int playerWon, const std::pair<std::string, std::string>& dllNames, const std::pair<int, int>& playersScore):
					playerWon(playerWon), dllNames(std::make_pair(dllNames.first,dllNames.second)),
					playersScore(std::make_pair(playersScore.first,playersScore.second))
{
}

GameInfo::~GameInfo()
{
}


