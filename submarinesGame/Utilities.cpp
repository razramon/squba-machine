#include "Utilities.h"

const std::string Utilities::ATTACK_SUFF = ".attack";

Utilities::Utilities()
{
}

Utilities::~Utilities()
{
}


/*
*Returns true if path exists && it's a directory
*/
bool Utilities::doesPathExist(const char* path)
{
	DWORD dirAttr = GetFileAttributesA(path);
	if (dirAttr == INVALID_FILE_ATTRIBUTES)
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}
	return ((dirAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

/*
*Returns true if "filename" ends with "suffix"
*/
bool Utilities::endsWith(const std::string& fileName, const std::string& suffix)
{
	if (fileName.length() < suffix.length())
	{
		return false;
	}
	if (suffix.length() == 0) return true; //if suffix is the empty string returns true
	size_t fln = fileName.length();
	size_t sln = suffix.length();
	for (int i = 0; i < sln; ++i)
	{
		if (suffix.at(i) != fileName.at(fln - sln + i))
		{
			return false;
		}
	}
	return true;
}

/*
* Returns the current stream position,
* Updates the current line into "line".
*/
std::istream& Utilities::getAllKindsOfLine(std::istream& inputStream, std::string& line)
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

	}
	catch (std::exception& e)
	{
		std::string s= "Error: failed reading line: ";
		s.append(e.what());
		throw Exception(s.c_str());
	}
}

std::string Utilities::delSpaces(std::string &str)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
	return str;
}

/*
*Returns a path to (this) working directory - it's where the .exe file is...
*http://moodle.tau.ac.il/mod/forum/discuss.php?d=47695#p73943
*/
std::string Utilities::workingDirectory() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}


///* Checks if a given fileName ends with ATTACK_A_SUFF/ATTACK_B_SUFF/BOARD_SUFF,
//* if it does && the according attackFileA/attackFileB/boardFile is NOT initiallized,
//* initializes it.
//* USER OF THIS FUNCTION HAVE TO FREE MEMORY THAT HAS BEEN ALLOCATED IN IT!!
//*  */
//void Utilities::checkFileName(const std::string& fileName, char** boardFile, char** attackFileA, char** attackFileB)
//{
//	if (Utilities::endsWith(fileName, ATTACK_A_SUFF) && *attackFileA == nullptr)
//	{
//		*attackFileA = new char[fileName.length() + 1];
//		strcpy_s(*attackFileA, fileName.length() + 1, fileName.c_str());
//	}
//	else if (Utilities::endsWith(fileName, ATTACK_B_SUFF) && *attackFileB == nullptr)
//	{
//		*attackFileB = new char[fileName.length() + 1];
//		strcpy_s(*attackFileB, fileName.length() + 1, fileName.c_str());
//	}
//	else if (Utilities::endsWith(fileName, BOARD_SUFF) && *boardFile == nullptr)
//	{
//		*boardFile = new char[fileName.length() + 1];
//		strcpy_s(*boardFile, fileName.length() + 1, fileName.c_str());
//	}
//}

/*Checks if the path exists,
*that the 3 files are there,
*And if they are - updates their names */
//TODO:: fix this function!!
 bool Utilities::isValidPath(const char* path, char** boardFile, char** attackFileA, char** attackFileB)
{
	// Changing work directory to reletive path if needed
	if (_chdir(path) != 0)
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	try
	{
		std::vector<std::string> attackFiles;
		findAttackFiles(path, std::strlen(path), attackFiles);
	} catch (std::exception& e)
	{
		throw e;
	}

	//char buffer[MAX_BUFFER];
	//std::string data_str;
	//std::string command = "2>NUL dir /a-d /b \"";
	//command.append(path);
	//command.append("\"");
	//FILE* fp;
	//if ((fp = _popen(command.c_str(), "r")) != NULL) {
	//	while (fgets(buffer, MAX_BUFFER, fp))
	//	{
	//		data_str += std::string(buffer);
	//	}
	//	_pclose(fp);
	//}
	//else
	//{
	//	throw Exception(exceptionInfo(WRONG_PATH, path));
	//}
	//std::stringstream data_stream(data_str);
	//std::string line;
	//while (std::getline(data_stream, line))
	//{
	//	checkFileName(line, boardFile, attackFileA, attackFileB);
	//}

	//making sure everything's initiallized
	if (*boardFile == nullptr || *attackFileA == nullptr || *attackFileB == nullptr) {
		return false;
	}

	return true;
}

/*
*Used when crucial files are missing, frees memory of files who where found,
*Prints relevant errors to the screen
*/
void Utilities::printNotFoundFileErrors(const char* path, char* boardFile, char* attackFileA, char* attackFileB)
{
	if (boardFile == nullptr) {
		std::cout << "Missing board file (*.sboard) looking in path: " << path << std::endl;
		if (attackFileA != nullptr)
		{
			delete attackFileA; //clear memory
		}
		else
		{
			std::cout << "Missing attack file for player A (*.attack-a) looking in path: " << path << std::endl;
		}
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

/*TODO:: move this function to relevant algorithm
* Maintains "attackFiles" to hold at most 2 attack files,
* The ones which are first lexicographically.
*/
void Utilities::addAttackFileToList(std::vector<std::string>& attackFiles, std::string filename)
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

/*TODO:: move this function to relevant algorithm
 * Gets a valid path (directory), updates "attackFiles" to contain 2 (at most) attack files - oredered lexicographically 
 */
int Utilities::findAttackFiles(const char* path, size_t pathLen, std::vector<std::string>& attackFiles)
{
	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.
	StringCchLengthA(path, MAX_PATH, &pathLen);
	if (pathLen > (MAX_PATH - 3))
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path);
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return dwError;
	}

	// checks all files in the directory in search for ".attack" files.
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			if (endsWith(std::string(ffd.cFileName), std::string(".attack")))
			{
				addAttackFileToList(attackFiles, std::string(ffd.cFileName));
			}
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		return dwError;
	}
	FindClose(hFind);
	return dwError;
}

/*TODO:: move this function to relevant algorithm
 * Gets a valid path (directory), updates "attackFiles" to contain 2 (at most) attack files - oredered lexicographically 
 */
void Utilities::findAttackFiles(const char* path, size_t pathLen, std::vector<std::string>& attackFiles)
{
	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.
	StringCchLengthA(path, MAX_PATH, &pathLen);
	if (pathLen > (MAX_PATH - 3))
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path);
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		throw Exception("Error: failed finding files in path");
	}

	// checks all files in the directory in search for ".attack" files.
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			if (endsWith(std::string(ffd.cFileName), std::string(ATTACK_SUFF)))
			{
				addAttackFileToList(attackFiles, std::string(ffd.cFileName));
			}
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		throw Exception("Error: failed passing all files in path");
	}
	FindClose(hFind);
}