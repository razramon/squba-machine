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
#include <atomic>

typedef std::vector<std::shared_ptr<GameInfo>> vecOfGameInfoPtrs;
typedef std::vector<std::shared_ptr<PlayerInfo>> vecOfPlayerInfoPtrs;
static const int NOT_INIT = -1;

class GameManager
{
	class GameBasicData
	{
		friend class GameManager;
	protected:
		std::pair<std::string, std::unique_ptr<IBattleshipGameAlgo>> dllA;
		std::pair<std::string, std::unique_ptr<IBattleshipGameAlgo>> dllB;
		std::shared_ptr<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> >> board;
		int indexOfGameShips;
	public:
		GameBasicData(std::pair<std::string, GetAlgoFuncType>& algoA, std::pair<std::string, GetAlgoFuncType>& algoB,
			std::pair< std::shared_ptr<Board>,std::shared_ptr<Board>> board, int indexOfShips) :
			board(std::make_shared<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board>>>(board)), indexOfGameShips(indexOfShips)
		{
			std::string dllNameA(algoA.first);
			dllA = std::make_pair<std::string , std::unique_ptr<IBattleshipGameAlgo>>(std::move(dllNameA), std::make_unique<IBattleshipGameAlgo>(*(*algoA.second)()));
			std::string dllNameB(algoB.first);
			dllB = std::make_pair<std::string, std::unique_ptr<IBattleshipGameAlgo>>(std::move(dllNameB), std::make_unique<IBattleshipGameAlgo>(*(*algoB.second)()));
		}
		~GameBasicData() = default;
	};
	
	int numberThreads;
	std::atomic<int> gameNumber;
	std::atomic<int> roundNumber;

	//A vector to contain all game combinations:
	std::vector<std::unique_ptr<GameBasicData>> allGamesData; 
	
	////A vector that will contain all game results:
	//vecOfGameInfoPtrs allGamesResults; 
	
	//A vector that will contain all players 
	vecOfPlayerInfoPtrs allPlayersInfo;
	
	//A vector of pairs: <dll name, (not initiaized)dll>
	std::vector<std::unique_ptr<std::pair<std::string,GetAlgoFuncType>>> dlls; 
	
	//A vector of ptrs to all pairs of valid Boards: <Board to send to PLAYER_A, Board to sent to PLAYER_B>    
	std::vector<std::unique_ptr<std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> >>> boards;
	
	//for each board (represented as a pair), boardsShips[i] = ships of boards[i]
	std::vector<std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector>>> boardsShips;

	/*
	 * Returns true iff playerA's win rate > playerB's win rate
	 */
	static bool sortPlayers(std::pair<std::string, std::vector<int>> playerA, std::pair<std::string, std::vector<int>> playerB);

public:
	

	void runGameThread();
	
	/*
	 * If there are more games to play - updates pointer "gameBasicData" to point at next GameBasicData (from allGamesData),
	 * Otherwise - gameBasicData is updated to nullptr, to indicate no more games left.
	 */
	void getGame(std::shared_ptr<GameBasicData> gameBasicData);
	
	/*
	 * Sets this->numberThreads to be "numberThreads"
	 */
	void setNumberThreads(int numberThreads);
	
	/*
	 * Creates relevant number of threads and start the tournament
	 */
	void startGames();
	
	/*
	 * Creates a new game manager: 
	 *	Sets roundNumber to be 1.
	 */
	GameManager(std::shared_ptr<std::vector<std::string>> dllsFiles,
			std::shared_ptr<std::vector<std::string>> boardFiles, int numOfThreads);
	
	/*
	 * Loads all DLLs into "dlls" vector, creates all players (dlls) info in "allPlayersInfo"
	 * Note: GameManager has to make sure it has enough dlls
	 */
	void loadAllDlls(std::shared_ptr<std::vector<std::string>> dllsFiles);

	/*
	* Loads all boards into "boards" vector, and all ships to boardsShips according to the relevant board.
	*/
	void loadAllBoards(std::shared_ptr<std::vector<std::string>> boardFiles);

	/*
	 * Creates all combinations of games (each couple of players play 2 games on the same board,
	 *										one with first player as playerA,
	 *										second with first player as playerB)
	 */
	void divideToGames();

	void printRound();

	void addNewGameInfo(std::unique_ptr<GameInfo>& game);
};