#include "GameInfo.h"


int GameInfo::getPlayerScore(int player)
{
	int score = -1;

	score = player == this->firstPlayer_number_score.first ? this->firstPlayer_number_score.second : 0;
	
	score = player == this->secondPlayer_number_score.first ? this->secondPlayer_number_score.second : 0;

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

GameInfo::GameInfo(std::shared_ptr<IBattleshipGameAlgo> firstDll, std::shared_ptr<IBattleshipGameAlgo> secondDll, std::shared_ptr<boardType> board) {

	this->playerA = firstDll;
	this->playerB = secondDll;
	this->board = board;
}

std::pair<std::shared_ptr<IBattleshipGameAlgo>, std::shared_ptr<IBattleshipGameAlgo>> GameInfo::getPlayersAlgos() {

	return std::make_pair(this->playerA, this->playerB);
}

std::shared_ptr<boardType> GameInfo::getBoard() {
	
	return this->board;
}
void GameInfo::divideToGames(std::vector<int> dlls, std::vector<boardType> boards, std::vector<std::unique_ptr<GameInfo>> allGames) {

	for (int i = 0; i < dlls.size(); i++) {

		for (int j = i + 1; j < dlls.size(); j++) {

			for (int indexBoard = 0; indexBoard < boards.size(); indexBoard++) {

				std::unique_ptr<GameInfo> game = std::make_unique<GameInfo>(dlls[i], dlls[j], boards[indexBoard]);
				allGames.push_back(game);
				game = std::make_unique<GameInfo>(dlls[j], dlls[i], boards[indexBoard]);
				allGames.push_back(game);
			}
		}
	}
}