#pragma once

#include "IBattleshipGameAlgo.h"
#include "Ship.h"
#include <vector>
#include <iostream>
#include "Sizes.h"
#include "BoardCreator.h"

// define function of the type we expect
typedef IBattleshipGameAlgo*(*GetAlgoFuncType)();
typedef std::vector<std::vector<std::vector<char>>> boardType;

class Game
{
	mutable std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips;
	int playerPlaying;
	IBattleshipGameAlgo* playerA;
	IBattleshipGameAlgo* playerB;
	std::pair<int, int> points;
	std::pair<int, int> shipSunk;

	/*
	* Deletes all memory allocations of playerShips
	*/
	void deletePlayerShips() const;

	/*
	 * Returns TRUE if current playerPlaying has an attack, if he does - updates attackOfPlayer
	 * If he doesn't - updates playerPlaying to other player and if it has an attack updates attackOfPlayer
	 * If BOTH players doesn't have an attack - returns FALSE
	 */
	bool playersHaveAttack(std::pair<int, int>& attackOfPlayer);
	/*
	 * Notifies both players on last result of the attack
	 * Function should be called before updating "playerPlaying"! 
	 */
	void notifyPlayers(std::pair<int, int>& currAttack, AttackResult& result) const;
	/*
	 *Returns 'A' if player A won, 'B' if player B won
	 */
	static char getLetterByNumber(int number);
	/*
	 * Returns true if current attack simbolize player has no more attacks(-1,-1)
	 * (Attack should be checked for validation before using this function!)
	 */
	static bool noAttacksLeft(std::pair<int, int>& attackOfPlayer);
	/*
	* Returns true if current attack is valid, (-1,-1) is valid attack!!
	*/
	static bool isValAttack(std::pair<int, int>& attackOfPlayer);

	/*
	 *Returns the ship which in position board[row][col]
	 */
	Ship* getShipAtPosition(int row, int col) const;

	//	bool initPlayers(int playerNum, std::pair<char**, char**> boards);

public:
	Game(std::shared_ptr<IBattleshipGameAlgo> playerA, std::shared_ptr<IBattleshipGameAlgo> playerB, std::shared_ptr<boardType> board);
	~Game();

	int isHit(int row, int col, char& letter) const;

	void game();

	int checkWin() const;
};
