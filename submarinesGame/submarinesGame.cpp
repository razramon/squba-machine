#include <windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <exception>

//returns true if the path exists && it's a DIRECTORY
bool doesPathExist(const char* path)
{
	DWORD dirAttr = GetFileAttributesA(path);
	if (dirAttr == INVALID_FILE_ATTRIBUTES)
	{
		throw GetLastError();
	} 
	return ((dirAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
}


//checks if the path exists and  that the 3 files are there
bool isValidPath(const char* path)
{
	bool doesExist;
	try
	{
		doesExist = doesPathExist(path);
		if(doesExist)
		{
			std::cout << "path is a directory!!" << std::endl;
			return true;
		} else
		{
			std::cout << "path exists, but it's not a directory!!" << std::endl;
			return false;
		}
		
	}
	catch (std::exception& e) {
		std::cout << "Exception accured: " << e.what() << std::endl;
		return false;
	}

	//char* attackFileA = NULL;
	//char* attackFileB = NULL;
	//char* boardFile = NULL;


	//while()
	//{
	//	
	//}
}

int main(int argc, char* argv[])
{
	std::cout << isValidPath << std::endl;
	return 0;
}

