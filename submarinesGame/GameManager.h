#pragma once

#include "IBattleshipGameAlgo.h"
#include "GameInfo.h"
#include "Game.h"
#include <thread>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <mutex>
#include "PlayerInfo.h"
#include <iostream>
#include <iomanip>

typedef std::shared_ptr<std::vector<std::shared_ptr<GameInfo>>> ptrToVecOfGameInfoPtrs;
typedef std::shared_ptr<std::vector<std::shared_ptr<PlayerInfo>>> ptrToVecOfPlayerInfoPtrs;
static const int NOT_INIT = -1;

class GameManager
{
	class GameBasicData
	{
	protected:
		std::unique_ptr<IBattleshipGameAlgo> algoA;
		std::unique_ptr<IBattleshipGameAlgo> algoB;
		std::unique_ptr<Board> board;
	};
	
	int numberThreads;
	int gameNumber;
	int roundNumber;
	std::vector<GameBasicData> allGamesData; //A vector to contain all game combinations
	ptrToVecOfGameInfoPtrs allGamesResults; //A vector that will contain all game results
	ptrToVecOfPlayerInfoPtrs allPlayersInfo;

	/*
	 * Returns true iff playerA's win rate > playerB's win rate
	 */
	static bool sortPlayers(std::pair<std::string, std::vector<int>> playerA, std::pair<std::string, std::vector<int>> playerB);

public:
	
	void runGameThread(std::shared_ptr<GameInfo> gameInfo);
	
	/*
	 * If there are more games to play - updates pointer "game" to point at a new game,
	 * Otherwise - changes game's playerWon to -1, to indicate no more games left.
	 */
	void getGame(std::shared_ptr<GameInfo> game);

	void addNewGameInfo(std::unique_ptr<GameInfo> game);
	
	/*
	 * Sets this->numberThreads to be "numberThreads"
	 */
	void setNumberThreads(int numberThreads);
	
	void startGames();
	
	/*
	 * Creates a new game manager: 
	 *	Gets - allGamesData, allPlayersInfo
	 *	Sets roundNumber to be 1.
	 */
	GameManager(ptrToVecOfGameInfoPtrs allGamesData, ptrToVecOfPlayerInfoPtrs allPlayersInfo);
	
	/*
	 * Loads all DLLs into "dlls" vector, and all players (dlls) info into "allPlayersInfo"
	 */
	static void loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles, std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls,  ptrToVecOfPlayerInfoPtrs allPlayersInfo);

	/*
	 * Creates all combinations of games (each couple of players play 2 games on the same board,
	 *										one with first player as playerA,
	 *										second with first player as playerB)
	 */
	static void divideToGames(std::shared_ptr<std::vector<std::unique_ptr<IBattleshipGameAlgo>>> dlls, std::shared_ptr<std::vector<Board>> boards);

	void printRound();

	
};