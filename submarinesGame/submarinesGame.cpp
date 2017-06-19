#include "submarinesGame.h"

typedef std::vector<std::vector<std::vector<char>>> boardType;
using namespace std;


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //for memory leaks! :) TODO::delete before 

	int threadsNum = THREADS_DEFAULT_NUMBER;

	shared_ptr<std::vector<std::string>> filesFound = make_shared<vector<string>>(Utilities::buildPath(argc, argv, threadsNum));
	shared_ptr<std::vector<std::string>> boardFiles;
	shared_ptr<std::vector<std::string>> DLLFiles;
	shared_ptr<std::vector<std::shared_ptr<GameInfo>>> allGamesData;

	Utilities::divideToDLLAndBoard(filesFound, boardFiles, DLLFiles);

	//print errors, according to: http://moodle.tau.ac.il/mod/forum/discuss.php?d=63147#p93599
	if ((*filesFound).size() <= 2 || (*boardFiles).size() == 0 || (*DLLFiles).size() < 2)
	{

		return 1;
	}

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

