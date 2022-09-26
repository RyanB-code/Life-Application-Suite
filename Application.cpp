#include "Application.h"
#pragma warning(disable : 4996)


Application::Application() {
	if (!FileSystem::createDirectory(DIRECTORY_PATH.string()) 
		|| !FileSystem::createDirectory(DEBUG_PATH.string()) 
		|| !FileSystem::createDirectory(VEHICLE_PATH.string()))
	{
		Log(LogCode::FATAL, "Could not initialize the file system.");
	}
	else {
		//Once the app folders are made,
		std::ostringstream debugFilePath{ DEBUG_PATH.string() + LogFileName().str() + ".dat" };	//Make debug log file for the instance

		if (!FileSystem::createFile(debugFilePath.str())) {
			Log(LogCode::FATAL, "Could not create debug log file.");
		}
		else {
			//All was initialized successfully
			m_currentInstanceLogFile = debugFilePath.str();
			Log::m_path = m_currentInstanceLogFile.string();

			m_initialized = true;
			Log(LogCode::LOG, "Initialization successful.");
		}
	}
}

void Application::run() {
	if (!m_initialized) {
		throw Log(LogCode::FATAL, "Application initialization failed.");
		return;
	}
	else {
		app = this;
		Startup();
	}

	return;
}

inline void Application::NewVehicle(Vehicle& vehicle) {
	m_vehicleList.push_back(vehicle);
	Log(LogCode::ROUTINE, "Added " + vehicle.getName() + " to list of vehicles.");
}
bool const Application::saveVehicles() {
	bool success{ false };
	for (Vehicle& currentVehicle : m_vehicleList) {
		std::ostringstream saveFileName{ VEHICLE_PATH.string() + currentVehicle.getName() + ".dat"};
		if (FileSystem::createFile(saveFileName.str())){	//Make/find file of the given name

			if (!FileSystem::writeToFile(saveFileName.str(), currentVehicle)) {
				Log(LogCode::WARNING, "Could not save vehicle information for " + currentVehicle.getName());
				success = false;
			}
			else {
				Log(LogCode::LOG, "Saved vehicle information for " + currentVehicle.getName());
				success = true;
			}
		}
		else {
			success = false;
		}
	}
	return success;
}

const std::string Application::makeVehicle(std::string& text) {
	if (text == "") { //Check if the text string exists
		return "";
	}

	std::string vehNameBuf{};

	bool writeChar{ false };
	bool exitLoop{ false };
	int charactersRead{ 0 };
	for (const char& c : text) { //Reads text between ( ) to write to vehNameBuf and deletes read characters
		++charactersRead;
		if (c == ')') {
			writeChar = false;
			exitLoop = true;
		}
		if (writeChar) {
			vehNameBuf += c;
		}
		if (c == '(') {
			writeChar = true;
		}

		if (exitLoop) {
			for (int i{ 0 }; i < charactersRead; ++i) {
				text.erase(0, 1);
			}
			return { vehNameBuf, 0 };
		}

	}
	return vehNameBuf;
}
const void Application::makeRepair(std::string& text, std::vector<Repair>& repairList) {
	std::string repBuf;						//Buffer of characters being read from text
	std::vector<std::string> repairStrings; //Vector of strings of repair info not yet formatted

	bool writeChar{ false };
	for (char& c : text) {					//Reads between < > to find repair information and writes to foundRepBuf;
		if (c == '>') {
			repairStrings.push_back(repBuf);
			repBuf.erase();
			writeChar = false;
		}

		if (writeChar) {
			repBuf += c;
		}
		if (c == '<') {
			writeChar = true;
		}
	}

	char separator{ '|' };
	for (std::string currentString : repairStrings) { //Reads unformatted repairStrings, formats them into Repair types and adds to repairList vector
		uint32_t				mileBuf{};
		std::ostringstream		typeBuf{};
		double					costBuf{};
		std::ostringstream		notesBuf{};
		bool					thirdPartyBuf{};

		readUntil(currentString, separator, mileBuf);
		typeBuf << readUntil(currentString, separator);
		readUntil(currentString, separator, costBuf);
		readUntil(currentString, separator, thirdPartyBuf);
		notesBuf << readUntil(currentString, separator);
		
		repairList.push_back(Repair { mileBuf, typeBuf.str(), costBuf, notesBuf.str(),thirdPartyBuf} );
	}

}
const void Application::makeGasStop(std::string& text, std::vector<GasStop>& gasList) {
	std::string foundGasBuf;				//Buffer of characters being read from text
	std::vector<std::string> gasStrings;	//Vector of gas stops not yet formatted

	bool writeChar{ false };
	for (char& c : text) {					//Read between [ ], write to gasStrings to be formatted to GasStops
		if (c == ']') {
			gasStrings.push_back(foundGasBuf);
			foundGasBuf.erase();
			writeChar = false;
		}
		if (writeChar) {
			foundGasBuf += c;
		}
		if (c == '[') {
			writeChar = true;
		}
	}

	char separator{ '|' };
	for (std::string currentString : gasStrings) {	//Reads unformatted gasStrings, formats them into GasStop types and adds to gasList vector
		uint32_t			mileBuf{};
		short				gallonsBuf{};
		double				ppgBuf{};
		std::ostringstream	notesBuf{};

		readUntil(currentString, separator, mileBuf);
		readUntil(currentString, separator, ppgBuf);
		readUntil(currentString, separator, gallonsBuf);
		notesBuf << readUntil(currentString, separator);

		gasList.push_back(GasStop{ mileBuf, gallonsBuf, ppgBuf, notesBuf.str()});
	}
}

std::ostringstream Application::LogFileName() {
	time_t now{ time(0) };
	tm* ltm = localtime(&now);

	std::ostringstream date;

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

	// THIS DISPLAYS THE TIME IN HH-MM-SS format

	date << "_"
		<< std::setw(2) << std::setfill('0') << ltm->tm_hour << '-'
		<< std::setw(2) << std::setfill('0') << ltm->tm_min << '-'
		<< std::setw(2) << std::setfill('0') << ltm->tm_sec;
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
	return date;
}
//========END PUBLIC=========

//==========PRIVATE==========

void Application::Startup() {

	//Go through the vehicle folder and store the names of the files
	std::vector <std::string> vehicleFiles;
	FileSystem::filesInDirectory(VEHICLE_PATH.string(), vehicleFiles);

	//Iterate through the files in the folder found and create Vehicle types
	for (const std::string& fileName : vehicleFiles) {
		std::ostringstream fileText;

		//Opens file, reads text, outputs to fileText
		if (!FileSystem::readFile(fileName, fileText)) {
			//If vehicle file couldn't be opened to read
			//Do nothing
		}
		else { //File opened, save the text and iterate. Writes Vehicle and repairs and gas stops

			std::string vehInfoBuf{ fileText.str() };

			Vehicle vehicleBuffer{ makeVehicle(vehInfoBuf), 0 };
	
			if (vehicleBuffer.getName() == "") {	//If the file could not find a name, does not add vehicle to m_vehicleList
				Log(LogCode::WARNING, "Vehilce data format error for " + fileName);
			}
			else {
				//Reads vehInfoBuf and writes found repairs to repairBuffer
				std::vector<Repair> repairBuffer;
				makeRepair(vehInfoBuf, repairBuffer);		//Make repairs from the remaining text

				//Add the repairBuffer members to the vehicleBuffer
				for (Repair& rep : repairBuffer) {
					vehicleBuffer.NewRepair(rep);
					Log(LogCode::ROUTINE, "Added a repair to " + vehicleBuffer.getName());
				}

				//Reads vehInfoBuf and writes found gas stops to gasBuffer
				std::vector<GasStop> gasBuffer;
				makeGasStop(vehInfoBuf, gasBuffer);		//Make Gas stop from the remaining text

				//Add the gasBuffer members to vehicleBuffer
				for (GasStop& gas : gasBuffer) {
					vehicleBuffer.NewGasStop(gas);
					Log(LogCode::ROUTINE, "Added a gas stop to " + vehicleBuffer.getName());
				}
				NewVehicle(vehicleBuffer); //Add vehicleBuffer to the master m_vehicleList
			}
		}
		Log(LogCode::LOG, "Done reading " + fileName);
	}
	Log(LogCode::ROUTINE, "Startup processes finised.");
}
void Application::readUntil(std::string& text, const char limit, auto& returnType) {
	std::stringstream bufferText;
	
	int charRead{ 0 };
	for (char& c : text) {
		++charRead;
		if (c != limit) {
			bufferText << c;
		}
		else {
			bufferText >> returnType;
			text.erase(0, charRead);
			return;
		}
	}
}
std::string Application::readUntil(std::string& text, const char limit) {
	std::string bufferText;
	int charRead{ 0 };
	for (const char& c : text) {
		++charRead;
		if (c != limit && charRead < text.length()) {
			bufferText += c;
		}
		else {
			if (c != limit) {
				bufferText += c;
			}
			text.erase(0, charRead);
			return bufferText;
		}
	}
}
//===========================