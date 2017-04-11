#include <windows.h>
#include "Ship.h"
#include "Sizes.h"
#include <iostream>
#include "Exception.h"
#include "Macros.h"
#include <vector>
#include <fstream>

using namespace std;
static const char DEFAULT_LETTER='a';
static const std::string ATTACK_A_SUFF = ".attack-a";
static const std::string ATTACK_B_SUFF = ".attack-b";
static const std::string BOARD_SUFF = ".sboard";

//returns true if the path exists && it's a DIRECTORY
static bool doesPathExist(const char* path)
{
	DWORD dirAttr = GetFileAttributesA(path);

	if (dirAttr == INVALID_FILE_ATTRIBUTES)
	{
		throw Exception(exceptionInfo(WRONG_PATH,path)); //change thisto create a new exception
	} 
	return ((dirAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

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

//returns a path to (this) working directory
//NEEDS A SMALL FIX!!!
static std::string workingDirectory() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}

/* Checks if a given fileName ends with ATTACK_A_SUFF/ATTACK_B_SUFF/BOARD_SUFF, 
 * if it does && the according attackFileA/attackFileB/boardFile is NOT initiallized,
 * initializes it.
 * USER OF THIS FUNCTION HAS TO DELETE WHAT'S CREATED IN IT!!
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


//checks if the path exists and that the 3 files are there, if they are - updates their names
static bool isValidPath(const char* path, char* boardFile, char* attackFileA, char* attackFileB)
{
	bool doesExist;
	try
	{
		doesExist = doesPathExist(path);
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return false;
	}
	if (!doesExist) //meaning: path exist, but it's not a directory (it's a file)
	{
		std::cout << Exception(exceptionInfo(WRONG_PATH, path)).what() << std::endl;
		return false;
	}

	//creating a file conataining all files in "path" 
	std::string command = string(path);
	command.insert(0, "dir ");
	command.append("/b /a-d > file_names.txt");
	std::cout << "command is: " << command << std::endl;
	system(command.c_str());

	//reads all file names from the file created
	std::string fn = workingDirectory();
	fn.append("\\file_names.txt");
	std::ifstream file(fn.c_str());
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string line;
	try
	{
		while (getline(file, line))
		{
			checkFileName(line, &boardFile, &attackFileA, &attackFileB);
		}
	} catch (std::ifstream::failure e)
	{
		std::cout << "Error: could not open/read file_names.txt, error is: " << e.what() << std::endl;
		return false;
	}

	//making sure everything's initiallized
	if (boardFile == nullptr || attackFileA == nullptr || attackFileB == nullptr) {
		return false;
	}
	
	return true;
}

/*
 *Used when cruitiall files are missing, frees memory of files who where found,
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

int main(int argc, char* argv[])
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :)

	std::string path;
	if (argc==1)
	{
		path = workingDirectory();
	} else
	{
		path = argv[1];
	}

	//char* pathToCheck = new char[MAX_PATH];
	//std::cout << "_pgmptr is: " << _get_pgmptr(&pathToCheck) << std::endl;

	char* boardFilePtr = nullptr;
	char* attackFileAPtr = nullptr;
	char* attackFileBPtr = nullptr;

	std::cout << "path is: " << path << std::endl;
	
	bool pathIsValid = isValidPath(path.c_str(), boardFilePtr, attackFileAPtr, attackFileBPtr);

	std::cout << "answer is: " << pathIsValid << std::endl;

	if(!pathIsValid)
	{
		printNotFoundFileErrors(path.c_str(), boardFilePtr, attackFileAPtr, attackFileBPtr);
	}

	/*
	std::string filen = "filey.attack_b";
	checkFileName(filen, &boardFilePtr, &attackFileAPtr, &attackFileBPtr);

	std::cout << "boardFilePtr is: " << ((boardFilePtr==nullptr)? "nullptr": boardFilePtr) << std::endl;
	std::cout << "attackFileAPtr is: " << ((attackFileAPtr == nullptr) ? "nullptr" : attackFileAPtr) << std::endl;
	std::cout << "attackFileBPtr is: " << ((attackFileBPtr == nullptr) ? "nullptr" : attackFileBPtr) << std::endl;
	if (boardFilePtr !=nullptr) delete boardFilePtr; //clear memory
	if (attackFileAPtr != nullptr) delete attackFileAPtr; //clear memory
	if (attackFileBPtr != nullptr) delete attackFileBPtr; //clear memory
	*/
	return 0;
}
bool isShip(char c)
{
	c = tolower(c);
	return (c == 'm' || c == 'b' || c == 'd' || c == 'p') ? true : false;
}
int pointsOfShip(char c)
{
	c = tolower(c);
	switch (c)
	{
	case 'b': return B_POINTS;

	case 'p': return P_POINTS;

	case 'm': return M_POINTS;

	case 'd': return D_POINTS;

	default: return -1;
	}
}
int sizeOfShip(char c)
{
	c = tolower(c);
	switch (c)
	{
	case 'b': return B_SIZE;

	case 'p': return P_SIZE;

	case 'm': return M_SIZE;

	case 'd': return D_SIZE;

	default: return -1;
	}
}
void createShip(Ship * ship, char c)
{
	int shipPoints = 0;
	if ((shipPoints = pointsOfShip(c)) != -1)
	{
		int shipSize = 0;
		if ((shipSize = sizeOfShip(c)) != -1)
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
			if(isShip(letter))
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
			if (isShip(letter))
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

bool isHit(int row, int col, Player player)
{
	for()


}

void game()
{
	int isHit = 0;
	int playerPlaying = 0;

	



}
