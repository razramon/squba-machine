#include "Exception.h"
#include "Macros.h"
#include <iostream>


Exception::Exception(const char* msgInfo)
{
	size_t len = strlen(msgInfo);
	std::cout << "msgInfo is: " << msgInfo << std::endl;
	std::cout << "msgInfo length is: " << len << std::endl;
	msg = new char[len + 1];
	strcpy_s(msg, len+1, msgInfo);
}

Exception::Exception(std::string msgInfo)
{
	new Exception(msgInfo.c_str());
}

Exception::~Exception()
{
	delete[] msg;
}

const char* Exception::what() const{
	return msg;
}
