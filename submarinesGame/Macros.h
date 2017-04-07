#pragma once

enum Exeptions
{
	WRONG_SIZE_SHAPE,
	TOO_MANY_SHIPS,
	TOO_FEW_SHIPS,
	AJACENT_SHIPS,
	WRONG_PATH,
	MISSING_BOARD,
	MISSING_ATTACK_FILE
};


static std::string exceptionInfo(int exceptionNumber, std::string str1, std::string str2)
{
	switch (exceptionNumber)
	{
	case WRONG_SIZE_SHAPE:
		return ("Wrong size or shape for ship " + str1 + " for player " + str2);
	case MISSING_ATTACK_FILE: //will not be used eventually..
		return ("Missing attack file for player " + str1 + " (*.attack-a) looking in path: " + str2);
	default:
		return ("UNKNOWN exception type. check it!"); //not supposed to get here ever

	}
}

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
		case MISSING_BOARD: //will not be used eventually..
			return("Missing board file (*.sboard) looking in path: " + str1);
		default:
			return ("UNKNOWN exception type. check it!"); //not supposed to get here ever
	}
}

static std::string exceptionInfo(int exceptionNumber)
{
	switch (exceptionNumber)
	{
	case AJACENT_SHIPS:
		return ("Adjacent Ships on Board");
	default:
		return ("UNKNOWN exception type. check it!"); //not supposed to get here ever

	}
}