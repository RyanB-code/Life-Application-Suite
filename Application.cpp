#include "Application.h"



#pragma warning(disable : 4996)


Application::Application() {
	if (!FileSystem::createDirectory(DIRECTORY_PATH.string()) || !FileSystem::createDirectory(DEBUG_PATH.string())) {
		Log(LogCode::FATAL, "Could not initialize the file system.");
	}
	else {
		//Once the app folder is made and a debug folder is made
		std::ostringstream debugFilePath{ DEBUG_PATH.string() + LogFileName().str() + ".dat" };

		if (!FileSystem::createFile(debugFilePath.str())) {
			Log(LogCode::FATAL, "Could not create log file.");
		}
		else {
			m_currentInstanceLogFile = debugFilePath.str();
			Log::m_path = m_currentInstanceLogFile.string();

			m_initialized = true;
		}
	}
}

void Application::run() {
	if (!m_initialized) {
		throw Log(LogCode::FATAL, "Application initialization failed.");
		return;
	}
	else {
		Display::Home();
	}

	return;
}

bool const Application::saveVehicles() {
	bool success{ false };
	std::ostringstream savePath{ DIRECTORY_PATH.string() + VEHICLE_FILE_NAME };
	for (Vehicle& currentVehicle : m_vehicleList) {
		if (!FileSystem::writeToFile(savePath.str(), currentVehicle)) {
			success = false;
		}
		else
			success = true;
	}

	return success;
}


std::ostringstream Application::LogFileName() {
	time_t now{ time(0) };
	tm* ltm = localtime(&now);

	std::ostringstream date;

	// THIS DISPLAYS THE TIME IN HH : MM:SS format

		date
		<< std::setw(2) << std::setfill('0') << ltm->tm_hour << '-'
		<< std::setw(2) << std::setfill('0') << ltm->tm_min << '-'
		<< std::setw(2) << std::setfill('0') << ltm->tm_sec << " ";
	/*
	switch (ltm->tm_wday) {
	case 0:
		date << "SUN";
		break;
	case 1:
		date << "MON";
		break;
	case 2:
		date << "TUE";
		break;
	case 3:
		date << "WED";
		break;
	case 4:
		date << "THU";
		break;
	case 5:
		date << "FRI";
		break;
	case 6:
		date << "SAT";
		break;
	default:
		date << "DDD";
	}
	date << "-";
	*/

	date << std::setw(2) << std::setfill('0') << ltm->tm_mday << '-';
	switch (ltm->tm_mon) {
	case 0:
		date << "JAN";
		break;
	case 1:
		date << "FED";
		break;
	case 2:
		date << "MAR";
		break;
	case 3:
		date << "APR";
		break;
	case 4:
		date << "MAY";
		break;
	case 5:
		date << "JUN";
		break;
	case 6:
		date << "JUL";
		break;
	case 7:
		date << "AUG";
		break;
	case 8:
		date << "SEP";
		break;
	case 9:
		date << "OCT";
		break;
	case 10:
		date << "NOV";
		break;
	case 11:
		date << "DEC";
		break;
	default:
		date << "MMM";
	}


	int year{ ltm->tm_year + 1900 };

	date << '-' << year;

	return date;
}