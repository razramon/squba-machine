#include "GameInfo.h"


int GameInfo::getPlayerScore(int player)
{
	int score = -1;

	score = player == this->firstPlayer_number_score.first ? this->firstPlayer_number_score.second : 0;
	
	score = player == this->secondPlayer_number_score.first ? this->secondPlayer_number_score.second : score;

	return score;
}

int GameInfo::getPlayerWon()
{
	return this->playerWon;
}

// Getting the players played- for future use
std::pair<int, int> GameInfo::getPlayersPlayed()
{
	return std::make_pair(this->firstPlayer_number_score.first, this->secondPlayer_number_score.first);
}

// Setting the playerInfo for a single player
void GameInfo::setAll(int player, int score)
{
	if (this->firstPlayer_number_score.first == NULL) {

		this->firstPlayer_number_score = std::make_pair(player, score);
	}
	else if (this->secondPlayer_number_score.first == NULL) {

		this->secondPlayer_number_score = std::make_pair(player, score);
	}
}

void GameInfo::setPlayersScore(int scoreA, int scoreB)
{
	this->firstPlayer_number_score.second = scoreA;
	this->secondPlayer_number_score.second = scoreB;
}

void GameInfo::setPlayerWon(int won)
{
	this->playerWon = won;
}

GameInfo::GameInfo(std::unique_ptr<IBattleshipGameAlgo> firstDll, std::unique_ptr<IBattleshipGameAlgo> secondDll, std::shared_ptr<boardType> board) {

	this->playerA = std::move(firstDll);
	this->playerB = std::move(secondDll);
	this->board = board;
}

std::pair<std::unique_ptr<IBattleshipGameAlgo>, std::unique_ptr<IBattleshipGameAlgo>> GameInfo::getPlayersAlgos() {

	return std::make_pair(std::move(this->playerA), std::move(this->playerB));
}

std::shared_ptr<boardType> GameInfo::getBoard() {
	
	return this->board;
}


