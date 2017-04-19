#include <windows.h>
#include "Ship.h"
#include "Sizes.h"
#include <iostream>
#include "Exception.h"
#include "Macros.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include "Player.h"
#include "Utilities.h"

#include <stdlib.h>
#include <direct.h>
#include <string>

using namespace std;
static const char DEFAULT_LETTER='a';
static const std::string ATTACK_A_SUFF = ".attack-a";
static const std::string ATTACK_B_SUFF = ".attack-b";
static const std::string BOARD_SUFF = ".sboard";

/*
 *Returns true if path exists && it's a directory
 */
static bool doesPathExist(const char* path)
{
	DWORD dirAttr = GetFileAttributesA(path);
	if (dirAttr == INVALID_FILE_ATTRIBUTES)
	{
		throw Exception(exceptionInfo(WRONG_PATH,path));
	} 
	return ((dirAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

/*
 *Returns true if "filename" ends with "suffix"  
 */
static bool endsWith(const std::string& fileName, const std::string& suffix)
{
	if (fileName.length() < suffix.length())
	{
		return false;
	}
	if (suffix.length() == 0) return true; //if suffix is the empty string returns true
	size_t fln = fileName.length();
	size_t sln = suffix.length();
	for (int i = 0 ; i < sln; ++i)
	{
		if (suffix.at(i)!=fileName.at(fln-sln+i))
		{
			return false;
		}
	}
	return true;
}

/* Checks if a given fileName ends with ATTACK_A_SUFF/ATTACK_B_SUFF/BOARD_SUFF, 
 * if it does && the according attackFileA/attackFileB/boardFile is NOT initiallized,
 * initializes it.
 * USER OF THIS FUNCTION HAVE TO FREE MEMORY THAT HAS BEEN ALLOCATED IN IT!!
 *  */
static void checkFileName(const std::string& fileName, char** boardFile, char** attackFileA, char** attackFileB)
{
	if (endsWith(fileName, ATTACK_A_SUFF) && *attackFileA==nullptr)
	{
		*attackFileA = new char[fileName.length() + 1];
		strcpy_s(*attackFileA, fileName.length() + 1 , fileName.c_str());
	} else if (endsWith(fileName, ATTACK_B_SUFF) && *attackFileB == nullptr)
	{
		*attackFileB = new char[fileName.length() + 1];
		strcpy_s(*attackFileB, fileName.length() + 1, fileName.c_str());
	} else if(endsWith(fileName, BOARD_SUFF) && *boardFile == nullptr)
	{
		*boardFile = new char[fileName.length() + 1];
		strcpy_s(*boardFile, fileName.length() + 1, fileName.c_str());
	}
}

/*Checks if the path exists,
 *that the 3 files are there, 
 *And if they are - updates their names */
static bool isValidPath(const char* path, char** boardFile, char** attackFileA, char** attackFileB)
{

	// Changing work directory to reletive path if needed
	if (_chdir(path) != 0)
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	std::string command = "";
	command.insert(0, "2>NUL dir /b /a-d *.*");
	command.append(" > file_names.txt");
	std::cout << "command is: " << command.c_str() << std::endl;
	bool b = system(command.c_str());

	if(b)
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	std::ifstream file("file_names.txt");
	if (!file) {
		std::cout << "Error: could not read file_names.txt." << std::endl;
		return false;
	}
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string line;
	try
	{
		while (std::getline(file, line))
		{

			checkFileName(line, boardFile, attackFileA, attackFileB);
		}
	} catch (std::ifstream::failure e)
	{
		if (!file.eof())
		{
			std::cout << "Error: could not open/read file_names.txt, error is: " << e.what() << std::endl;
			return false;
		}  //otherwise, the exception caught is because the file has ended and it's OK!
	}

	//making sure everything's initiallized
	if (*boardFile == nullptr || *attackFileA == nullptr || *attackFileB == nullptr) {
		return false;
	}
	
	//deletes the file we've created
	file.close();
	system("del file_names.txt");
	return true;
}

/*
 *Used when crucial files are missing, frees memory of files who where found,
 *Prints relevant errors to the screen
 */
static void printNotFoundFileErrors(const char* path,char* boardFile, char* attackFileA, char* attackFileB)
{
	if (boardFile == nullptr) {
		std::cout << "Missing board file (*.sboard) looking in path: " << path << std::endl;
		if (attackFileA != nullptr)
		{
			delete attackFileA; //clear memory
		} else
		{
			std::cout << "Missing attack file for player A (*.attack-a) looking in path: " << path << std::endl;
		}
		if (attackFileB != nullptr)
		{
			delete attackFileB; //clear memory
		} else
		{
			std::cout << "Missing attack file for player B (*.attack-b) looking in path: " << path << std::endl;
		}
		return;
	}
	if (attackFileA == nullptr) {
		delete boardFile; //clear memory, if got here it's not nullptr
		std::cout << "Missing attack file for player A (*.attack-a) looking in path: " << path << std::endl;
		if (attackFileB != nullptr)
		{
			delete attackFileB; //clear memory
		}
		else
		{
			std::cout << "Missing attack file for player B (*.attack-b) looking in path: " << path << std::endl;
		}
		return;
	}
	if (attackFileB == nullptr) {
		delete boardFile; //clear memory, if got here it's not nullptr
		delete attackFileA; //clear memory,  if got here it's not nullptr
		std::cout << "Missing attack file for player B (*.attack-b) looking in path: " << path << std::endl;
	}
}

/*
 *Frees memory of allocated board
 */
static void deleteBoard(char** board)
{
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		delete[] board[i];
	}
	delete[] board;
}

/*
 * This Function returns a refference to a 2D-array of chars, representing the game board
 * BOARD HAS TO BE DELETED USING "deleteBoard" FUNCTION!
 */
static char** getBoardFromFile(const char* boardFile){
	
	char** board = new char*[BOARD_LENGTH];
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		board[i] = new char[BOARD_LENGTH];
	}
		
	std::string line;
	
	//opening boardfile:
	std::ifstream bfile(boardFile);
	if (!bfile) {
		throw Exception("Error: failed opening board file.");
	}
	int row = 0;
	while (row < BOARD_LENGTH)
	{
		//reads 1 line from the file:
		line.clear();
		try
		{
			Utilities::getAllKindsOfLine(bfile, line);
		}
		catch (std::exception& e)
		{
			if (bfile.eof())
			{
				break;
			}
			else
			{
				deleteBoard(board);
				throw Exception("Error: failed reading from board file.");
			}
		}
		if (line.length() < BOARD_LENGTH)
		{
			for (int col = 0; col < line.length(); ++col)
			{
				board[row][col] = line.at(col);
			}
			for (int col = line.length(); col < BOARD_LENGTH; ++col)
			{
				board[row][col] = ' ';
			}
		} else
		{
			for (int col = 0; col < BOARD_LENGTH; ++col)
			{
				board[row][col] = line.at(col);
			}
		}
		++row;
	}

	if (row < BOARD_LENGTH)
	{
		for (int i = row; i < BOARD_LENGTH; ++i)
		{
			for (int j = 0; j < BOARD_LENGTH; ++j)
			{
				board[i][j] = ' ';
			}
		}
	}

	return board;
}


void printBoard(Ship *ships)
{
	for (int indexRow = 0; indexRow < BOARD_LENGTH; indexRow++)
	{
		for (int indexColumn = 0; indexColumn < BOARD_LENGTH; indexColumn++)
		{
			for (int indexShip = 0; indexShip < NUMBER_SHIPS; indexShip++)
			{
				bool letterInPosition = false;
				int indexInShip = 0;

				// Looping on the ships and their positions
				while (ships[indexShip].getPosition()[indexInShip] != nullptr)
				{
					// Checking if there is a match in ship position
					if (indexRow == ships[indexShip].getPosition()[indexInShip][0] &&
						indexColumn == ships[indexShip].getPosition()[indexInShip][1])
					{
						cout << ships[indexShip].getLetter();
						letterInPosition = true;
						break;
					}

					indexInShip++;
				}

				if (letterInPosition)
					continue;
				else
					cout << ' ';
			}
		}
		cout << endl;
	}
}

/*
* Gets a letter and the first index of the ship on the board:
* If there's sequence that might represent a valid ship - creates it
* Changes all ships (valid/invalid) letters to the default letter
* Updates values of:
*		numShipsForCurrPlayer - (+1) if a ship was added
*		wrongSizeOrShape - to "true" if the current letter is from a deformed ship
*		shipsOfPlayer - adds a ship to the players' ships (if ship might be valid)
*		badLetterIndexes - if it's not a ship, adds the Letters "bad" indexes to a list
*		board - updates each visited cell to the DEFAULT_LETTER
*/
void checkShipBorders(char ** board, int numRows, int numCols, int currRow, int currCol,
	char letter, int& numShipsForCurrPlayer, std::vector<Ship*>& shipsOfPlayer,
	bool& wrongSizeOrShape, std::vector<std::pair<int, int>*>& badLetterIndexes)
{
	if ((currRow >= numRows) || (currCol >= numCols)) //never supposed to get here,just to check ourselves
	{
		std::cout << "Error: wrong index was passed to function:\"checkShipBorders\", index: (" << currRow <<
			"," << currCol << ")" << std::endl;
		return;
	}

	if (board[currRow][currCol] != letter) //never supposed to get here
		return;

	int shipCells = 1;
	int row = currRow;
	int col = currCol;
	board[currRow][currCol] = DEFAULT_LETTER;
	//check for ship cells on next columns:
	if ((currCol < numCols - 1) && (board[currRow][currCol + 1]) == letter)
	{
		col++;
		while (shipCells < Ship::sizeOfShip(letter) && (col <= numCols - 1) && (board[currRow][col]) == letter)
		{
			board[currRow][col] = DEFAULT_LETTER;
			shipCells++;
			col++;
		}

		if (shipCells < Ship::sizeOfShip(letter)) //ship has a wrong size (too small)
		{
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes" letter appearences into the list
			{
				std::pair<int, int>* p1 = new std::pair<int, int>(currRow, currCol + i);
				badLetterIndexes.push_back(p1);
			}
			return;
		}
		//If gets here,shipCells == Ship::sizeOfShip(letter):
		if (((col <= numCols - 1) && board[currRow][col] != letter) || (col == numCols)) //Makes sure the ship is of right size (not too large)
		{
			Ship* ship =new Ship(letter);
			shipsOfPlayer.push_back(ship);
			for (int i = 0; i < shipCells; ++i)
			{
				(*(shipsOfPlayer[numShipsForCurrPlayer])).setPosition(i, currRow, currCol + i, 0);
			}
			numShipsForCurrPlayer++;
		}
		else //wrong size - ship's too big
		{
			while ((col <= numCols - 1) && (board[currRow][col]) == letter) //turns all letters to the default one
			{
				std::pair<int, int>* p1 = new std::pair<int, int>(currRow, col);
				badLetterIndexes.push_back(p1);
				board[currRow][col] = DEFAULT_LETTER;
				col++;
			}
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes"
			{
				std::pair<int, int>* p1 = new std::pair<int, int>(currRow, currCol + i);
				badLetterIndexes.push_back(p1);
			}
		}
		// Searching rows for the rest of the ship
	}
	else if ((row < numRows - 1) && (board[row + 1][currCol]) == letter)
	{
		row++;
		while (shipCells < Ship::sizeOfShip(letter) && (row <= numRows - 1) && (board[row][currCol]) == letter)
		{
			board[row][currCol] = DEFAULT_LETTER;
			shipCells++;
			row++;
		}

		if (shipCells < Ship::sizeOfShip(letter)) //ship has a wrong size (too small)
		{
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes" letter appearences into the list
			{
				std::pair<int, int>* p1 = new std::pair<int, int>(currRow + i, currCol);
				badLetterIndexes.push_back(p1);
			}
			return;
		}
		//If gets here,shipCells == Ship::sizeOfShip(letter):
		if (((row <= numRows - 1) && board[row][currCol] != letter) || (row == numRows)) //Makes sure the ship is of right size (not too large)
		{
			Ship* ship = new Ship(letter);
			shipsOfPlayer.push_back(ship);
			for (int i = 0; i < shipCells; ++i)
			{
				(*(shipsOfPlayer[numShipsForCurrPlayer])).setPosition(i,currRow + i, currCol, 0);
			}
			numShipsForCurrPlayer++;
		}
		else //wrong size - ship's too big
		{
			while ((row <= numRows - 1) && (board[row][currCol]) == letter) //turns all letter to the default one
			{
				std::pair<int, int>* p1 = new std::pair<int, int>(row, currCol);
				badLetterIndexes.push_back(p1);
				board[row][currCol] = DEFAULT_LETTER;
				row++;
			}
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes"
			{
				std::pair<int, int>* p1 = new std::pair<int, int>(currRow + i, currCol);
				badLetterIndexes.push_back(p1);
			}
		}
	}
	else // Ship has only one (the current) cell
	{
		if (Ship::sizeOfShip(letter) != 1)
		{
			std::pair<int, int>* p1 = new std::pair<int, int>(currRow, col);
			badLetterIndexes.push_back(p1);
			wrongSizeOrShape = true;
			return;
		}
		else
		{
			Ship* ship = new Ship(letter);
			shipsOfPlayer.push_back(ship);
			(*(shipsOfPlayer[numShipsForCurrPlayer])).setPosition(0, currRow, currCol, 0);
			numShipsForCurrPlayer++;
		}
	}
}

/*
 * Returns true if ship is next to another ship/"bad index", of the same letter! (==wrong shape)
 * @Params:
 *		badLetterIndexes - a vector containing all indexes in which the letter appeard in, and weren't valid
 *		ship - the ship we're checking
 *		letter - the ship's letter
 */
bool checkShipShape(Ship* ship,char letter, std::vector<std::pair<int, int>*>& badLetterIndexes, std::vector<Ship*>& shipsOfPlayer)
{
	bool res = false;
	int** pos = (*ship).getPosition();
	int row = -1;
	int col = -1;
	for (int i = 0; i < (*ship).getShipSize(); ++i)
	{
		row = pos[i][0];
		col = pos[i][1];
		for (int k = 0; k <  badLetterIndexes.size(); ++k)
		{
			if (((row == (*badLetterIndexes.at(k)).first)&&( col-1 == (*badLetterIndexes.at(k)).second || col + 1 == (*badLetterIndexes.at(k)).second))
				|| ((col == (*badLetterIndexes.at(k)).second)&&(row - 1 == (*badLetterIndexes.at(k)).first || row + 1 == (*badLetterIndexes.at(k)).first)))
			{
				res = true;
				break;
			}
		}
		if (res) break;
	}
	
	if(!res) //now check in "valid" ships
	{
		for (int i = 0; i < shipsOfPlayer.size(); ++i)
		{
			if (!(pos[0][0] == (*(shipsOfPlayer.at(i))).getPosition()[0][0] && pos[0][1] == (*(shipsOfPlayer.at(i))).getPosition()[0][1])) //makes sure it's NOT the same ship
			{
				if ((*(shipsOfPlayer.at(i))).getLetter() == letter) {
					Ship* shipToCompare = shipsOfPlayer.at(i);
					for (int j = 0; j < (*ship).getShipSize(); ++j)
					{
						row = pos[j][0];
						col = pos[j][1];
						for (int k = 0; k < (*shipToCompare).getShipSize(); ++k)
						{
							if (((row == (*shipToCompare).getPosition()[k][0]) && (col - 1 == (*shipToCompare).getPosition()[k][1] || col + 1 == (*shipToCompare).getPosition()[k][1]))
								|| ((col == (*shipToCompare).getPosition()[k][1]) && (row - 1 == (*shipToCompare).getPosition()[k][0] || row + 1 == (*shipToCompare).getPosition()[k][0])))
							{
								res = true;
								break;
							}
						}
						if (res) break;
					}

				}
			}
			if(res)	break;
		}
	}
	if (res)
	{
		for (int i = 0; i < Ship::sizeOfShip(letter); ++i)
		{
			std::pair<int, int>* badindex = new std::pair<int, int>(pos[i][0], pos[i][1]);
			badLetterIndexes.push_back(badindex);
		}
	}
	return res;
}

/*
*Returns true if there IS an adjacent (different) ship.
*/
bool checkNeighbourShips1(char ** board, int currentRow, int currentCol, int numRows, int numCols)
{
	if (currentRow - 1 >= 0)
	{
		if (Ship::isShip(board[currentRow - 1][currentCol]) && (board[currentRow - 1][currentCol] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	if (currentCol -1 >=0)
	{
		if (Ship::isShip(board[currentRow][currentCol - 1]) && (board[currentRow][currentCol - 1] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	if (currentRow+1 < numRows)
	{
		if (Ship::isShip(board[currentRow + 1][currentCol]) && (board[currentRow + 1][currentCol] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	if (currentCol+1 < numCols)
	{
		if (Ship::isShip(board[currentRow][currentCol + 1]) && (board[currentRow][currentCol + 1] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	return false;
}

std::pair <std::vector<Ship*>*,std::vector<Ship*>*>* checkBoard(char ** board, int numRows, int numCols)
{
	std::vector<Ship*>* shipsA = new std::vector<Ship*>;
	std::vector<Ship*>* shipsB = new std::vector<Ship*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_B = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_b = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_P = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_p = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_M = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_m = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_D = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_d = new std::vector<std::pair<int, int>*>;
	//Initialized to an arbitrary "badLetterIndex" in order to prevent error:
	std::vector<std::pair<int, int>*>* badLetterIndexes = badLetterIndexes_B;
	
	std::set<char>  wrongSizeShapeShips;

	int indexShipA = 0;
	int indexShipB = 0;

	bool boardNotValid = false;
	bool wrongSizeOrShape;
	bool adjacentShips = false;
	char letter = '`';
	
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			if (Ship::isShip(board[row][col]))
			{
				adjacentShips = checkNeighbourShips1(board, row, col, numRows, numCols);
			}
			if (adjacentShips) break;
		}
		if (adjacentShips) break;
	}

	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			wrongSizeOrShape = false;
			letter = board[row][col];
			switch (letter)
			{
			case 'B':badLetterIndexes = badLetterIndexes_B;
				break;
			case 'b':badLetterIndexes = badLetterIndexes_b;
				break;
			case 'P':badLetterIndexes = badLetterIndexes_P;
				break;
			case 'p':badLetterIndexes = badLetterIndexes_p;
				break;
			case 'M':badLetterIndexes = badLetterIndexes_M;
				break;
			case 'm':badLetterIndexes = badLetterIndexes_m;
				break;
			case 'D':badLetterIndexes = badLetterIndexes_D;
				break;
			case 'd':badLetterIndexes = badLetterIndexes_d;
				break;
			default: //TODO:: throw exception or something, never supposed to get here anyway
				break;
			}
			if (Ship::isShip(letter)) {
				if (islower(letter))//letter is "player B"'s letter (small letters)
				{
					checkShipBorders(board, numRows, numCols, row, col, letter, indexShipB, *shipsB, wrongSizeOrShape, *badLetterIndexes);
				} else //"player A"'s letter
				{
					checkShipBorders(board, numRows, numCols, row, col, letter, indexShipA, *shipsA, wrongSizeOrShape, *badLetterIndexes);
				}
				if (wrongSizeOrShape)
				{
					wrongSizeShapeShips.insert(letter);
				}
			}
		}
	}

	//check for wrong size or shape(in near columns/rows)
	for (int m = 0; m < 2; ++m)
	{
		std::vector<Ship*>* ships = (m == 0) ? shipsA : shipsB;
		int numOfShipsToCheck = (m == 0) ? (*shipsA).size() : (*shipsB).size();
		int originalShipIndex = 0;
		int currShipToCheck = 0;
		while (originalShipIndex < numOfShipsToCheck)
		{
			switch ((*((*ships).at(currShipToCheck))).getLetter())
			{
			case 'B':badLetterIndexes = badLetterIndexes_B;
				break;
			case 'b':badLetterIndexes = badLetterIndexes_b;
				break;
			case 'P':badLetterIndexes = badLetterIndexes_P;
				break;
			case 'p':badLetterIndexes = badLetterIndexes_p;
				break;
			case 'M':badLetterIndexes = badLetterIndexes_M;
				break;
			case 'm':badLetterIndexes = badLetterIndexes_m;
				break;
			case 'D':badLetterIndexes = badLetterIndexes_D;
				break;
			case 'd':badLetterIndexes = badLetterIndexes_d;
				break;
			default: //TODO:: throw exception or something, never supposed to get here anyway
				break;
			}
			if (checkShipShape((*ships).at(currShipToCheck), (*((*ships).at(currShipToCheck))).getLetter(), *badLetterIndexes, (*ships)))//if true, it's invalid ship
			{
				(m==0) ? indexShipA-- : indexShipB--;
				wrongSizeShapeShips.insert((*(*ships).at(currShipToCheck)).getLetter());
				delete (*ships).at(currShipToCheck); //erase doesn't free allocated ship's space
				(*ships).erase((*ships).begin() + currShipToCheck);
				currShipToCheck--;
			}
			currShipToCheck++;
			originalShipIndex++;
		}
	}
		
	///*Searching for adjacent ships*/
	//for (int n = 0; n < (*shipsA).size(); ++n)
	//{
	//	for (int m = n+1; m < (*shipsA).size(); ++m)
	//	{
	//		adjacentShips = checkNeighbourShips((*shipsA).at(n), (*shipsA).at(m));
	//		if (adjacentShips) break;
	//	}
	//	if (adjacentShips) break;
	//	for (int m = 0; m < (*shipsB).size(); ++m)
	//	{
	//		adjacentShips = checkNeighbourShips((*shipsA).at(n), (*shipsB).at(m));
	//		if (adjacentShips) break;
	//	}
	//	if (adjacentShips) break;
	//}
	//if (!adjacentShips)
	//{
	//	for (int n = 0; n < (*shipsB).size(); ++n)
	//	{
	//		for (int m = n + 1; m < (*shipsB).size(); ++m)
	//		{
	//			adjacentShips = checkNeighbourShips((*shipsB).at(n), (*shipsB).at(m));
	//			if (adjacentShips) break;

	//		}
	//		if (adjacentShips) break;
	//	}
	//}
	///*End of searching for adjacent ships*/
	
	/*Printing errors as instructed - wrong size or shape*/
	if (wrongSizeShapeShips.size() > 0)
	{
		boardNotValid = true;
		for (std::set<char>::iterator i = wrongSizeShapeShips.begin(); i != wrongSizeShapeShips.end(); ++i)
		{
			if (!islower(*i))
			{
				std::cout << "Wrong size or shape for ship " << *i << " for player A" << std::endl;
			}
		}
		for (std::set<char>::iterator i = wrongSizeShapeShips.begin(); i != wrongSizeShapeShips.end(); ++i)
		{
			if (islower(*i))
			{
				std::cout << "Wrong size or shape for ship " << *i << " for player B" << std::endl;
			}
		}
	}

	/*Printing errors as instructed - too many / to few ships*/
	if ((*shipsA).size() > NUMBER_SHIPS) {
		boardNotValid = true;
		std::cout << "Too many ships for player A" << std::endl;
	}
	if ((*shipsA).size() < NUMBER_SHIPS)
	{
		boardNotValid = true;
		std::cout << "Too few ships for player A" << std::endl;
	}
	if ((*shipsB).size() > NUMBER_SHIPS) {
		boardNotValid = true;
		std::cout << "Too many ships for player B" << std::endl;
	}
	if ((*shipsB).size() < NUMBER_SHIPS)
	{
		boardNotValid = true;
		std::cout << "Too few ships for player B" << std::endl;
	}
	if (adjacentShips)
	{
		boardNotValid = true;
		std::cout << "Adjacent Ships on Board" << std::endl;
	}

	/*DELETEING allocations:*/
	for (int k = 0; k < (*badLetterIndexes_B).size();++k)
	{
		delete (*badLetterIndexes_B).at(k);
	}
	for (int k = 0; k < (*badLetterIndexes_b).size(); ++k)
	{
		delete (*badLetterIndexes_b).at(k);
	}
	for (int k = 0; k < (*badLetterIndexes_P).size(); ++k)
	{
		delete (*badLetterIndexes_P).at(k);
	}
	for (int k = 0; k < (*badLetterIndexes_p).size(); ++k)
	{
		delete (*badLetterIndexes_p).at(k);
	}
	for (int k = 0; k < (*badLetterIndexes_M).size(); ++k)
	{
		delete (*badLetterIndexes_M).at(k);
	}
	for (int k = 0; k < (*badLetterIndexes_m).size(); ++k)
	{
		delete (*badLetterIndexes_m).at(k);
	}
	for (int k = 0; k < (*badLetterIndexes_D).size(); ++k)
	{
		delete (*badLetterIndexes_D).at(k);
	}
	for (int k = 0; k < (*badLetterIndexes_d).size(); ++k)
	{
		delete (*badLetterIndexes_d).at(k);
	}
	delete badLetterIndexes_B;
	delete badLetterIndexes_b;
	delete badLetterIndexes_P;
	delete badLetterIndexes_p;
	delete badLetterIndexes_M;
	delete badLetterIndexes_m;
	delete badLetterIndexes_D;
	delete badLetterIndexes_d;
	/*END OF DELETEING allocations*/

	/*If board's invalid - deletes all ships and return nullptr*/
	if (boardNotValid)
	{
		for (int i = 0; i < (*shipsA).size(); ++i)
		{
			delete (*shipsA).at(i);
		}
		for (int i = 0; i < (*shipsB).size(); ++i)
		{
			delete (*shipsB).at(i);
		}
		delete shipsA;
		delete shipsB;
		return nullptr;
	}

	std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* ret = new std::pair<std::vector<Ship*>*, std::vector<Ship*>*>(shipsA, shipsB);
	return ret;
}


//void game(Player* playerA, Player* playerB)
//{
//	int isHitA = 0;
//	int isHitB = 0;
//	int playerPlaying = 0;
//	//TODO: add points checker for the players
//	while ((*playerA).attackNumber != -1 || (*playerB).attackNumber != -1)
//	{
//		pair<int, int> attack;
//		AttackResult result = AttackResult::Miss;
//		// Inside player A and there are attacks left
//		if (playerPlaying == (*playerA).playerNum && (*playerA).attackNumber != -1)
//			attack = (*playerA).attack();
//		// Inside player B and there are attacks left
//		else if (playerPlaying == (*playerB).playerNum && (*playerB).attackNumber != -1)
//			attack = (*playerB).attack();
//
//		isHitA = (*playerA).isHit(attack.first, attack.second);
//		isHitB = (*playerB).isHit(attack.first, attack.second);
//
//		// Hit before
//		if (isHitA == 3 || isHitB == 3)
//		{
//			// Send hit, change playerPlaying
//			result = AttackResult::Hit;
//			playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//
//		}
//		// Sink
//		else if (isHitA == 2 || isHitB == 2)
//		{
//			result = AttackResult::Sink;
//			// Self sink and change playerPlaying to the other
//			if ((isHitA == 2 && playerPlaying == (*playerA).playerNum) || (isHitB == 2 && playerPlaying == (*playerB).playerNum))
//			{
//				playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//			}
//			std::cout << std::to_string(attack.first) + "," + std::to_string(attack.second);
//			std::cout << " is dead" << std::endl;
//
//		}
//		// Hit 
//		else if (isHitA == 1 || isHitB == 1)
//		{
//			result = AttackResult::Hit;
//			// Self hit and change playerPlaying to the other
//			if ((isHitA == 1 && playerPlaying == (*playerA).playerNum) || (isHitB == 1 && playerPlaying == (*playerB).playerNum))
//			{
//				playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//			}
//			std::cout << std::to_string(attack.first) + "," + std::to_string(attack.second);
//			std::cout << " is hit" << std::endl;
//		}
//		// Miss
//		else
//		{
//			// Change playerPlaying
//			playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
//		}
//
//		// Notify players on the result
//		(*playerA).notifyOnAttackResult(playerPlaying, attack.first, attack.second, result);
//		(*playerB).notifyOnAttackResult(playerPlaying, attack.first, attack.second, result);
//	}
//}


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	std::string path;
	if (argc == 1)
	{
		path = Utilities::workingDirectory();
	} else
	{
		path = argv[1];
	}

	char* boardFilePtr = nullptr;
	char* attackFileAPtr = nullptr;
	char* attackFileBPtr = nullptr;

	bool pathIsValid = false;
	//std::cout << "path is: " << path << std::endl;
	try
	{
		pathIsValid = isValidPath(path.c_str(), &boardFilePtr, &attackFileAPtr, &attackFileBPtr);
	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	if(!pathIsValid)
	{
		printNotFoundFileErrors(path.c_str(), boardFilePtr, attackFileAPtr, attackFileBPtr);
		return 1;
	}

	std::string fullPathToBoard = path +"\\"+ boardFilePtr;
	char** board = getBoardFromFile(fullPathToBoard.c_str());
	std::string fullPathToAttackFileA = path + "\\" + attackFileAPtr;
	//std::string* fullPathToAttackFileA = new std::string(path);
	//(*fullPathToAttackFileA).append("\\");
	//(*fullPathToAttackFileA).append(attackFileAPtr);

	std::string fullPathToAttackFileB = path + "\\" + attackFileBPtr;
	//vector<pair<int, int>> attackFileA = getAttackFile(fullPathToAttackFileA.c_str());
	//vector<pair<int, int>> attackFileB = getAttackFile(fullPathToAttackFileB.c_str());

	//prints board:
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (int j = 0; j < BOARD_LENGTH; ++j)
		{
			std::cout << board[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips = checkBoard(board, BOARD_LENGTH, BOARD_LENGTH);

	//prints board:
	std::cout << "********PRINTING BOARD AFTER CHANGE********" << std::endl;
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (int j = 0; j < BOARD_LENGTH; ++j)
		{
			std::cout << board[i][j] << "  ";
		}
		std::cout << std::endl;
	}
	//int numShipsForCurrPlayer = 0;
	//std::vector<Ship> shipsOfPlayer;
	//bool wrongSizeOrShape = false;
	//std::vector<std::pair<int, int>*> badLetterIndexes;
	//checkShipBorders(b, 5, 5, 2, 4, 'P', numShipsForCurrPlayer, shipsOfPlayer, wrongSizeOrShape, badLetterIndexes);


	//std::cout << "numShipsForCurrPlayer is: " << numShipsForCurrPlayer << std::endl;
	//std::cout << "shipsOfPlayer size is: " << shipsOfPlayer.size() << std::endl;
	//std::cout << "wrongSizeOrShape is: " << (wrongSizeOrShape ? "true":"false") << std::endl;
	//for (int i = 0; i < badLetterIndexes.size(); ++i)
	//{
	//	std::cout << "badLetterIndexes in index: " << i << " is: " << (*(badLetterIndexes[i])).first << "," << (*badLetterIndexes[i]).second << std::endl;
	//}
	//shipsOfPlayer.clear();



	///*char** board= getBoardFromFile("good_board_0.sboard");

	//deleteBoard(board);*/

	////prints board:
	//for (int i = 0; i < BOARD_LENGTH; ++i)
	//{
	//	for (int j = 0; j < BOARD_LENGTH; ++j)
	//	{
	//		std::cout << board[i][j] << "\t";
	//	}
	//	std::cout << std::endl;
	//}



	//deleteBoard(board);


	
	//deletes ships to free allocated space(?)
	//(*playersShips).first.clear();
	//(*playersShips).second.clear();
	//delete playersShips;


	// Creating new players for the test of the game
	Player* playerA = new Player(PLAYER_A, fullPathToAttackFileA, (*playersShips).first);
	Player* playerB = new Player(PLAYER_B, fullPathToAttackFileB, (*playersShips).second);

	int i = playerA->getAttackNumber();
	std::cout << "Attacks of player A are:" << std::endl;
	while (i!=-1)
	{
		std::cout << "this is attack number: " << i << std::endl;
		std::pair<int, int>* att = playerA->getAttack();
		std::cout << "(" << att->first << "," << att->second << ")" << std::endl;
		i= playerA->getAttackNumber();
	}

	for (int i = 0 ; i < NUMBER_SHIPS; ++i)
	{
		playerB->getShips()[i]->printShipInfo();
	}

	if (playersShips != nullptr)
	{
		//std::cout << "Player A's ships are:" << std::endl;
		//for (int i = 0; i < (*((*playersShips).first)).size(); ++i)
		//{
		//	(*((*((*playersShips).first)).at(i))).printShipInfo();
		//}
		//std::cout << "Player B's ships are:" << std::endl;
		//for (int i = 0; i < (*((*playersShips).second)).size(); ++i)
		//{
		//	(*((*((*playersShips).second)).at(i))).printShipInfo();
		//}

		//deletes ships to free allocated space
		for (int i = 0; i < (*(*playersShips).first).size(); ++i)
		{
			delete ((*((*playersShips).first)).at(i));
		}
		delete (*playersShips).first;
		for (int i = 0; i < (*(*playersShips).second).size(); ++i)
		{
			delete ((*((*playersShips).second)).at(i));
		}
		delete (*playersShips).second;
		delete playersShips;
	}
	else
	{
		std::cout << "Board is invalid." << std::endl;
	}



	//playerA->attacks = attackFileA;
	//Ship *s1 = new Ship('B');
	//s1->setPosition(0, 1, 4, 0);
	//playerA->ships[0] = *s1;

	//Player *playerB = new Player();
	//playerB->attacks = attackFileB;
	//playerB->playerNum = 1;
	//s1 = new Ship('b');
	//s1->setPosition(0, 10, 4, 0);
	//playerB->ships[2] = *s1;

	//Ship *s2 = new Ship('P');
	//s2->setPosition(0, 2, 8, 0);
	//s2->setPosition(1, 2, 9, 0);
	//playerA->ships[1] = *s2;

	//Ship *s3 = new Ship('M');
	//s3->setPosition(0, 4, 6, 0);
	//s3->setPosition(1, 4, 7, 0);
	//s3->setPosition(2, 4, 8, 0);
	//playerA->ships[2] = *s3;

	//Ship *s4 = new Ship('B');
	//(*s4).setPosition(0, 9, 1, 0);
	//playerA->ships[3] = *s4;

	//Ship *s5 = new Ship('P');
	//(*s5).setPosition(0, 10, 9, 0);
	//(*s5).setPosition(1, 10, 10, 0);
	//playerA->ships[4] = *s5;
	//playerA->playerNum = 0;

	//int** pos = (*s2).getPosition();
	//for (int k = 0; k < (*s2).getShipSize(); ++k)
	//{
	//	std::cout << "(" << pos[k][0] << "," << pos[k][1] << ")  ,  ";
	//}
	//std::cout << std::endl;


	//s2 = new Ship('p');
	//s2->setPosition(0, 2, 2, 0);
	//s2->setPosition(1, 3, 2, 0);
	//playerB->ships[0] = *s2;

	//s3 = new Ship('m');
	//s3->setPosition(0, 5, 1, 0);
	//s3->setPosition(1, 6, 1, 0);
	//s3->setPosition(2, 7, 1, 0);
	//playerB->ships[1] = *s3;

	//s4 = new Ship('b');
	//(*s4).setPosition(0, 6, 10, 0);
	//playerA->ships[3] = *s4;

	//s5 = new Ship('d');
	//s5->setPosition(0, 5, 4, 0);
	//s5->setPosition(1, 6, 4, 0);
	//s5->setPosition(2, 7, 4, 0);
	//s5->setPosition(3, 8, 4, 0);
	//playerA->ships[4] = *s5;

	//game(playerA, playerB);
	delete playerA;
	delete playerB;
	deleteBoard(board);
	if (pathIsValid)
	{
		delete[] boardFilePtr;
		delete[] attackFileAPtr;
		delete[] attackFileBPtr;
	}
	return 0;
}
