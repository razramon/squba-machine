#pragma once

#include "Ship.h"
#include "Utilities.h"
#include <vector>
#include <set>
#include <fstream>
#include <Windows.h>
#include <memory>
#include "IBattleshipGameAlgo.h"
#include "../Common/Logger.h"


#define DEFAULT_LETTER 'a';
#define EMPTY_LETTER ' ';

typedef std::vector<std::vector<std::vector<char>>> boardType;
typedef std::shared_ptr<std::vector<std::shared_ptr<Ship>>> ptrToShipsVector;

class BoardCreator
{
	BoardCreator() = delete;
	~BoardCreator() = delete;
	static const std::string BOARD_SUFF;
	static const char DELIMETER;

	static int getIndexOfRelevantBadCoordsVector(char letter);

	/*
	 * Fills board[depth] with spaces, starting at position: board[depth][row][0] 
	 * until position board[depth][numRows-1][numCols-1]
	 */
	static void fillRows(int row, int numRows, int numCols, int depth, std::unique_ptr<boardType>& board);

public:
	/*
	* Gets a vector of ships, updated board to conatin all letters representing ships
	*/
	static void updateShipsInBoard(std::shared_ptr<boardType>& board, ptrToShipsVector ships);

	static std::shared_ptr<boardType> getBoardFromShips(ptrToShipsVector ships, int numRows, int numCols, int numDepth);

	static void printCoord(Coordinate c);
	static int& updateCoordinate(int& row_ind, int& col_ind, int& depth_ind, Ship::INDEX_3D dimentionToCheck);
	static int checkSequence(const Coordinate& currPos, Ship::INDEX_3D dimentionToCheck, int maxIndex, std::unique_ptr<boardType>& board, char letter);
	static void insertBadCoords(int shipCells, bool& wrongSizeOrShape, std::shared_ptr<std::vector<Coordinate>>& badLetterCoords,
		int row, int col, int depth, Ship::INDEX_3D dimentionToUpdate);
	static void createShipAtCoord(char letter, std::vector<std::shared_ptr<Ship>>& shipsOfPlayer, int& numShipsForCurrPlayer,
		int row, int col, int depth, Ship::INDEX_3D dimentionToUpdate);
	static void checkShipBorders(std::unique_ptr<boardType>& board, int numRows, int numCols, int numDepth, const Coordinate& currPos,
		char letter, int& numShipsForCurrPlayer, std::vector<std::shared_ptr<Ship>>& shipsOfPlayer,
		bool& wrongSizeOrShape, std::shared_ptr<std::vector<Coordinate>>& badLetterCoords);
	static bool twoCoordsNeighbours(Coordinate c1, Coordinate c2);
	static bool checkShipShape(std::shared_ptr<Ship>& ship, char letter, std::shared_ptr<std::vector<Coordinate>>& badLetterCoords, std::vector<std::shared_ptr<Ship>>& shipsOfPlayer);
	static bool checkNeighbourShips(std::unique_ptr<boardType>& board, int currentRow, int currentCol, int currentDepth, int numRows, int numCols, int numDepth);
	static std::shared_ptr<std::pair<ptrToShipsVector, ptrToShipsVector>> checkBoard(std::unique_ptr<boardType>& board, int numRows, int numCols, int numDepth);
	static void getDimentions(int& numRows, int& numCols, int& numDepth, std::string line);
	static std::unique_ptr<boardType> getBoardFromFile(const char* boardFile, int& numRows, int& numCols, int& numDepth);


	static bool findBoardFile(const char* path, size_t pathLen, char** boardFile);
	static std::unique_ptr<boardType> createBoard(int numRows, int numCols, int numDepth);
	static std::unique_ptr<boardType> copyBoard(const std::shared_ptr<boardType>& board, int numRows, int numCols, int numDepth);
	static void printBoard(std::shared_ptr<boardType> board, int numRows, int numCols, int numDepth);

};

