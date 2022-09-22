#include "Log.h"

#pragma warning(disable : 4996)

std::string Log::m_path = "";

Log::Log(LogCode code, std::string msg)
	: m_code	{ code }, 
	m_msg		{ msg }
{
	std::ostringstream os;

	time_t now{ time(0) };
	tm* ltm = localtime(&now);
	// THIS DISPLAYS THE TIME IN HH : MM:SS format

	os << "["
		<< std::setw(2) << std::setfill('0') << ltm->tm_hour << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_min << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_sec << "]\t";
		// NEED TO ADD MILLISECONDS<< std::setw(2) << std::setfill('0') << ltm->tm_

	switch (m_code) {
	case LogCode::FATAL:
		os << "FATAL";
		break;
	case LogCode::WARNING:
		os << "WARNING";
		break;
	case LogCode::ROUTINE:
		os << "ROUTINE";
		break;
	case LogCode::LOG:
		os << "LOG";
		break;
	}

	os << "\t\t" << m_msg << "\n";

	if (m_path == "") {
		std::cout << os.str();
	}
	else {
		if(std::filesystem::exists(m_path)) {
			std::ofstream file{ m_path, std::ios_base::app};
			file << os.str();
		}
		else {
			std::cout << os.str();
		}
	}

}

std::ostream& operator<<(std::ostream& os, Log& log) {
	switch (log.m_code) {
	case LogCode::FATAL:
		os << "FATAL";
		break;
	case LogCode::WARNING:
		os << "WARNING";
		break;
	case LogCode::ROUTINE:
		os << "ROUTINE";
		break;
	case LogCode::LOG:
		os << "LOG";
		break;
	}

	os << "\t" << log.m_msg << '\n';

	return os;
}