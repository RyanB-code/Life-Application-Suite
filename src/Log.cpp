#include "Log.h"

#pragma warning(disable : 4996)

// static variable initialization
std::vector<Log> Log::s_logList{}; // Initializer for s_logList to be used
std::string Log::m_path = "";
bool Log::m_wroteAllStoredLogs = false;

Log::Log(LogCode code, std::string msg)
	: m_code	{ code }, 
	m_msg		{ msg }
{
	s_logList.push_back(*this);

	// Check if the log list will write to console or the file
	if (m_path == "") {
		std::cout << *this;
	}
	else {
		// Write to file
		if(std::filesystem::exists(m_path)) {
			std::ofstream file{ m_path, std::ios_base::app};

			// Check if stored logs have already been written to the file. If not, writes them
			if(!m_wroteAllStoredLogs){
				for(Log currentLog : s_logList){
					file << currentLog;
				}
				m_wroteAllStoredLogs = true;
			}
			else{		
				file << *this;	// If stored logs have been written already, append file normally
			}
		}
		else {
			// Write to console if issue finding file
			std::cout << *this;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Log& log) {

	tm* ltm = localtime(&log.m_timestamp);

	//Time in HH:MM:SS format
	os << "["
		<< std::setw(2) << std::setfill('0') << ltm->tm_hour << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_min << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_sec << "]";

	os << "\t[";
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
		os << "  LOG  ";
		break;
	}
	os << "]";

	os << "\t" << log.m_msg << '\n';

	return os;
}