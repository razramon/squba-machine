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

/*
 *Returns a path to (this) working directory
 */
//TODO: NEEDS A SMALL FIX - wait for amir's answer:http://moodle.tau.ac.il/mod/forum/discuss.php?d=49474
//http://moodle.tau.ac.il/mod/forum/discuss.php?d=47695#p73943
static std::string workingDirectory() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
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

///*
// * Returns path s.t. it won't contain the last 2 directories
// * in the passed path.
// * NOT FINISHED: because it's relevant to check if it's \ or /...
// */
//std::string getPath(std::string path)
//{
//	std::string str = path;
//	for (int i = 0; i < 2; ++i)
//	{
//		str = str.
//	}
//}


/*Checks if the path exists,
 *that the 3 files are there, 
 *And if they are - updates their names */
static bool isValidPath(const char* path, char** boardFile, char** attackFileA, char** attackFileB)
{
	bool doesExist;
	try
	{
		doesExist = doesPathExist(path);
	} catch (std::exception& e) {  
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}
	if (!doesExist) //meaning: path exist, but it's not a directory (it's a file)
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	//creating a file conataining all files in "path" 
	std::string command = string(path);
	command.insert(0, "dir \"");
	command.append("\" /b /a-d > file_names.txt");
//	std::cout << "command is: " << command << std::endl;
	system(command.c_str());

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
 * Returns the current stream position,
 * Updates the current line into "line".
 */
std::istream& getAllKindsOfLine(std::istream& inputStream, std::string& line)
{
	line.clear();
	try
	{
		std::istream::sentry se(inputStream, true);
		std::streambuf* sb = inputStream.rdbuf();
		int c;
		while (true) {
			c = sb->std::streambuf::sbumpc();
			switch (c) {
			case '\n':
				return inputStream;
			case '\r':
				if (sb->std::streambuf::sgetc() == '\n')
				{
					sb->std::streambuf::sbumpc();
				}
				return inputStream;
			case EOF:
				if (line.empty()) //last line of the file has no line terminator
				{
					inputStream.setstate(std::ios::eofbit);
				}
				return inputStream;
			default:
				line += static_cast<char>(c);
			}
		}

	} catch (std::exception& e)
	{
		throw Exception("Error: failed reading line.");
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
	for (int row = 0; row < BOARD_LENGTH; ++row)
	{
		//reads 1 line from the file:
		try
		{
			getAllKindsOfLine(bfile, line);
		} catch (std::exception& e)
		{
			deleteBoard(board);
			if (bfile.eof())
			{
				throw Exception("Error: board file is too small");
			}
			throw Exception("Error: failed reading from board file.");
		}
		if (line.length()!= BOARD_LENGTH)
		{
			deleteBoard(board);
			std::string er("Error: board file in not of right format: line ");
			er.append(std::to_string(row));
			er.append(" is too small");
			const char* erC = er.c_str();
			throw Exception(erC);
		}
		for (int col = 0; col < BOARD_LENGTH; ++col)
		{
			board[row][col] = line.at(col);
		}
		
	}
	return board;
}

string delSpaces(string &str)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
	return str;
}

vector<pair<int, int>> getAttackFile(const char* attackFile)
{
	vector<pair<int, int>> attacks;

	//opening attackFile:
	std::ifstream bfile(attackFile);
	if (!bfile) {
		throw Exception("Error: failed opening attack file.");
	}
	string line;
	while(!bfile.eof())
	{
		try
		{
			getAllKindsOfLine(bfile, line);
			if (line == "")
				break;
		}
		catch (std::exception& e)
		{
			//TODO:: print the relevant messege. this one is temporary
			std::cout << e.what() << std::endl;
		}
		
		// Getting each number, deleting spaces and turning to integer
		string row = delSpaces(line.substr(0, line.find(",")));
		string col = delSpaces(line.substr(line.find(",") + 1, line.length()));
		pair<int, int> attack;

		attack.first = stoi(row);
		attack.second = stoi(col);

		// Check if in range
		if(attack.first > 10 || attack.first < 1 || attack.second > 10 || attack.second < 1)
		{
			continue;
		}
		attacks.push_back(attack);

	}

	return attacks;

	
}

/*
*Returns true if there IS an adjacent (SAME kind of) ship.
*TO BE USED ONLY AFTER USING "checkNeighbourShips1".
*/
bool checkNeighbourShips2(char ** board, int currentRow, int currentCol, int numRows, int numCols)
{

	if ((currentRow - 1 >= 0) && (currentCol - 1 >= 0))
	{
		if (board[currentRow - 1][currentCol - 1] == board[currentRow][currentCol])
			return true;
	}

	if ((currentRow - 1 >= 0) && (currentCol + 1 < numCols))
	{
		if (board[currentRow - 1][currentCol + 1] == board[currentRow][currentCol])
			return true;
	}

	if ((currentRow + 1 < numRows) && (currentCol - 1 >= 0))
	{
		if (board[currentRow + 1][currentCol - 1] == board[currentRow][currentCol])
			return true;
	}

	if ((currentRow + 1 < numRows) && (currentCol + 1 < numCols))
	{
		if (board[currentRow + 1][currentCol + 1] == board[currentRow][currentCol])
			return true;
	}

	return false;
}

/*
*Returns true if there IS an adjacent (different) ship.
*/
bool checkNeighbourShips1(char ** board, int currentRow, int currentCol, int numRows, int numCols)
{
	int lowerRowBound = max(currentRow - 1, 0);
	int upperRowBound = min(currentRow + 1, numRows - 1);
	int lowerColBound = max(currentCol - 1, 0);
	int upperColBound = min(currentCol + 1, numCols - 1);

	for (int i = lowerRowBound; i <= upperRowBound; ++i)
	{
		for (int j = lowerColBound; j <= upperColBound; ++j)
		{
			if ((i != currentRow) || (j != currentCol))
			{
				if (Ship::isShip(board[i][j]) && (board[i][j] != board[currentRow][currentCol])) {
					return true;
				}
			}
		}
	}
	return false;
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
				while (ships[indexShip].position[indexInShip] != NULL)
				{
					// Checking if there is a match in ship position
					if (indexRow == ships[indexShip].position[indexInShip][0] &&
						indexColumn == ships[indexShip].position[indexInShip][1])
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
	char letter, int& numShipsForCurrPlayer, std::vector<Ship>& shipsOfPlayer,
	bool& wrongSizeOrShape, std::vector<std::pair<int, int>>& badLetterIndexes)
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
				badLetterIndexes.push_back(std::pair<int, int>(currRow, currCol + i));
			}
			return;
		}
		//If gets here,shipCells == Ship::sizeOfShip(letter):

		if (((col < numCols - 1) && board[currRow][col] != letter) || (col == numCols - 1)) //Makes sure the ship is of right size (not too large)
		{
			Ship* ship =new Ship(letter);
			shipsOfPlayer.push_back(*ship);
			for (int i = 0; i < shipCells; ++i)
			{
				shipsOfPlayer[numShipsForCurrPlayer].position[i] = new int[3]{ currRow, currCol + i, 0 };
			}
			numShipsForCurrPlayer++;
		}
		else //wrong size - ship's too big
		{
			while ((col <= numCols - 1) && (board[currRow][col]) == letter) //turns all letters to the default one
			{
				badLetterIndexes.push_back(std::pair<int, int>(currRow, col));
				board[currRow][col] = DEFAULT_LETTER;
				col++;
			}
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes"
			{
				badLetterIndexes.push_back(std::pair<int, int>(currRow, currCol + i));
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
				badLetterIndexes.push_back(std::pair<int, int>(currRow + i, currCol));
			}
			return;
		}
		//If gets here,shipCells == Ship::sizeOfShip(letter):
		if (((row < numRows - 1) && board[row][currCol] != letter) || (row == numRows - 1)) //Makes sure the ship is of right size (not too large)
		{
			Ship* ship = new Ship(letter);
			shipsOfPlayer.push_back(*ship);
			for (int i = 0; i < shipCells; ++i)
			{
				shipsOfPlayer[numShipsForCurrPlayer].position[i] = new int[3]{ currRow + i, currCol, 0 };
			}
			numShipsForCurrPlayer++;
		}
		else //wrong size - ship's too big
		{
			while ((row <= numRows - 1) && (board[row][currCol]) == letter) //turns all letter to the default one
			{
				badLetterIndexes.push_back(std::pair<int, int>(row, currCol));
				board[row][currCol] = DEFAULT_LETTER;
				row++;
			}
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes"
			{
				badLetterIndexes.push_back(std::pair<int, int>(currRow + i, currCol));
			}
		}
	}
	else // Ship has only one (the current) cell
	{
		if (Ship::sizeOfShip(letter) != 1)
		{
			badLetterIndexes.push_back(std::pair<int, int>(currRow, col));
			wrongSizeOrShape = true;
			return;
		}
		else
		{
			Ship* ship = new Ship(letter);
			shipsOfPlayer.push_back(*ship);
			shipsOfPlayer[numShipsForCurrPlayer].position[0] = new int[3]{ currRow, currCol, 0 };
			numShipsForCurrPlayer++;
		}
	}
}


std::pair <std::vector<Ship>,std::vector<Ship>> checkBoard(char ** board, int numRows, int numCols)
{
	std::vector<Ship> shipsA;
	std::vector<Ship> shipsB;
	std::vector<std::pair<int, int>> badLetterIndexes_B;
	std::vector<std::pair<int, int>> badLetterIndexes_b;
	std::vector<std::pair<int, int>> badLetterIndexes_P;
	std::vector<std::pair<int, int>> badLetterIndexes_p;
	std::vector<std::pair<int, int>> badLetterIndexes_M;
	std::vector<std::pair<int, int>> badLetterIndexes_m;
	std::vector<std::pair<int, int>> badLetterIndexes_D;
	std::vector<std::pair<int, int>> badLetterIndexes_d;
	std::vector<std::pair<int, int>>* badLetterIndexes = &badLetterIndexes_B;//Initialized to an arbitrary "badLetterIndex" in order to prevent error.
	std::set<char>  wrongSizeShapeShips;

	int indexShipA = 0;
	int indexShipB = 0;

	bool wrongSizeOrShape;
	bool adjacentShips = false;
	char letter = '`';
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			wrongSizeOrShape = false;
			letter = board[row][col];
			switch (letter)
			{
			case 'B':badLetterIndexes = &badLetterIndexes_B;
				break;
			case 'b':badLetterIndexes = &badLetterIndexes_b;
				break;
			case 'P':badLetterIndexes = &badLetterIndexes_P;
				break;
			case 'p':badLetterIndexes = &badLetterIndexes_p;
				break;
			case 'M':badLetterIndexes = &badLetterIndexes_M;
				break;
			case 'm':badLetterIndexes = &badLetterIndexes_m;
				break;
			case 'D':badLetterIndexes = &badLetterIndexes_D;
				break;
			case 'd':badLetterIndexes = &badLetterIndexes_d;
				break;
			default: //TODO:: throw exception or something, never supposed to get here anyway
				break;
			}
			if (Ship::isShip(letter)) {
				if (islower(letter))//letter is "player B"'s letter (small letters)
				{
					checkShipBorders(board, numRows, numCols, row, col, letter, indexShipB, shipsB, wrongSizeOrShape, *badLetterIndexes);
				} else //"player A"'s letter
				{
					checkShipBorders(board, numRows, numCols, row, col, letter, indexShipA, shipsA, wrongSizeOrShape, *badLetterIndexes);
				}
				if (wrongSizeOrShape)
				{
					wrongSizeShapeShips.insert(letter);
				}
			}
		}
	}
	return std::pair<std::vector<Ship>, std::vector<Ship>>(shipsA, shipsB);
}


int main(int argc, char* argv[])
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :)


	char b0[5] = { ' ',' ',' ',' ',' ' };
	char b1[5] = { ' ',' ','k','P','Q' };
	char b2[5] = { 'd','d','d','d','P' };
	char b3[5] = { ' ',' ',' ',' ','P' };
	char b4[5] = { 'm','m','m','m','m' };
	char* b[5] = { b0, b1,b2,b3,b4 };

	//prints board:
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			std::cout << b[i][j] << "\t";
		}
		std::cout << std::endl;
	}

	//std::pair<std::vector<Ship>, std::vector<Ship>> playersShips = checkBoard(b, 5, 5);


	int numShipsForCurrPlayer = 0;
	std::vector<Ship> shipsOfPlayer;
	bool wrongSizeOrShape = false;
	std::vector<std::pair<int, int>> badLetterIndexes;
	checkShipBorders(b, 5, 5, 2, 0, 'd', numShipsForCurrPlayer, shipsOfPlayer, wrongSizeOrShape, badLetterIndexes);


	std::cout << "numShipsForCurrPlayer is: " << numShipsForCurrPlayer << std::endl;
	std::cout << "shipsOfPlayer size is: " << shipsOfPlayer.size() << std::endl;
	std::cout << "wrongSizeOrShape is: " << (wrongSizeOrShape ? "true":"false") << std::endl;
	for (int i = 0; i < badLetterIndexes.size(); ++i)
	{
		std::cout << "badLetterIndexes in index: " << i << " is: " << badLetterIndexes[i].first << "," << badLetterIndexes[i].second << std::endl;
	}

	shipsOfPlayer.clear();

	//prints board:
	std::cout << "********PRINTING BOARD AFTER CHANGE********" << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			std::cout << b[i][j] << "\t";
		}
		std::cout << std::endl;
	}

	///*char** board= getBoardFromFile("good_board_0.sboard");

	//deleteBoard(board);*/

	//std::string path;
	//if (argc==1)
	//{
	//	path = workingDirectory();
	//} else
	//{
	//	path = argv[1];
	//}

	//char* boardFilePtr = nullptr;
	//char* attackFileAPtr = nullptr;
	//char* attackFileBPtr = nullptr;

	//bool pathIsValid = false;
	////std::cout << "path is: " << path << std::endl;
	//try
	//{
	//	pathIsValid = isValidPath(path.c_str(), &boardFilePtr, &attackFileAPtr, &attackFileBPtr);
	//} catch (std::exception& e)
	//{
	//	std::cout << e.what() << std::endl;
	//}

	//if(!pathIsValid)
	//{
	//	printNotFoundFileErrors(path.c_str(), boardFilePtr, attackFileAPtr, attackFileBPtr);
	//	return 1;
	//}


	//std::string fullPathToBoard = path +"\\"+ boardFilePtr;
	//char** board = getBoardFromFile(fullPathToBoard.c_str());
	//std::string fullPathToAttackFileA = path + "\\" + attackFileAPtr;
	//std::string fullPathToAttackFileB = path + "\\" + attackFileBPtr;
	//vector<pair<int, int>> attackFileA = getAttackFile(fullPathToAttackFileA.c_str());
	//vector<pair<int, int>> attackFileB = getAttackFile(fullPathToAttackFileB.c_str());

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

	//if (pathIsValid)
	//{
	//	delete[] boardFilePtr;
	//	delete[] attackFileAPtr;
	//	delete[] attackFileBPtr;
	//}

	return 0;
}
