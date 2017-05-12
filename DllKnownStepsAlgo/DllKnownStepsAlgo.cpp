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

DllKnownStepsAlgo::DllKnownStepsAlgo() :rows(NOT_INITIALIZED), cols(NOT_INITIALIZED), 
										playerNum(NOT_INITIALIZED), attackNumber(NOT_INITIALIZED), attacks(nullptr)
{
}

DllKnownStepsAlgo::~DllKnownStepsAlgo()
{
	delete attacks;
}

void DllKnownStepsAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	if (board == nullptr) return;
	playerNum = player;
	rows = numRows;
	cols = numCols;
}


/* 
 * Called once to allow init from files. returns whether the init succeeded or failed.
 * Path is absolute path (makes sure that it is to a directory) in which the attack file supposed to be : http://moodle.tau.ac.il/mod/forum/discuss.php?d=52703
 */
bool DllKnownStepsAlgo::init(const std::string & path)
{
	//Checks that setBoard() was read before init(), and that path is a directory
	if (playerNum == NOT_INITIALIZED || (!(Utilities::doesPathExist(path.c_str())))) return false; 
	
	std::vector<std::string> attackFiles;
	if (!(Utilities::find2FilesWithSuf(path.c_str(), path.size(), attackFiles, Utilities::ATTACK_SUFF))) return false;
	
	//If gets here, attackFiles conatin 1-2 attack files found in path
	std::string pathToFile = path + "\\" + getFullPathToAttackFile(attackFiles);

	try
	{
		attacks = getAttackFile(pathToFile.c_str());
	}
	catch (std::exception &e)
	{
		//no use of e: getting the attack file failed.
		e.what();
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

std::string DllKnownStepsAlgo::getFullPathToAttackFile(const std::vector<std::string>& attackFiles) const
{
	if (attackFiles.size() == 1) return attackFiles.at(0);
	return (playerNum==PLAYER_A)? attackFiles.at(PLAYER_A):attackFiles.at(PLAYER_B);
}

IBattleshipGameAlgo* GetAlgorithm()
{
	IBattleshipGameAlgo* ptrToAlg = new DllKnownStepsAlgo();
	return ptrToAlg;
}