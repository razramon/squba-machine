#include "submarinesGame.h"

typedef std::vector<std::vector<std::vector<char>>> boardType;
using namespace std;


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	int threadsNum = THREADS_DEFAULT_NUMBER;

	unique_ptr<std::vector<std::string>> boardFiles = make_unique<std::vector<std::string>>();
	unique_ptr<std::vector<std::string>> DLLFiles = make_unique<std::vector<std::string>>();

	//print errors, according to: http://moodle.tau.ac.il/mod/forum/discuss.php?d=63147#p93599
	try
	{
		Utilities::buildPath(argc, argv, threadsNum, DLLFiles, boardFiles);
	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;

		return 1;
	}

	//TODO:: check from here down (all of the code)
	shared_ptr<GameManager> manager = make_shared<GameManager>(DLLFiles, boardFiles);

	// Saving thread for the logger
	manager->setNumberThreads(threadsNum - 1);

	try
	{
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

