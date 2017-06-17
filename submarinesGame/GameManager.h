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

typedef std::vector<std::shared_ptr<GameInfo>> vecOfGameInfoPtrs;
typedef std::vector<std::shared_ptr<PlayerInfo>> vecOfPlayerInfoPtrs;
static const int NOT_INIT = -1;

class GameManager
{
	class GameBasicData
	{
	protected:
		std::pair<std::string, std::unique_ptr<IBattleshipGameAlgo>> dllA;
		std::pair<std::string, std::unique_ptr<IBattleshipGameAlgo>> dllB;
		std::shared_ptr<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> >> board;

		GameBasicData(std::pair<std::string, GetAlgoFuncType>& algoA, std::pair<std::string, GetAlgoFuncType>& algoB,
			std::pair< std::shared_ptr<Board>,std::shared_ptr<Board>> board) :
			board(std::make_shared<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board>>>(board))
		{
			std::string dllNameA(algoA.first);
			dllA = std::make_pair<std::string , std::unique_ptr<IBattleshipGameAlgo>>(std::move(dllNameA), std::make_unique<IBattleshipGameAlgo>(*(*algoA.second)()));
			std::string dllNameB(algoB.first);
			dllB = std::make_pair<std::string, std::unique_ptr<IBattleshipGameAlgo>>(std::move(dllNameB), std::make_unique<IBattleshipGameAlgo>(*(*algoB.second)()));
		}
		~GameBasicData() = default;
	};
	
	int numberThreads;
	int gameNumber;
	int roundNumber;
	//A vector to contain all game combinations:
	std::vector<std::unique_ptr<GameBasicData>> allGamesData; 
	//A vector that will contain all game results:
	vecOfGameInfoPtrs allGamesResults; 
	//A vector that will contain all players 
	vecOfPlayerInfoPtrs allPlayersInfo; 
	//A vector of pairs: <dll name, (not initiaized)dll>
	std::vector<std::unique_ptr<std::pair<std::string,GetAlgoFuncType>>> dlls; 
	//A vector of ptrs to all pairs of valid Boards: <Board to send to PLAYER_A, Board to sent to PLAYER_B>    
	std::vector<std::unique_ptr<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> >>> boards;

	/*
	 * Returns true iff playerA's win rate > playerB's win rate
	 */
	static bool sortPlayers(std::pair<std::string, std::vector<int>> playerA, std::pair<std::string, std::vector<int>> playerB);

public:
	
	void runGameThread(std::shared_ptr<GameInfo> gameInfo);
	
	/*
	 * If there are more games to play - updates pointer "game" to point at a new game,
	 * Otherwise - changes GameInfo's "moreGamesLeft" to false, to indicate no more games left.
	 */
	void getGame(std::shared_ptr<GameInfo> game);

	void addNewGameInfo(std::unique_ptr<GameInfo> game);
	
	/*
	 * Sets this->numberThreads to be "numberThreads"
	 */
	void setNumberThreads(int numberThreads);
	
	/*
	 * TODO:: fix edge cases
	 */
	void startGames();
	
	/*
	 * Creates a new game manager: 
	 *	Sets roundNumber to be 1.
	 */
	GameManager(std::shared_ptr<std::vector<std::string>> dllsFiles, std::shared_ptr<std::vector<std::string>> boardFiles);
	
	/*
	 * Loads all DLLs into "dlls" vector, creates all players (dlls) info in "allPlayersInfo"
	 * Note: GameManager has to make sure it has enough dlls
	 */
	void loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles);

	/*
	* Loads all boards into "boards" vector
	*/
	void loadAllBoards(std::shared_ptr<std::vector<std::string>> boardFiles);

	/*
	 * Creates all combinations of games (each couple of players play 2 games on the same board,
	 *										one with first player as playerA,
	 *										second with first player as playerB)
	 */
	void divideToGames();

	void printRound();

	
};