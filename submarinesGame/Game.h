#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>
#include <iostream>
#include "Sizes.h"
#include "BoardCreator.h"
#include "../Common/Board.h"
#include "GameInfo.h"


// define function of the type we expect
typedef IBattleshipGameAlgo*(*GetAlgoFuncType)();

class Game
{
	mutable std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector>> playersShips;
	int playerPlaying;
	std::shared_ptr<IBattleshipGameAlgo> playerA;
	std::shared_ptr<IBattleshipGameAlgo> playerB;
	std::pair<int, int> points;
	std::pair<int, int> shipSunk;
	int numRows, numCols, numDepth;
	std::pair <std::string, std::string> dllNames;
	std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> > board; // <board for playerA, board for playerB>
	

	/*
	 * Returns TRUE if current playerPlaying has an attack, if he does - updates attackOfPlayer
	 * If he doesn't - updates playerPlaying to other player and if it has an attack updates attackOfPlayer
	 * If BOTH players doesn't have an attack - returns FALSE
	 */
	bool playersHaveAttack(Coordinate& attackOfPlayer);
	/*
	 * Notifies both players on last result of the attack
	 * Function should be called before updating "playerPlaying"! 
	 */
	void notifyPlayers(Coordinate& currAttack, AttackResult& result) const;
	/*
	 *Returns 'A' if player A won, 'B' if player B won
	 */
	static char getLetterByNumber(int number);
	/*
	 * Returns true if current attack simbolize player has no more attacks(-1,-1,-1)
	 * (Attack should be checked for validation before using this function!)
	 */
	static bool noAttacksLeft(Coordinate& attackOfPlayer);
	/*
	* Returns true if current attack is valid, (-1,-1,-1) is valid attack!!
	*/
	bool isValAttack(Coordinate& attackOfPlayer) const;

	/*
	 *Returns the ship which in position board[depth][row][col]
	 */
	std::shared_ptr<Ship> getShipAtPosition(int row, int col, int depth) const;

	//	bool initPlayers(int playerNum, std::pair<char**, char**> boards);

	static void initializePlayer(IBattleshipGameAlgo* algo, int p_Number, BoardData& b);

public:
	Game(std::string dllAName, std::string dllBName, std::shared_ptr<GetAlgoFuncType> playerA,
		std::shared_ptr<GetAlgoFuncType> playerB, std::pair< std::shared_ptr<Board>, std::shared_ptr<Board> > board,
		std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector>> playersShips,
		int numRows, int numCols, int numDepth);
	~Game();

	int isHit(int row, int col, int depth, char& letter) const;

	std::unique_ptr<GameInfo> game();

	int checkWin() const;
};
