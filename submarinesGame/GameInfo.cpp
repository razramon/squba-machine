#include "GameInfo.h"


int GameInfo::getPlayerScore(int player) const
{
	int score = -1;

	score = player == this->firstPlayer_number_score.first ? this->firstPlayer_number_score.second : 0;
	
	score = player == this->secondPlayer_number_score.first ? this->secondPlayer_number_score.second : 0;

	return score;
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


