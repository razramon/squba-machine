#include "Exception.h"


Exception::Exception(const char* msgInfo)
{
	size_t len = strlen(msgInfo);
	msg = new char[len + 1];
	strcpy_s(msg, len+1, msgInfo);
}

Exception::Exception(std::string msgInfo):Exception::Exception(msgInfo.c_str())
{
}

Exception::~Exception()
{
	delete[] msg;
}

const char* Exception::what() const{
	return msg;
}
