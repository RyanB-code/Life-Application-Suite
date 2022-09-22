#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <filesystem>
#include <fstream>

#include <iostream>


enum class LogCode {
	FATAL,
	WARNING,
	ROUTINE,
	LOG
};

class Log {
public:
	Log(LogCode code, std::string msg);

	LogCode m_code;
	std::string m_msg;

	static std::string m_path;
	static std::vector<Log> buffer;

};

std::ostream& operator<<(std::ostream& os, Log& log);

