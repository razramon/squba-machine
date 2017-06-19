#include "submarinesGame.h"

typedef std::vector<std::vector<std::vector<char>>> boardType;

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	int threadsNum = THREADS_DEFAULT_NUMBER;
	std::unique_ptr<std::vector<std::string>> boardFiles = std::make_unique<std::vector<std::string>>();
	std::unique_ptr<std::vector<std::string>> DLLFiles = std::make_unique<std::vector<std::string>>();

	//print errors, according to: http://moodle.tau.ac.il/mod/forum/discuss.php?d=63147#p93599
	try
	{
		Utilities::buildPath(argc, argv, threadsNum, DLLFiles, boardFiles);
	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	try
	{
		std::unique_ptr<GameManager> manager = std::make_unique<GameManager>(DLLFiles, boardFiles, threadsNum);
		manager->startGames();
	}
	catch (std::exception& e)
	{
		if (strcmp(e.what(), "PRINT_NOTHING") != 0)
		{
			std::cout << e.what() << std::endl;
		}
	}

	return 0;
}

