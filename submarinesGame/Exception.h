#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
protected:
	char* msg;
public:
	explicit Exception(const std::string msgInfo);
	explicit Exception(const char* msgInfo);
	virtual ~Exception();
	virtual const char* what() const;
};

