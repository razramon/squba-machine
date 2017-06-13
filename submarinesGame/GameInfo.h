#pragma once

#include <vector>
#include <thread>
#include <iostream>
#include "Sizes.h"
#include "Ship.h"
#include "IBattleshipGameAlgo.h"

// define function of the type we expect
typedef IBattleshipGameAlgo*(*GetAlgoFuncType)();
typedef std::vector<std::vector<std::vector<char>>> boardType;

class GameInfo
{
	std::pair<int, int> firstPlayer_number_score;
	std::pair<int, int> secondPlayer_number_score;
	int playerWon;
	std::shared_ptr<boardType> board;
	std::vector<Ship> ships;
	std::shared_ptr<IBattleshipGameAlgo> playerA;
	std::shared_ptr<IBattleshipGameAlgo> playerB;

public:
	
	int getPlayerScore(int player);
	int getPlayerWon();
	std::pair<int, int> getPlayersPlayed();
	void setAll(int player, int score);
	void setPlayersScore(int scoreA, int scoreB);
	void setPlayerWon(int won);
	GameInfo(std::shared_ptr<IBattleshipGameAlgo> firstDll, std::shared_ptr<IBattleshipGameAlgo> secondDll, std::shared_ptr<boardType> board);
	std::pair<std::shared_ptr<IBattleshipGameAlgo>, std::shared_ptr<IBattleshipGameAlgo>> getPlayersAlgos();
	std::shared_ptr<boardType> getBoard();
	static void divideToGames(std::vector<int> dlls, std::vector<boardType> boards, std::vector<std::unique_ptr<GameInfo>> allGames);
	//static void loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles, std::shared_ptr<std::vector<std::shared_ptr<IBattleshipGameAlgo>>> dlls);
};