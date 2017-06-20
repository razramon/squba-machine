#pragma once
#include <string>



class Macros {
public:
	static std::string exceptionInfo(int exceptionNumber, std::string str1)
	{
		switch (exceptionNumber)
		{
		case TOO_MANY_SHIPS:
			return ("Too many ships for player " + str1);
		case TOO_FEW_SHIPS:
			return ("Too few ships for player " + str1);
		case WRONG_PATH:
			return ("Wrong path: " + str1);
		case MISSING_BOARD:
			return ("No board files (*.sboard) looking in path: " + str1);
		case MISSING_ALGO:
			return ("Missing algorithm (dll) files looking in path: " + str1 + " (needs at least two)");
		case CANNOT_LOAD_DLL:
			return ("Cannot load dll: " + str1);
		case ALGO_INIT_FAILED:
			return ("Algorithm initialization failed for dll: " + str1);
		default:
			return ("UNKNOWN exception type. check it!"); //not supposed to get here ever
		}
	}
	
	enum Exeptions
	{
		WRONG_SIZE_SHAPE,
		TOO_MANY_SHIPS,
		TOO_FEW_SHIPS,
		AJACENT_SHIPS,
		WRONG_PATH,
		MISSING_BOARD,
		MISSING_ATTACK_FILE,
		CANNOT_LOAD_DLL,
		ALGO_INIT_FAILED,
		MISSING_ALGO
	};
};
