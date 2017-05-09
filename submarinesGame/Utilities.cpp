#include "Utilities.h"

const std::string Utilities::ATTACK_SUFF = ".attack";
const std::string Utilities::BOARD_SUFF = ".sboard";

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
	} while (FindNextFileA(hFind, &ffd) != 0);

	return false;
}

/*
* Prints relevant errors to the screen
*/
void Utilities::printNotFoundFileErrors(const char* path, char* boardFile)
{
	if (boardFile == nullptr) std::cout << "Missing board file (*.sboard) looking in path: " << path << std::endl;
}
