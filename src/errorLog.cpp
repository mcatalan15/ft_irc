#include "../include/ErrorLog.hpp"

// Default Constructor
errorLog::errorLog() {}

// Destructor
errorLog::~errorLog() {}

// ErrorLog
bool errorLog::ErrorLog(std::string error) {
	std::cerr << "ircserv: " << error << std::endl;
	return true;
}
