// Logger.h
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>
#include <iomanip>
#include <stdexcept>

// Definition of a multithread safe singleton logger class
class Logger
{
public:
	static const std::string LogLevelDebug;
	static const std::string LogLevelInfo;
	static const std::string LogLevelError;

	// Returns a reference to the singleton Logger object
	static Logger& instance();

	// Logs a single message at the given log level
	void log(const std::string& inMessage,
		const std::string& inLogLevel);


protected:
	// Static variable for the one-and-only instance  
	static Logger* pInstance;

	// Constant for the filename
	static const char* const LogFileName;

	// Data member for the output stream
	std::ofstream mOutputStream;

	// Embedded class to make sure the single Logger
	// instance gets deleted on program shutdown.
	friend class Cleanup;
	class Cleanup
	{
	public:
		~Cleanup();
	};

	// Logs message. The thread should own a lock on sMutex
	// before calling this function.
	void logHelper(const std::string& inMessage,
		const std::string& inLogLevel);

private:
	Logger();
	virtual ~Logger();
	static std::mutex sMutex;
};