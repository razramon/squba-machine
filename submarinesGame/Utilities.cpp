#include "Utilities.h"

Utilities::Utilities()
{
}

Utilities::~Utilities()
{
}


/*
* Returns the current stream position,
* Updates the current line into "line".
*/
std::istream& Utilities::getAllKindsOfLine(std::istream& inputStream, std::string& line)
{
	line.clear();
	try
	{
		std::istream::sentry se(inputStream, true);
		std::streambuf* sb = inputStream.rdbuf();
		int c;
		while (true) {
			c = sb->std::streambuf::sbumpc();
			switch (c) {
			case '\n':
				return inputStream;
			case '\r':
				if (sb->std::streambuf::sgetc() == '\n')
				{
					sb->std::streambuf::sbumpc();
				}
				return inputStream;
			case EOF:
				if (line.empty()) //last line of the file has no line terminator
				{
					inputStream.setstate(std::ios::eofbit);
				}
				return inputStream;
			default:
				line += static_cast<char>(c);
			}
		}

	}
	catch (std::exception& e)
	{
		throw Exception("Error: failed reading line.");
	}
}

std::string Utilities::delSpaces(std::string &str)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
	return str;
}

/*
*Returns a path to (this) working directory - it's where the .exe file is...
*http://moodle.tau.ac.il/mod/forum/discuss.php?d=47695#p73943
*/
std::string Utilities::workingDirectory() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}
