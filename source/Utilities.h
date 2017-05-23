#pragma once

#include <windows.h>
#include <sstream>
#include <algorithm>
#include "Exception.h"
#include <direct.h>
#include <vector>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include "Sizes.h"
#include <cctype>
#pragma comment(lib, "User32.lib")

class Utilities
{
	Utilities() = delete;
	~Utilities() = delete;
	static const int MAX_BUFFER = 4096;
	enum Arguments
	{
		Path,
		Quiet,
		Delay,
		Number
	};
public:
	static const std::string ATTACK_SUFF, BOARD_SUFF, DLL_SUFF;
	static const int NUMBER_DLLS, FILE_NOT_FOUND_ERROR, INDEX_PATH_DLL_A, INDEX_PATH_DLL_B, INDEX_BOARD_PATH;
	static const char HIT_SIGN, SINK_SIGN,MISS_SIGN;

	static void getFullPath(std::string& path);

	static bool doesPathExist(const char* path);
	static bool endsWith(const std::string& fileName, const std::string& suffix);
	static std::istream& getAllKindsOfLine(std::istream& inputStream, std::string& line);
	static std::string delSpaces(std::string& str);
	static std::string workingDirectory();
	static bool isValidPath(const std::string path, std::string& boardFile);
	static void printNotFoundFileErrors(bool pathIsValid, const std::string& path, const std::vector<std::string>& filesFound);
	/*
	* Returns a vector of size 3, containing:
	*				index 0 - full path to player A's dll
	*				index 1 - full path to player B's dll
	*				index 2 - full path to board.
	*	If One (or more) is missing, returns smaller vector and PRINTS errors to screen!
	*/
	static std::vector<std::string>* buildPath(int argc, char* argv[], bool& delay, int& delayMS, bool&quiet);
	/*
	* Gets a valid path (directory), updates "filesFound" to contain 2 (at most) files
	* that end with suffix - oredered lexicographically.
	* Returns 0/-1 if no files where found / an error accured,
	*			1/2 if 1/2 files where found
	*/
	static int find2FilesWithSuf(const char* path, size_t pathLen, std::vector<std::string>& filesFound, const std::string suffix);
	/*
	* Maintains "filesFound" to hold at most 2 files,
	* The ones which are first lexicographically.
	*/
	static void addFileToList(std::vector<std::string>& filesFound, std::string filename, const std::string path);
	/*
	 * Depicts types of arguments of argv
	 */
	
	/*
	 * Return true if string s is numeric
	 * (all letters are digits)
	 * NOTE: isdigit() might throw an exception
	 */
	static bool isNumeric(std::string& s);
	/*
	* Gets a string: checks if it is:
	*	"-quiet" / "-delay" / a number / otherwise - the default is to recognize it as a path
	*/
	static Arguments getTypeOfArg(std::string argu);

	//initialize delayMS to be negative before calling this function!
	/*
	* Initializes arguments:
	*			path - if it was delivered! (or something that can be regognized as one)
	*					NOTE: user will have to check if path haven't been change in this function
	*						(if it's still of length 0) - if it wasn't - user should, as required,
	*						take working directory as a path!!
	*			delay - updates value to "true" if "-delay" flag was found
	*			quiet - updates value to "true" if "-quiet" flag was found
	*			delayMS - updates to the mili-sec found (only if it's in format: "-delal <delay in ms>"
	*					NOTE: if a "-delay" flag was found, without <delay in ms>, we'll use default one
	*/
	static void setArguments(int argc, char* argv[], std::string& path, bool& delay, int& delayMS, bool& quiet);
};
