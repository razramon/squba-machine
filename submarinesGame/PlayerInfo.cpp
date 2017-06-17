#include "PlayerInfo.h"


PlayerInfo::PlayerInfo(std::string playerName) :
	playerName(playerName), rounds()
{
}

void PlayerInfo::addNewGame(std::unique_ptr<GameInfo> game)
{
	int enemyPoints = game->getOtherPlayerScore(playerName);
	if (enemyPoints==-1) //never supposed to get here, just to self test! gets's inside only if current player wasn't a player in "game"
	{
		throw Exception("Error: tried to insert a game with wrong player name");
	}

	Round newRound;

	//when no player wins - numberWin/numberLose doesn't change, according to:
	//http://moodle.tau.ac.il/mod/forum/discuss.php?d=63620#p93614
	if (game->getPlayerWonName() != nullptr) {
		newRound.numberWin = (*(game->getPlayerWonName()) == playerName) ? 1 : 0;
		newRound.numberLose = 1 - newRound.numberWin;
	} else
	{
		newRound.numberWin = 0;
		newRound.numberLose = 0;
	}
	newRound.pointsFor = game->getPlayerScore(playerName);
	newRound.pointsAgainst = game->getOtherPlayerScore(playerName);

	if (rounds.size() > 0)
	{
		Round prevRound = rounds[rounds.size() - 1];
		newRound.numberWin += prevRound.numberWin;
		newRound.numberLose += prevRound.numberLose;
		newRound.pointsFor += prevRound.pointsFor;
		newRound.pointsAgainst += prevRound.pointsAgainst;
	}

	this->rounds.push_back(newRound);
}

bool PlayerInfo::hasGameInRound(int roundNumber) const
{
	return (rounds.size() >= roundNumber);
}

std::string PlayerInfo::getPlayerName() const{

	return playerName;
}

std::vector<int> PlayerInfo::getRoundPoints(int roundNumber)
{
	std::vector<int> result={}; //initialize an empty vector
	if ( 0 <= roundNumber && roundNumber<rounds.size() )
	{
		Round r = rounds[roundNumber - 1];
		result.push_back(r.numberWin);
		result.push_back(r.numberLose);
		result.push_back(r.pointsFor);
		result.push_back(r.pointsAgainst);
	}

	return result;
}
