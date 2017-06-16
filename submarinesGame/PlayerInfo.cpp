#include "PlayerInfo.h"

PlayerInfo::PlayerInfo(int playerNumber, std::string playerName)
{
	this->playerName = playerName;
	this->playerNumber = playerNumber;
}

void PlayerInfo::addNewGame(std::shared_ptr<GameInfo> game)
{
	int enemy = game->getPlayersPlayed().first == this->playerNumber ? game->getPlayersPlayed().second : game->getPlayersPlayed().first;

	Round newRound;
	newRound.numberWin = game->getPlayerWon() == this->playerNumber ? 1 : 0;
	newRound.numberLose = 1 - newRound.numberWin;
	newRound.pointsFor = game->getPlayerScore(this->playerNumber);
	newRound.pointsAgainst = game->getPlayerScore(enemy);

	for (Round curRound : this->Rounds) {

		newRound.numberWin += curRound.numberWin;
		newRound.numberLose += curRound.numberLose;
		newRound.pointsFor += curRound.pointsFor;
		newRound.pointsAgainst += curRound.pointsAgainst;
	}

	this->Rounds.push_back(newRound);
}

bool PlayerInfo::hasGameInRound(int roundNumber)
{
	return this->Rounds.size() >= roundNumber;
}

std::string PlayerInfo::getPlayerName() {

	return this->playerName;
}

std::vector<int> PlayerInfo::getRoundPoints(int roundNumber)
{
	std::vector<int> result;
	Round r = this->Rounds[roundNumber - 1];

	result.push_back(r.numberWin);
	result.push_back(r.numberLose);
	result.push_back(r.pointsFor);
	result.push_back(r.pointsAgainst);

	return result;
}
