#pragma once

#include <windows.h>
#include <sstream>
#include <algorithm>
#include "Exception.h"
#include <direct.h>

class Utilities
{
	static const std::string ATTACK_A_SUFF;
	static const std::string ATTACK_B_SUFF;
	static const std::string BOARD_SUFF;
	static const int MAX_BUFFER = 4096;
public:
	Utilities();
	~Utilities();
	static bool doesPathExist(const char* path);
	static bool endsWith(const std::string& fileName, const std::string& suffix);
	static std::istream& getAllKindsOfLine(std::istream& inputStream, std::string& line);
	static std::string delSpaces(std::string &str);
	static std::string workingDirectory();
	static bool isValidPath(const char* path, char** boardFile, char** attackFileA, char** attackFileB);
	static void checkFileName(const std::string& fileName, char** boardFile, char** attackFileA, char** attackFileB);
	static void printNotFoundFileErrors(const char* path, char* boardFile, char* attackFileA, char* attackFileB);
};

