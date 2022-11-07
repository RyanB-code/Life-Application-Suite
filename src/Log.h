#ifndef LOG_H
#define LOG_H

#include <string>
#include <sstream>
#include <iomanip>

#include <filesystem>
#include <fstream>

#include <iostream>


enum class LogCode {
	FATAL,
	WARNING,
	ROUTINE,
	LOG,
};

class Log {
public:
	Log(LogCode code, std::string msg);

	LogCode m_code;
	std::string m_msg;

	static std::string m_path;

};

std::ostream& operator<<(std::ostream& os, Log& log);

#endif