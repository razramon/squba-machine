#pragma once

#include <windows.h>
#include <iostream>
#include "Exception.h"
#include "Macros.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

class Utilities
{
public:
	Utilities();
	~Utilities();
	static std::istream& getAllKindsOfLine(std::istream& inputStream, std::string& line);
	static std::string delSpaces(std::string &str);
	static std::string workingDirectory();
};

