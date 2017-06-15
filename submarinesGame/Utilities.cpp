#include "Utilities.h"

const std::string Utilities::BOARD_SUFF = ".sboard";
const std::string Utilities::DLL_SUFF = ".dll";
const int Utilities::NUM_PARTICIPANTS = 2;
const int Utilities::NUMBER_DLLS = 2;
const int Utilities::FILE_NOT_FOUND_ERROR = -1;
const int Utilities::INDEX_PATH_DLL_A = 0;
const int Utilities::INDEX_PATH_DLL_B = 1;
const int Utilities::INDEX_BOARD_PATH = 2;
const char Utilities::HIT_SIGN = '*';
const char Utilities::SINK_SIGN = '@';
const char Utilities::MISS_SIGN = '~';

void Utilities::getFullPath(std::string& path)
{
	char path_c[MAX_BUFFER];
	strncpy_s(path_c, path.c_str(), path.size());
	DWORD retval = 0;
	char buffer[MAX_BUFFER];
	char** lppPart = {nullptr};

	// Retrieve the full path name for a file. 
	retval = GetFullPathNameA(path_c, MAX_BUFFER, buffer, lppPart);

	if (retval == 0)
	{
		// Handle an error condition.
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}
	path = buffer;
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
		while (true)
		{
			c = sb->std::streambuf::sbumpc();
			switch (c)
			{
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
		std::string s = "Error: failed reading line: ";
		s.append(e.what());
		throw Exception(s.c_str());
	}
}

std::string Utilities::delSpaces(std::string& str)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
	return str;
}

/*
*Returns a path to (this) working directory - it's where the .exe file is...
*http://moodle.tau.ac.il/mod/forum/discuss.php?d=47695#p73943
*/
std::string Utilities::workingDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}


/*Checks if the path exists,
*that the board file is there,
*And if it is- updates its names */
bool Utilities::isValidPath(const std::string path, std::string& boardFile)	
{
	// Change working directory to reletive path if needed
	if (_chdir(path.c_str()) != 0)
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Check that input path+3 isn't longer than MAX_PATH, 3 characters for the "\*" plus NULL appended below.
	if (path.size() > (MAX_PATH - 3))
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path.c_str());
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		throw Exception(exceptionInfo(WRONG_PATH, path));
	}

	// checks all files in the directory in search for ".sboard" file.
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			if (Utilities::endsWith(std::string(ffd.cFileName), std::string(Utilities::BOARD_SUFF)))
			{
				boardFile = std::string(ffd.cFileName);
				FindClose(hFind);
				return true;
			}
		}
	}
	while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
	return false;
}

/*
* Prints relevant errors to the screen
*/
void Utilities::printNotFoundFileErrors(bool pathIsValid, const std::string& path, const std::vector<std::string>& filesFound)
{
	if (!pathIsValid) std::cout << "Missing board file (*.sboard) looking in path: " << path << std::endl;
	if (filesFound.size() < 2) std::cout << "Missing an algorithm (dll) file looking in path: " << path << std::endl;
}

/*
 * Returns a vector of size 3, containing:
 *				index 0 = INDEX_PATH_DLL_A =  full path to player A's dll
 *				index 1 = INDEX_PATH_DLL_B = full path to player B's dll
 *				index 2 = INDEX_BOARD_PATH = full path to board.
 *	If One (or more) is missing, returns smaller vector and PRINTS errors to screen!
 */
std::vector<std::string>* Utilities::buildPath(int argc, char* argv[], int& threadsNum)
{
	std::string path;

	Utilities::setArguments(argc, argv, path, threadsNum);
	if (path.size() == 0) //
	{
		path = workingDirectory();
	}

	//TODO:: need to edit this, to support printing!

	std::string boardFilePtr, fullPathToBoard;
	bool pathIsValid = false;
	std::vector<std::string>* filesFound = new std::vector<std::string>;

	try
	{
		getFullPath(path);
		pathIsValid = isValidPath(path, boardFilePtr);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return filesFound;
	}

	findAllFilesWithSuf(path.c_str(), path.size(), *filesFound, DLL_SUFF);
	if (((*filesFound).size() != NUMBER_DLLS) || (!pathIsValid))
	{
		printNotFoundFileErrors(pathIsValid, path, *filesFound);
	}
	else
	{
		(*filesFound).push_back(path + "\\" + boardFilePtr);
	}

	return filesFound;
}


/*
* Gets a valid path (directory), updates "filesFound" to contain 2 (at most) files - oredered lexicographically
* Returns:  number of files found in path, which are inerted to "filesFound" vector,
*			FILE_NOT_FOUND_ERROR = -1 / 0 if an error accured / no files have been found.
*/

int Utilities::findAllFilesWithSuf(const char* path, size_t pathLen, std::vector<std::string>& filesFound, const std::string suffix)
{
	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.
	StringCchLengthA(path, MAX_PATH, &pathLen);
	if (pathLen > (MAX_PATH - 3))
	{
		return FILE_NOT_FOUND_ERROR;
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path);
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return FILE_NOT_FOUND_ERROR;
	}

	// checks all files in the directory in search for files that end with suffix.
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			if (Utilities::endsWith(std::string(ffd.cFileName), std::string(suffix)))
			{
				addFileToList(filesFound, std::string(ffd.cFileName), path);
			}
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	if (GetLastError() != ERROR_NO_MORE_FILES)
	{
		return FILE_NOT_FOUND_ERROR;
	}
	FindClose(hFind);
	return filesFound.size();
}

/*
* Maintains "filesFound"
*/
void Utilities::addFileToList(std::vector<std::string>& filesFound, std::string filename, const std::string path)
{
	if (filesFound.size() == 0)
	{
		filesFound.push_back(path + "\\" + filename);
		return;
	}
	std::vector<std::string>::iterator iter;
	for (iter = filesFound.begin(); iter != filesFound.end(); ++iter)
	{
		filesFound.insert(iter, path + "\\" + filename);

	}
}

bool Utilities::isNumeric(std::string & s)
{
	for (size_t i = 0; i < s.length(); ++i)
	{
		if (!(std::isdigit(s.at(i)))) return false;
	}
	return true;
}

Utilities::Arguments Utilities::getTypeOfArg(std::string argu)
{
	if (argu.compare("-quiet") == 0)
	{
		return Quiet;
	}
	if (argu.compare("-delay") == 0)
	{
		return Delay;
	}
	try { //isNumeric might through exception...(if isDigit() failes)
		if (isNumeric(argu))
		{
			return Number;
		}
	}
	catch (std::exception& e)
	{
	}
	return Path;
}

void Utilities::setArguments(int argc, char * argv[], std::string & path, int & threadsNum)
{
	for (int i = 1; i < argc; ++i)
	{
		switch (getTypeOfArg(argv[i]))
		{
		case(Path):
			if (path.size() == 0)//means: path wasn't initialized yet
			{
				path = argv[i];
			}
			break;
		case (Number):
			threadsNum = *argv[i];
			break;
		default:
			break;
		}
	}
}


void Utilities::divideToDLLAndBoard(std::shared_ptr<std::vector<std::string>> allFiles, std::shared_ptr<std::vector<std::string>> boardFiles, std::shared_ptr<std::vector<std::string>> DLLFiles) {

	for (std::string file : *allFiles) {

		if (Utilities::endsWith(file, std::string(Utilities::BOARD_SUFF))) {

			(*boardFiles).push_back(file);
		}
		else if (Utilities::endsWith(file, std::string(Utilities::BOARD_SUFF))) {

			(*DLLFiles).push_back(file);
		}
	}
}