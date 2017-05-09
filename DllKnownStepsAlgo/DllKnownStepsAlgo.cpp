#include "DllKnownStepsAlgo.h"


const int DllKnownStepsAlgo::NOT_INITIALIZED = -1;

void DllKnownStepsAlgo::getAttackFromLine(std::string& line, std::pair<int,int>& attack)
{
	// Getting each number, deleting spaces and turning to integer
	std::string temp = line.substr(0, line.find(","));
	std::string row = Utilities::delSpaces(temp);
	temp.clear();
	temp = line.substr(line.find(",") + 1, line.length());
	std::string col = Utilities::delSpaces(temp);

	attack.first = stoi(row);
	attack.second = stoi(col);
}

std::vector<std::pair<int, int>>* DllKnownStepsAlgo::getAttackFile(const char * attackFile)
{
	//opening attackFile:
	std::ifstream bfile(attackFile);
	if (!bfile) {
		bfile.close();
		throw Exception("Error: failed opening attack file.");
	}
	std::vector<std::pair<int, int>>* attacksVector = new std::vector<std::pair<int, int>>();

	std::pair<int, int> attack;
	std::string line;
	while (!bfile.eof())
	{
		try
		{
			Utilities::getAllKindsOfLine(bfile, line);
			if (line == "" || line.find(",") == std::string::npos || line.find_first_of(",") != line.find_last_of(","))
				continue;
		}
		catch (std::exception& e)
		{
			delete attacksVector;
			std::string s("Error: failed when reading attack file: ");
			s.append(e.what());
			bfile.close();
			throw Exception(s.c_str());
		}
		//updates attack to contain the positions that "line" holds:
		getAttackFromLine(line, attack);

		// Check if in range
		if (attack.first > 10 || attack.first < 1 || attack.second > 10 || attack.second < 1)
		{
			continue;
		}
		(*attacksVector).push_back(attack);
	}
	bfile.close();
	return attacksVector;
}

DllKnownStepsAlgo::DllKnownStepsAlgo() :attacks(nullptr), playerNum(NOT_INITIALIZED), 
										rows(NOT_INITIALIZED), cols(NOT_INITIALIZED), attackNumber(NOT_INITIALIZED)
{
}

DllKnownStepsAlgo::~DllKnownStepsAlgo()
{
}

void DllKnownStepsAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	if (board == nullptr) return;
	playerNum = player;
	rows = numRows;
	cols = numCols;
}


/* 
 * Called once to allow init from files if needed returns whether the init succeeded or failed.
 * Path is absolute path (makes sure that it is to a directory) in which the attack file supposed to be : http://moodle.tau.ac.il/mod/forum/discuss.php?d=52703
 */
bool DllKnownStepsAlgo::init(const std::string & path)
{
	//Checks that setBoard() was read before init(), and that path is a directory
	if (playerNum == NOT_INITIALIZED || (!(Utilities::doesPathExist(path.c_str())))) return false; 
	
	std::vector<std::string> attackFiles;
	if (!(findAttackFile(path.c_str(), path.size(), attackFiles))) return false;
	
	//If gets here, attackFiles conatin 1-2 attack files found in path
	std::string pathToFile = path + "\\" + getFullPathToAttackFile(attackFiles);

	try
	{
		attacks = getAttackFile(pathToFile.c_str());
	}
	catch (std::exception &e)
	{
		//no use of e: getting the attack file failed.
		return false;
	}
	attackNumber = 0;
	return true;
}

// ask player for his move
std::pair<int, int> DllKnownStepsAlgo::attack()
{
	if (attackNumber < 0 || attackNumber >= (*attacks).size())
	{
		return std::pair<int, int>(-1, -1);
	}
	++attackNumber;
	if (attackNumber == (*attacks).size())
	{
		attackNumber = -1;
		return ((*attacks).at(((*attacks).size()) - 1));
	}
	return ((*attacks).at(attackNumber - 1));
}

// notify on last move result
void DllKnownStepsAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	//DOES NOTHING - because player from a file doesn't do anything with that information.
}

IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllKnownStepsAlgo();
	return ptrToAlg;
}

/* TODO:: check this function
* Gets a valid path (directory), updates "attackFiles" to contain 2 (at most) attack files - oredered lexicographically
* Returns: true if succeeded (found 1 or 2 attack files in path which are inside "attackFiles" vector
*			false if an error accured / no files have been found.
*/
bool DllKnownStepsAlgo::findAttackFile(const char* path, size_t pathLen, std::vector<std::string>& attackFiles)
{
	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.
	StringCchLengthA(path, MAX_PATH, &pathLen);
	if (pathLen > (MAX_PATH - 3))
	{
		return false;
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path);
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return false;
	}

	// checks all files in the directory in search for ".attack" file.
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			if (Utilities::endsWith(std::string(ffd.cFileName), std::string(Utilities::ATTACK_SUFF)))
			{
				addAttackFileToList(attackFiles, std::string(ffd.cFileName));
			}
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	if (GetLastError() != ERROR_NO_MORE_FILES)
	{
		return false;
	}
	FindClose(hFind);
	return (0 < attackFiles.size() && attackFiles.size() <= 2);
}

std::string DllKnownStepsAlgo::getFullPathToAttackFile(const std::vector<std::string>& attackFiles) const
{
	if (attackFiles.size() == 1) return attackFiles.at(0);
	return (playerNum==PLAYER_A)? attackFiles.at(PLAYER_A):attackFiles.at(PLAYER_B);
}

/*
* Maintains "attackFiles" to hold at most 2 attack files,
* The ones which are first lexicographically.
*/
void DllKnownStepsAlgo::addAttackFileToList(std::vector<std::string>& attackFiles, std::string filename)
{
	if (attackFiles.size() == 0)
	{
		attackFiles.push_back(filename);
		return;
	}
	std::vector<std::string>::iterator iter;
	for (iter = attackFiles.begin(); iter != attackFiles.end(); ++iter)
	{
		if (*iter >= filename)
		{
			attackFiles.insert(iter, filename);
			if (attackFiles.size() > 2) //deletes the "largest" file (lexicographically)
			{
				attackFiles.pop_back();
			}
			break; //To avoid inserting "filename" twice.
		}
	}
	if (attackFiles.size() < 2) //means: there was only 1 file in "attackFiles" which was smaller (lexicographically) than "filename"
	{
		attackFiles.push_back(filename);
	}
}