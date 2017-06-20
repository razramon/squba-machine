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
	
	/*
	* Returns a string representing the current working directory
	*/
	static std::string workingDirectory();

	/*
	* Checks if the path exists,
	* Updates vectors of strings:
	*		DLLFiles - to represent all dll files found in path (each one has the full path to it!!)
	*		boardFiles - to represent all board files found in path (each one has the full path to it!!)
	* If path doesn't exist or an error getting to it accured - throws exception
	*/
	static void findDLLBoardFiles(const std::string path, std::unique_ptr<std::vector<std::string>>& DLLFiles, std::unique_ptr<std::vector<std::string>>& boardFiles);
	
	/*
	* Updates vectors of strings:
	*		DLLFiles - to represent all dll files found in path (each one has the full path to it!!)
	*		boardFiles - to represent all board files found in path (each one has the full path to it!!)
	*		
	* Throws exception errors, containing the folowing ".what()":
	*		If path is wrong - "Wrong path: <path>"
	*		If no (.sboard) files were found - "No board files (*.sboard) looking in path: <path>"
	*		If number of dlls who were found is less than 2 - "Missing algorithm (dll) files looking in path: <path> (needs at least two)"
	*/
	static void buildPath(int argc, char* argv[], int& threadsNum, 
		std::unique_ptr<std::vector<std::string>>& DLLFiles, std::unique_ptr<std::vector<std::string>>& boardFiles);

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

	/*
	* Returns true if line is empty and has NO SPACES
	* NOTE: returns true for an empty string!
	*/
	static bool isEmptyLineWOSpaces(const std::string line);

	/*
	 * A function that does nothing - to avoid warnings such as: "'e':unreferenced local variable" (where e is an exception we don't care about)
	 */
	template<class T>
	static void ignore(const T&){}
};
