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
		throw Exception(exceptionInfo(WRONG_PATH,path)); //change thisto create a new exception
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
 *http://moodle.tau.ac.il/mod/forum/discuss.php?d=47695#p73943
 */
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
			if (line == "" || line.find(",") == std::string::npos || line.find_first_of(",") != line.find_last_of(","))
				continue;;
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
				badLetterIndexes.push_back(&std::pair<int, int>(currRow, currCol + i));
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
				badLetterIndexes.push_back(&std::pair<int, int>(currRow, col));
				board[currRow][col] = DEFAULT_LETTER;
				col++;
			}
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes"
			{
				badLetterIndexes.push_back(&std::pair<int, int>(currRow, currCol + i));
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
				badLetterIndexes.push_back(&std::pair<int, int>(currRow + i, currCol));
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
				badLetterIndexes.push_back(&std::pair<int, int>(row, currCol));
				board[row][currCol] = DEFAULT_LETTER;
				row++;
			}
			wrongSizeOrShape = true;
			for (int i = 0; i < shipCells; ++i)//insert all "bad indexes"
			{
				badLetterIndexes.push_back(&std::pair<int, int>(currRow + i, currCol));
			}
		}
	}
	else // Ship has only one (the current) cell
	{
		if (Ship::sizeOfShip(letter) != 1)
		{
			badLetterIndexes.push_back(&std::pair<int, int>(currRow, col));
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
 * Returns true if ship is next to another ship/"bad index" of the same letter (==wrong shape)
 * @Params:
 *		badLetterIndexes - a vector containing all indexes in which the letter appeard in, and weren't valid
 *		ship - the ship we're checking
 *		letter - the ship's letter
 */
bool checkShipShape(Ship* ship,char letter, std::vector<std::pair<int, int>*>& badLetterIndexes, std::vector<Ship>& shipsOfPlayer)
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
				|| ((col == (*badLetterIndexes.at(k)).second)&&(row - 1 == (*badLetterIndexes.at(k)).first || col + 1 == (*badLetterIndexes.at(k)).first)))
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
			if (!(pos[0][0]==shipsOfPlayer.at(i).getPosition()[0][0] && pos[0][1] == shipsOfPlayer.at(i).getPosition()[0][1])) //makes sure it's NOT the same ship
			{
				Ship* shipToCompare = &shipsOfPlayer.at(i);
				for (int j = 0; j < (*ship).getShipSize(); ++j)
				{
					row = pos[j][0];
					col = pos[j][1];
					for (int k = 0; k < (*shipToCompare).getShipSize(); ++k)
					{
						if (((row == (*shipToCompare).getPosition()[k][0]) && (col - 1 == (*shipToCompare).getPosition()[k][1] || col + 1 == (*shipToCompare).getPosition()[k][1]))
							|| ((col == (*shipToCompare).getPosition()[k][1]) && (row - 1 == (*shipToCompare).getPosition()[k][0] || col + 1 == (*shipToCompare).getPosition()[k][0])))
						{
							res = true;
							break;
						}
					}
					if (res) break;
				}
			}
			if(res)	break;
		}
	}
	if (res)
	{
		for (int i = 0; i < Ship::sizeOfShip(letter); ++i)
		{
			std::pair<int, int> badindex(pos[i][0], pos[i][1]);
			badLetterIndexes.push_back(&badindex);
		}
	}
	return res;
}

std::pair <std::vector<Ship*>,std::vector<Ship*>>* checkBoard(char ** board, int numRows, int numCols)
{
	std::vector<Ship*> shipsA;
	std::vector<Ship*> shipsB;
	std::vector<std::pair<int, int>*>* badLetterIndexes_B = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_b = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_P = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_p = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_M = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_m = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_D = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes_d = new std::vector<std::pair<int, int>*>;
	std::vector<std::pair<int, int>*>* badLetterIndexes = badLetterIndexes_B;//Initialized to an arbitrary "badLetterIndex" in order to prevent error.
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
	//check for wrong size or shape(in near columns/rows)
	
	
	
	//TODO:: add here all neighbour checks: for:
	//2. adjacent ships

	if (wrongSizeShapeShips.size() > 0)
	{
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
	
	delete badLetterIndexes_B;
	delete badLetterIndexes_b;
	delete badLetterIndexes_P;
	delete badLetterIndexes_p;
	delete badLetterIndexes_M;
	delete badLetterIndexes_m;
	delete badLetterIndexes_D;
	delete badLetterIndexes_d;

	std::pair<std::vector<Ship*>, std::vector<Ship*>>* ret = new std::pair<std::vector<Ship*>, std::vector<Ship*>>(shipsA, shipsB);
	return ret;
}


void game(Player* playerA, Player* playerB)
{
	int isHitA = 0;
	int isHitB = 0;
	int playerPlaying = 0;
	//TODO: add points checker for the players
	while ((*playerA).attackNumber != -1 || (*playerB).attackNumber != -1)
	{
		pair<int, int> attack;
		AttackResult result = AttackResult::Miss;
		// Inside player A and there are attacks left
		if (playerPlaying == (*playerA).playerNum && (*playerA).attackNumber != -1)
			attack = (*playerA).attack();
		// Inside player B and there are attacks left
		else if (playerPlaying == (*playerB).playerNum && (*playerB).attackNumber != -1)
			attack = (*playerB).attack();

		isHitA = (*playerA).isHit(attack.first, attack.second);
		isHitB = (*playerB).isHit(attack.first, attack.second);

		// Hit before
		if (isHitA == 3 || isHitB == 3)
		{
			// Send hit, change playerPlaying
			result = AttackResult::Hit;
			playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;

		}
		// Sink
		else if (isHitA == 2 || isHitB == 2)
		{
			result = AttackResult::Sink;
			// Self sink and change playerPlaying to the other
			if ((isHitA == 2 && playerPlaying == (*playerA).playerNum) || (isHitB == 2 && playerPlaying == (*playerB).playerNum))
			{
				playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
			}
			std::cout << std::to_string(attack.first) + "," + std::to_string(attack.second);
			std::cout << " is dead" << std::endl;

		}
		// Hit 
		else if (isHitA == 1 || isHitB == 1)
		{
			result = AttackResult::Hit;
			// Self hit and change playerPlaying to the other
			if ((isHitA == 1 && playerPlaying == (*playerA).playerNum) || (isHitB == 1 && playerPlaying == (*playerB).playerNum))
			{
				playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
			}
			std::cout << std::to_string(attack.first) + "," + std::to_string(attack.second);
			std::cout << " is hit" << std::endl;
		}
		// Miss
		else
		{
			// Change playerPlaying
			playerPlaying = playerPlaying == (*playerB).playerNum ? (*playerA).playerNum : (*playerB).playerNum;
		}

		// Notify players on the result
		(*playerA).notifyOnAttackResult(playerPlaying, attack.first, attack.second, result);
		(*playerB).notifyOnAttackResult(playerPlaying, attack.first, attack.second, result);
	}
}


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :)


	char b0[5] = { ' ','P','P','P',' ' };
	char b1[5] = { ' ',' ','P','P',' ' };
	char b2[5] = { 'M','d',' ',' ',' ' };
	char b3[5] = { 'M',' ',' ',' ','B' };
	char b4[5] = { 'M',' ',' ',' ',' ' };
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

	std::pair<std::vector<Ship*>, std::vector<Ship*>>* playersShips = checkBoard(b, 5, 5);

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

	std::string path;
	if (argc==1)
	{
		path = workingDirectory();
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
	std::string fullPathToAttackFileB = path + "\\" + attackFileBPtr;
	vector<pair<int, int>> attackFileA = getAttackFile(fullPathToAttackFileA.c_str());
	vector<pair<int, int>> attackFileB = getAttackFile(fullPathToAttackFileB.c_str());

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
	std::cout << "Player A's ships are:" << std::endl;
	for (int i = 0; i < (*playersShips).first.size(); ++i)
	{
		std::cout << "ship of type: " << (*((*playersShips).first.at(i))).getLetter() <<
			"\n in position: " << std::endl;
		int** pos = (*((*playersShips).first.at(i))).getPosition();
		for (int k = 0; k < (*((*playersShips).first.at(i))).getShipSize(); ++k)
		{
			std::cout << "(" << pos[k][0] << "," << pos[k][1] << ")  ,  ";
		}
		std::cout << std::endl;
	}
	
	//deletes ships to free allocated space(?)
	(*playersShips).first.clear();
	(*playersShips).second.clear();
	delete playersShips;


	// Creating new player for the test of the game
	Player *playerA = new Player();
	playerA->attacks = attackFileA;
	Ship *s1 = new Ship('B');
	s1->setPosition(0, 1, 4, 0);
	playerA->ships[0] = *s1;

	Ship *s2 = new Ship('P');
	s2->setPosition(0, 2, 8, 0);
	s2->setPosition(1, 2, 9, 0);
	playerA->ships[1] = *s2;

	Ship *s3 = new Ship('M');
	s3->setPosition(0, 4, 6, 0);
	s3->setPosition(1, 4, 7, 0);
	s3->setPosition(2, 4, 8, 0);
	playerA->ships[2] = *s3;

	Ship *s4 = new Ship('B');
	(*s4).setPosition(0, 9, 1, 0);
	playerA->ships[3] = *s4;

	Ship *s5 = new Ship('P');
	(*s5).setPosition(0, 10, 9, 0);
	(*s5).setPosition(1, 10, 10, 0);
	playerA->ships[4] = *s5;
	playerA->playerNum = 0;

	int** pos = (*s2).getPosition();
	for (int k = 0; k < (*s2).getShipSize(); ++k)
	{
		std::cout << "(" << pos[k][0] << "," << pos[k][1] << ")  ,  ";
	}
	std::cout << std::endl;



	Player *playerB = new Player();
	playerB->attacks = attackFileB;
	playerB->playerNum = 1;
	s1 = new Ship('b');
	s1->setPosition(0, 10, 4, 0);
	playerB->ships[2] = *s1;

	s2 = new Ship('p');
	s2->setPosition(0, 2, 2, 0);
	s2->setPosition(1, 3, 2, 0);
	playerB->ships[0] = *s2;

	s3 = new Ship('m');
	s3->setPosition(0, 5, 1, 0);
	s3->setPosition(1, 6, 1, 0);
	s3->setPosition(2, 7, 1, 0);
	playerB->ships[1] = *s3;

	s4 = new Ship('b');
	(*s4).setPosition(0, 6, 10, 0);
	playerA->ships[3] = *s4;

	s5 = new Ship('d');
	s5->setPosition(0, 5, 4, 0);
	s5->setPosition(1, 6, 4, 0);
	s5->setPosition(2, 7, 4, 0);
	s5->setPosition(3, 8, 4, 0);
	playerA->ships[4] = *s5;

	game(playerA, playerB);
	return 0;
}
