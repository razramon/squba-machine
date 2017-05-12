#include "BoardCreator.h"

const std::string BoardCreator::BOARD_SUFF = ".sboard";

std::pair<char **,char**> BoardCreator::getInitBoardForEachPlayer(std::pair<std::vector<Ship*>*, std::vector<Ship*>*>* playersShips) {
	
	return std::make_pair(getBoardFromShips((*playersShips).first), getBoardFromShips((*playersShips).second));
}

char ** BoardCreator::getBoardFromShips(std::vector<Ship*>* ships) {

	char ** board = new char*[BOARD_LENGTH];
	
	for (int indexRow = 0; indexRow < BOARD_LENGTH; indexRow++) {

		board[indexRow] = new char[BOARD_LENGTH];
		for (int indexCol = 0; indexCol < BOARD_LENGTH; indexCol++) {

			board[indexRow][indexCol] = DEFAULT_LETTER;
		}
	}
	std::vector<Ship*>* ps = ships;
	for (int i = 0; i < (*ps).size(); ++i)
	{
		Ship* s = (*ps).at(i);

		int** positions = (*s).getPosition();
		char letter = (*s).getLetter();

		for (int pos = 0; pos < (*s).getShipSize(); ++pos) {

			board[positions[pos][0]][positions[pos][1]] = letter;
		}
	}
	return board;
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
void BoardCreator::checkShipBorders(char ** board, int numRows, int numCols, int currRow, int currCol,
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
			Ship* ship = new Ship(letter);
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
				(*(shipsOfPlayer[numShipsForCurrPlayer])).setPosition(i, currRow + i, currCol, 0);
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
bool BoardCreator::checkShipShape(Ship* ship, char letter, std::vector<std::pair<int, int>*>& badLetterIndexes, std::vector<Ship*>& shipsOfPlayer)
{
	bool res = false;
	int** pos = (*ship).getPosition();
	int row = -1;
	int col = -1;
	for (int i = 0; i < (*ship).getShipSize(); ++i)
	{
		row = pos[i][0];
		col = pos[i][1];
		for (int k = 0; k < badLetterIndexes.size(); ++k)
		{
			if (((row == (*badLetterIndexes.at(k)).first) && (col - 1 == (*badLetterIndexes.at(k)).second || col + 1 == (*badLetterIndexes.at(k)).second))
				|| ((col == (*badLetterIndexes.at(k)).second) && (row - 1 == (*badLetterIndexes.at(k)).first || row + 1 == (*badLetterIndexes.at(k)).first)))
			{
				res = true;
				break;
			}
		}
		if (res) break;
	}

	if (!res) //now check in "valid" ships
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
			if (res)	break;
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
bool BoardCreator::checkNeighbourShips(char ** board, int currentRow, int currentCol, int numRows, int numCols)
{
	if (currentRow - 1 >= 0)
	{
		if (Ship::isShip(board[currentRow - 1][currentCol]) && (board[currentRow - 1][currentCol] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	if (currentCol - 1 >= 0)
	{
		if (Ship::isShip(board[currentRow][currentCol - 1]) && (board[currentRow][currentCol - 1] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	if (currentRow + 1 < numRows)
	{
		if (Ship::isShip(board[currentRow + 1][currentCol]) && (board[currentRow + 1][currentCol] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	if (currentCol + 1 < numCols)
	{
		if (Ship::isShip(board[currentRow][currentCol + 1]) && (board[currentRow][currentCol + 1] != board[currentRow][currentCol]))
		{
			return true;
		}
	}
	return false;
}

std::pair <std::vector<Ship*>*, std::vector<Ship*>*>* BoardCreator::checkBoard(char ** board, int numRows, int numCols)
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
				adjacentShips = checkNeighbourShips(board, row, col, numRows, numCols);
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
			default: //never supposed to get here anyway
				break;
			}
			if (Ship::isShip(letter)) {
				if (islower(letter))//letter is "player B"'s letter (small letters)
				{
					checkShipBorders(board, numRows, numCols, row, col, letter, indexShipB, *shipsB, wrongSizeOrShape, *badLetterIndexes);
				}
				else //"player A"'s letter
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
		size_t numOfShipsToCheck = (m == 0) ? (*shipsA).size() : (*shipsB).size();
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
			default: //never supposed to get here anyway
				break;
			}
			if (checkShipShape((*ships).at(currShipToCheck), (*((*ships).at(currShipToCheck))).getLetter(), *badLetterIndexes, (*ships)))//if true, it's invalid ship
			{
				(m == 0) ? indexShipA-- : indexShipB--;
				wrongSizeShapeShips.insert((*(*ships).at(currShipToCheck)).getLetter());
				delete (*ships).at(currShipToCheck); //erase doesn't free allocated ship's space
				(*ships).erase((*ships).begin() + currShipToCheck);
				currShipToCheck--;
			}
			currShipToCheck++;
			originalShipIndex++;
		}
	}

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
	for (int k = 0; k < (*badLetterIndexes_B).size(); ++k)
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

/*
* This Function returns a refference to a 2D-array of chars, representing the game board
* BOARD HAS TO BE DELETED USING "freeBoard" FUNCTION!
*/
char** BoardCreator::getBoardFromFile(const char* boardFile) {

	char** board = createBoard(BOARD_LENGTH, BOARD_LENGTH);

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
			//Frees memory of allocated board
			for (int i = 0; i < BOARD_LENGTH; ++i)
			{
				delete[] board[i];
			}
			delete[] board;
			bfile.close();
			std::string str = e.what();
			str.insert(0, "Error: failed reading from board file: ");
			throw Exception(str.c_str());
		}
		if (line.length() < BOARD_LENGTH)
		{
			for (size_t col = 0; col < line.length(); ++col)
			{
				board[row][col] = line.at(col);
			}
			for (size_t col = line.length(); col < BOARD_LENGTH; ++col)
			{
				board[row][col] = ' ';
			}
		}
		else
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
	bfile.close();
	return board;
}

/*
* Gets a path,
* If it's a valid path and there exists a board file - updates "boardFile" to contain the name of the board file
* and returns true.
* If path is valid but there's no board file - returns false.
* If path file isn't valid or an error accured - throws exception
*/
bool BoardCreator::findBoardFile(const char* path, size_t pathLen, char** boardFile)
{
	try
	{
		if(!(Utilities::doesPathExist(path)))
		{
			throw Exception(exceptionInfo(WRONG_PATH, path));
		}
	} catch (std::exception& e)
	{
		throw e;
	}
	
	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	bool retVal = false;

	// Check that the input path plus 3 is not longer than MAX_PATH: 3 characters for "\*" plus NULL appended below.
	StringCchLengthA(path, MAX_PATH, &pathLen);
	if (pathLen > (MAX_PATH - 3))
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	// Prepare string for use with FindFile functions: copy the string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path);
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		throw Exception("Error: failed finding files in path");
	}

	// checks all files in the directory in search for ".sboard" files.
	std::string currFileName;
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			currFileName = std::string(ffd.cFileName);
			if (Utilities::endsWith(currFileName, std::string(".sboard")))
			{
				*boardFile = new char[currFileName.size() + 1];
				strcpy_s(*boardFile, currFileName.size() + 1, ffd.cFileName);
				retVal = true;
				break;
			}
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
	return retVal;
}

/*
 * Deletes memory allocations of "board" including itself.
 */
void BoardCreator::freeBoard(char ** board, int numRows)
{
	for (int row = 0; row < numRows; row++)
	{
		delete[] board[row];
	}
	delete[] board;
}

/*
* Allocates memory for a "board", returns a pointer to it.
* User of this function should delete the memory allocated in it using "freeBoard"!
*/
char ** BoardCreator::createBoard(int numRows, int numCols)
{
	char** board = new char*[numRows];
	for (int i = 0; i < numRows; ++i)
	{
		board[i] = new char[numCols];
	}
	return board;
}

/*
* Creates a copy of "board" - Allocates memory for that copy. 
* User of this function should delete the memory allocated in it using "freeBoard"!
*/
char ** BoardCreator::copyBoard(const char ** board, int numRows, int numCols)
{
	char **retBoard = createBoard(numRows, numCols);
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++){
			retBoard[row][col] = board[row][col];
		}
	}
	return retBoard;
}
