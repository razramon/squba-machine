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
#include <stdlib.h>
#include <thread>


#pragma comment(lib, "User32.lib")

class Utilities
{
	Utilities() = delete;
	~Utilities() = delete;
	static const int MAX_BUFFER = 4096;
	enum Arguments
	{
		Path,
		Threads,
		Number
	};
public:
	static const std::string ATTACK_SUFF, BOARD_SUFF, DLL_SUFF;
	static const int NUM_PARTICIPANTS, NUMBER_DLLS, FILE_NOT_FOUND_ERROR, INDEX_PATH_DLL_A, INDEX_PATH_DLL_B, INDEX_BOARD_PATH;
	static const char HIT_SIGN, SINK_SIGN, MISS_SIGN;

	static void getFullPath(std::string& path);

	static bool doesPathExist(const char* path);
	static bool endsWith(const std::string& fileName, const std::string& suffix);
	static std::istream& getAllKindsOfLine(std::istream& inputStream, std::string& line);
	static std::string delSpaces(std::string& str);
	static std::string workingDirectory();

	/*
	 *
	 */
	static bool isValidPath(const std::string path, std::string& boardFile);
	static void printNotFoundFileErrors(bool pathIsValid, const std::string& path, const std::vector<std::string>& filesFound);
	

	/*
	* Returns a vector of strings, representing all files found in path
	* PRINTS errors to screen:
	*		If path is wrong, print "Wrong path: <path>"
	*		If no (.sboard) files were found, prints: "No board files (*.sboard) looking in path: <path>"
	*		If number of dlls who were found is less than 2, prints: "Missing algorithm (dll) files looking in path: <path> (needs at least two)"
	*/
	static std::unique_ptr<std::vector<std::string>> buildPath(int argc, char* argv[], int& threadsNum);

	/*
	* Gets a valid path (directory), updates "filesFound" to contain 2 (at most) files
	* that end with suffix - oredered lexicographically.
	* Returns 0/-1 if no files where found / an error accured,
	*			1/2 if 1/2 files where found
	*/
	static int findAllFilesWithSuf(const char * path, size_t pathLen, std::vector<std::string>& filesFound, const std::string suffix);
	
	
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
	*	"-threads" / a number, otherwise - the default is to recognize it as a path
	*	Note: the format to pass "-threads" argument is: "-threads <number of threads>"
	*	according to this: http://moodle.tau.ac.il/mod/forum/discuss.php?d=62283#p92869
	*/
	static Arguments getTypeOfArg(std::string argu);



	static void divideToDLLAndBoard(std::shared_ptr<std::vector<std::string>> allFiles, std::shared_ptr<std::vector<std::string>> boardFiles, std::shared_ptr<std::vector<std::string>> DLLFiles);

	//initialize delayMS to be negative before calling this function!
	/*
	* Initializes arguments:
	*			path - if it was delivered! (or something that can be regognized as one)
	*					NOTE: user will have to check if path haven't been change in this function
	*						(if it's still of length 0) - if it wasn't - user should, as required,
	*						take working directory as a path!!
	*/
	static void setArguments(int argc, char * argv[], std::string & path, int & threadsNum);

	/*
	* Returns the number of appearances of ch in str
	* 0 if it doesn't apper.
	*/
	static int countAppearances(char ch, const std::string& str);

	/*
	* Returns true if line contains only spaces
	* NOTE: returns true for an empty string!
	*/
	static bool isLineEmpty(const std::string line);
};
