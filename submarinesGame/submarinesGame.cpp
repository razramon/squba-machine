#include <windows.h>

//returns true if the path exists
bool doesPathExist(const char* path)
{
	DWORD fileAttr = GetFileAttributesA(path);
	if (fileAttr == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	} else
	{
		return true;
	}
}

//checks if the path exists and  that the 3 files are there
bool isValidPath(const char* path, size_t pathLen)
{

}

int main(int argc, char* argv[])
{


	return 0;
}

