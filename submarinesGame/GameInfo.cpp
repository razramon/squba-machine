#include "GameInfo.h"


int GameInfo::getPlayerScore(int player) const
{
	int score = -1;

	score = player == this->firstPlayer_number_score.first ? this->firstPlayer_number_score.second : 0;
	
	score = player == this->secondPlayer_number_score.first ? this->secondPlayer_number_score.second : 0;

	return score;
}

int GameInfo::getPlayerScore(std::string nameOfPlayer) const
{
	if(dllNames.first==nameOfPlayer)
	{
		return playersScore.first;
	}
	else if (dllNames.second==nameOfPlayer)
	{
		return playersScore.second;
	}
	return -1;
}

int GameInfo::getOtherPlayerScore(std::string nameOfCurrentPlayer) const
{
	if (dllNames.first == nameOfCurrentPlayer) {
		return playersScore.second;
	}
	else if (dllNames.second == nameOfCurrentPlayer)
	{
		return playersScore.first;
	}
	return -1;
}

int GameInfo::getNumberOfPlayer(std::string nameOfPlayer) const
{
	if (dllNames.first == nameOfPlayer)
	{
		return PLAYER_A;
	}
	else if (dllNames.second == nameOfPlayer)
	{
		return PLAYER_B;
	}
	return -1;
}

int GameInfo::getPlayerWon() const
{
	return playerWon;
}

std::unique_ptr<std::string> GameInfo::getPlayerWonName() const
{
	if (playerWon == -1) return nullptr;
	return std::make_unique<std::string>((playerWon==PLAYER_A)? dllNames.first: dllNames.second);
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


