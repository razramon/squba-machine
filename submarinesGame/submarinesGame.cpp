#include <windows.h>
#include "Ship.h"
#include "Sizes.h"
#include <iostream>
#include "Exception.h"
#include "Macros.h"
#include <fstream>
#include <vector>
#include <algorithm>

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
	} catch (std::exception& e) { //a 
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

int main(int argc, char* argv[])
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :)

	/*char** board= getBoardFromFile("good_board_0.sboard");

	deleteBoard(board);*/

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
		return 0;
	}

	if(!pathIsValid)
	{
		printNotFoundFileErrors(path.c_str(), boardFilePtr, attackFileAPtr, attackFileBPtr);
	}


	std::string fullPathToBoard = path +"\\"+ boardFilePtr;
	char** board = getBoardFromFile(fullPathToBoard.c_str());
	std::string fullPathToAttackFileA = path + "\\" + attackFileAPtr;
	std::string fullPathToAttackFileB = path + "\\" + attackFileBPtr;
	vector<pair<int, int>> attackFileA = getAttackFile(fullPathToAttackFileA.c_str());
	vector<pair<int, int>> attackFileB = getAttackFile(fullPathToAttackFileB.c_str());

	//prints board:
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (int j = 0; j < BOARD_LENGTH; ++j)
		{
			std::cout << board[i][j] << "\t";
		}
		std::cout << std::endl;
	}



	deleteBoard(board);

	if (pathIsValid)
	{
		delete[] boardFilePtr;
		delete[] attackFileAPtr;
		delete[] attackFileBPtr;
	}

	return 0;
}



void createShip(Ship * ship, char c)
{
	int shipPoints = 0;
	if ((shipPoints = Ship::pointsOfShip(c)) != -1)
	{
		int shipSize = 0;
		if ((shipSize = Ship::sizeOfShip(c)) != -1)
		{
			(*ship).setLetter(c);
			(*ship).setNumberOfPoints(shipPoints);
			(*ship).position = new int*[shipSize];
			for (int i = 0; i < shipSize; i++)
			{
				(*ship).position[i] = new int[3];
			}

		}
	}
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

void setBoard(char ** board, int numRows, int numCols)
{
	Ship shipA[NUMBER_SHIPS];
	Ship shipB[NUMBER_SHIPS];
	int indexShipA = 0;
	int indexShipB = 0;
	int indexInShip = 0;
	for(int indexRow = 0; indexRow < numRows; indexRow++)
	{
		for(int indexColumn = 0; indexColumn < numCols; indexColumn++)
		{
			char letter = board[indexRow][indexColumn];
			if(Ship::isShip(letter))
			{
				if (islower(letter))
				{
					createShip((&shipB[indexShipB]), letter);
					shipB[indexShipB].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}
				else
				{
					createShip((&shipA[indexShipA]), letter);
					shipA[indexShipA].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}

				int indexInShip = 1;
				int indexShip = indexColumn++;
				
				// Searching column for the rest of the ship
				while (board[indexRow][indexShip] == letter)
				{
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexRow, indexShip, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}

				indexShip = indexRow++;
				// Searching row for the rest of the ship
				while (board[indexShip][indexColumn] == letter)
				{	
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}
				if (islower(letter))
					indexShipB++;
				else
					indexShipA++;
				
			}
		}
	}
	printBoard(shipA);
	printBoard(shipB);

}

bool checkBoard(char ** board)
{
	Ship shipA[NUMBER_SHIPS];
	Ship shipB[NUMBER_SHIPS];
	int indexShipA = 0;
	int indexShipB = 0;
	int indexInShip = 0;
	for (int indexRow = 0; indexRow < BOARD_LENGTH - 1; indexRow++)
	{
		for (int indexColumn = 0; indexColumn < BOARD_LENGTH - 1; indexColumn++)
		{
			char letter = board[indexRow][indexColumn];
			if (Ship::isShip(letter))
			{
				if (islower(letter))
				{
					createShip((&shipB[indexShipB]), letter);
					shipB[indexShipB].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}
				else
				{
					createShip((&shipA[indexShipA]), letter);
					shipA[indexShipA].position[0] = new int[3]{ indexRow, indexColumn, 0 };
				}

				int indexInShip = 1;
				int indexShip = indexColumn++;

				// Searching column for the rest of the ship
				while (board[indexRow][indexShip] == letter)
				{
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexRow, indexShip, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}

				indexShip = indexRow++;
				// Searching row for the rest of the ship
				while (board[indexShip][indexColumn] == letter)
				{
					if (islower(letter))
						shipB[indexShipB].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					else
						shipA[indexShipA].position[indexInShip] = new int[3]{ indexShip, indexColumn, 0 };
					indexInShip++;
				}
				if (islower(letter))
					indexShipB++;
				else
					indexShipA++;

			}
		}
	}



	return false;//meital - added by me just to check main!

}


void game()
{
	int isHit = 0;
	int playerPlaying = 0;

	



}
