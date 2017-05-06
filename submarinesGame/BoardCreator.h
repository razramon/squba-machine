#pragma once
#include "Ship.h"
#include "Utilities.h"
#include <vector>
#include <set>
#include <fstream>

class BoardCreator
{
	static const char DEFAULT_LETTER = 'a';
public:
	BoardCreator();
	~BoardCreator();
	static void checkShipBorders(char ** board, int numRows, int numCols, int currRow, int currCol,
		char letter, int& numShipsForCurrPlayer, std::vector<Ship*>& shipsOfPlayer,
		bool& wrongSizeOrShape, std::vector<std::pair<int, int>*>& badLetterIndexes);
	static bool checkShipShape(Ship* ship, char letter, std::vector<std::pair<int, int>*>& badLetterIndexes, std::vector<Ship*>& shipsOfPlayer);
	static bool checkNeighbourShips(char ** board, int currentRow, int currentCol, int numRows, int numCols);
	static std::pair <std::vector<Ship*>*, std::vector<Ship*>*>* checkBoard(char ** board, int numRows, int numCols);
	static char** getBoardFromFile(const char* boardFile);
	static int findBoardFile(const char* path, size_t pathLen, char* boardFile);
};

