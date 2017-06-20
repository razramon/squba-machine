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
		throw Exception(Macros::exceptionInfo(Macros::WRONG_PATH, path));
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
		throw Exception(Macros::exceptionInfo(Macros::WRONG_PATH, path));
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


void Utilities::findDLLBoardFiles(const std::string path, std::unique_ptr<std::vector<std::string>>& DLLFiles, std::unique_ptr<std::vector<std::string>>& boardFiles)
{
	// Change working directory to reletive path if needed
	if (_chdir(path.c_str()) != 0)
	{
		throw Exception(Macros::exceptionInfo(Macros::WRONG_PATH, path));
	}

	WIN32_FIND_DATAA ffd;
	char szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Check that input path+3 isn't longer than MAX_PATH, 3 characters for the "\*" plus NULL appended below.
	if (path.size() > (MAX_PATH - 3))
	{
		throw Exception(Macros::exceptionInfo(Macros::WRONG_PATH, path));
	}

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.
	StringCchCopyA(szDir, MAX_PATH, path.c_str());
	StringCchCatA(szDir, MAX_PATH, "\\*");

	// Find the first file in the directory.
	hFind = FindFirstFileA(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		throw Exception(Macros::exceptionInfo(Macros::WRONG_PATH, path));
	}

	// checks all files in the directory in search for ".sboard" file.
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //checks the file isn't a directory:
		{
			if (Utilities::endsWith(std::string(ffd.cFileName), std::string(Utilities::BOARD_SUFF)))
			{
				std::string boardFile = std::string(ffd.cFileName);
				(*boardFiles).push_back(path + "\\" + boardFile);
			}
			else if (Utilities::endsWith(std::string(ffd.cFileName), std::string(Utilities::DLL_SUFF)))
			{
				std::string dllFile = std::string(ffd.cFileName);
				(*DLLFiles).push_back(path + "\\" + dllFile);
			}
		}
	}
	while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
}

void Utilities::buildPath(int argc, char* argv[], int& threadsNum, std::unique_ptr<std::vector<std::string>>& DLLFiles,
						std::unique_ptr<std::vector<std::string>>& boardFiles)
{
	std::string path;
	threadsNum = THREADS_DEFAULT_NUMBER;
	Utilities::setArguments(argc, argv, path, threadsNum);
	if (path.size() == 0) //
	{
		path = workingDirectory();
	}

	try
	{
		getFullPath(path);
		findDLLBoardFiles(path, DLLFiles, boardFiles);
	}
	catch (std::exception& e)
	{
		throw Exception(e.what());
	}

	if ((*boardFiles).size() == 0 && (*DLLFiles).size() < 2)
	{
		throw Exception(Macros::exceptionInfo(Macros::MISSING_BOARD, path) + "\n" + Macros::exceptionInfo(Macros::MISSING_ALGO, path));
	} 
	if ((*boardFiles).size() == 0)
	{
		throw Exception(Macros::exceptionInfo(Macros::MISSING_BOARD, path));
	}
	if ((*DLLFiles).size() < 2)
	{
		throw Exception(Macros::exceptionInfo(Macros::MISSING_ALGO, path));
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
	if (argu.compare("-threads") == 0)
	{
		return Threads;
	}

	try { //isNumeric might through exception...(if isDigit() failes)
		if (isNumeric(argu))
		{
			return Number;
		}
	}
	catch (std::exception& e)
	{
		ignore(e);
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
		case (Threads):
			if (i < argc - 1)
			{
				if (getTypeOfArg(argv[i + 1]) == Number)
				{
					i++;
					threadsNum = std::atoi(argv[i]);
				}
			}
			break;
		case (Number):
		default:
			break;
		}
	}
}


int Utilities::countAppearances(char ch, const std::string & str)
{
	int counter = 0;
	for (char c : str)
	{
		if (c == ch)
		{
			counter++;
		}
	}
	return counter;
}

bool Utilities::isLineEmpty(const std::string line)
{
	for (char c : line)
	{
		if (c != ' '  && c != '\t' && c != '\n')
		{
			return false;
		}
	}
	return true;
}

bool Utilities::isEmptyLineWOSpaces(const std::string line)
{
	for (char c : line)
	{
		if (c != '\r' && c != '\n')
		{
			return false;
		}
	}
	return true;
}
