#pragma once
#include "../submarinesGame/IBattleshipGameAlgo.h"
#include "../submarinesGame/BoardCreator.h"

class DllKnownStepsAlgo : public IBattleshipGameAlgo
{
	//Note: this player doesn't need to keep board - because he doesn't do anything about it...
	//char** myBoard;
	static const int NOT_INITIALIZED;
	int rows;
	int cols;
	int playerNum;
	int attackNumber;//starts from zero when initialized, and goes up as we deliver attacks to "Game".
	std::vector<std::pair<int, int>>* attacks;
	static void getAttackFromLine(std::string & line, std::pair<int, int>& attack);
	static std::vector<std::pair<int, int>>* getAttackFile(const char* attackFile);


	/*
	 * Returns the relevant full path to attack file - according to playerNumber
	 */
	std::string getFullPathToAttackFile(const std::vector<std::string>& attackFiles) const;

public:
	DllKnownStepsAlgo();
	~DllKnownStepsAlgo();
	/* Called once (by the Game) to notify (this) player on his board
	* by: http://moodle.tau.ac.il/mod/forum/discuss.php?d=54032,
	* The convention is that a space - ' ', represents "sea",
	* And Letters Represent Ships.
	*/
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	/*
	* Called once to allow init from files if needed returns whether the init succeeded or failed.
	* Path is absolute path (makes sure that it is to a directory) in which the attack file supposed to be : http://moodle.tau.ac.il/mod/forum/discuss.php?d=52703
	*/
	bool init(const std::string& path) override;
	/*
	 * Returns (a copy of) the next player's attack: IN RANGE 1-10!! (not 0-9)
	 */
	std::pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
};
