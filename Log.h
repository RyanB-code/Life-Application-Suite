#pragma once
#include <string>
#include <sstream>
#include <iomanip>

#include <iostream>
enum class LogCode {
	FATAL,
	WARNING,
	ROUTINE,
	LOG
};

struct Log {
	Log(LogCode code, std::string msg);

	LogCode m_code;
	std::string m_msg;

};

std::ostream& operator<<(std::ostream& os, Log& log);

