// Logger.cpp
// Implementation of a multithread safe singleton logger class

#include "Logger.h"
#include <stdexcept>

using namespace std;

const string Logger::LogLevelDebug = "DEBUG";
const string Logger::LogLevelInfo = "INFO";
const string Logger::LogLevelError = "ERROR";

const char* const Logger::LogFileName = "game.log";

Logger* Logger::pInstance = nullptr;

mutex Logger::sMutex;

Logger& Logger::instance()
{
	static Cleanup cleanup;

	lock_guard<mutex> guard(sMutex);

	if (pInstance == nullptr)
		pInstance = new Logger();
	return *pInstance;
}

Logger::Cleanup::~Cleanup()
{
	lock_guard<mutex> guard(Logger::sMutex);
	delete Logger::pInstance;
	Logger::pInstance = nullptr;
}

Logger::~Logger()
{
	mOutputStream.close();
}

Logger::Logger()
{
	mOutputStream.open(kLogFileName, ios_base::app);

	if (!mOutputStream.good()) {

		throw runtime_error("Unable to initialize the Logger!");
	}
}

void Logger::log(const string& inMessage, const string& inLogLevel)
{
	lock_guard<mutex> guard(sMutex);
	logHelper(inMessage, inLogLevel);
}


void Logger::logHelper(const std::string& inMessage, const std::string& inLogLevel)
{
	mOutputStream << inLogLevel << ": " << inMessage << endl;
}