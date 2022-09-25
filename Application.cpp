#include "Application.h"



#pragma warning(disable : 4996)


Application::Application() {
	if (!FileSystem::createDirectory(DIRECTORY_PATH.string()) 
		|| !FileSystem::createDirectory(DEBUG_PATH.string()) 
		|| !FileSystem::createDirectory(VEHICLE_FOLDER.string()))
	{
		Log(LogCode::FATAL, "Could not initialize the file system.");
	}
	else {
		//Once the app folder is made and a debug folder is made
		std::ostringstream debugFilePath{ DEBUG_PATH.string() + LogFileName().str() + ".dat" };

		if (!FileSystem::createFile(debugFilePath.str())) {
			Log(LogCode::FATAL, "Could not create debug log file.");
		}
		else {
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
		Display::clear();
		Startup();	
		std::cout << ListVehicles().str();

	}

	return;
}

bool const Application::saveVehicles() {

	bool success{ false };
	for (Vehicle& currentVehicle : m_vehicleList) {
		std::ostringstream saveFileName{ VEHICLE_FOLDER.string() + currentVehicle.getName() + ".dat"};
		if (FileSystem::createFile(saveFileName.str())){

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

bool Application::Startup() {

	//read Vehicle files
	std::vector<std::string> vehicleFiles;
	std::string fileNameBuffer;

	bool writeChar{ true };
	for (const char& c : FileSystem::filesInDirectory(VEHICLE_FOLDER.string()).str()) {

		//Iterate through the stream and write each file name to the vehicleFile vector
		if(c == '\n') {
			vehicleFiles.push_back(fileNameBuffer); //save current string buffer to vector
			fileNameBuffer = "";					//reset buffer
			writeChar = false;						//do not copy the newling character
		}
		else {
			writeChar = true;
		}

		if (writeChar) {
			fileNameBuffer += c; //save the current character
		}
	}

	std::ostringstream logText; 
	logText << "Found " << vehicleFiles.size() << " vehicle file(s)";
	Log(LogCode::ROUTINE, logText.str());


	for (const std::string& fileName : vehicleFiles) {
		std::ostringstream fileText;
		
		//Opens vehicle file and reads text 
		if (!FileSystem::readFile(fileName, fileText)) { 
			//If vehicle file couldn't be opened to read
			//Do nothing
		}
		else{
			std::string vehicleInformationBuffer{ fileText.str() };

			//make a vehicle type from the text found in the vehicle info buffer and delete read characters
			Vehicle vehicleBuffer{ makeVehicle(vehicleInformationBuffer) }; 
			
			//Make repairs from the remaining text
			std::vector<Repair> repairBuffer;
			makeRepair(vehicleInformationBuffer, repairBuffer);

			for (Repair& rep : repairBuffer) {
				vehicleBuffer.NewRepair(rep);
				Log(LogCode::ROUTINE, "Added a repair to " + vehicleBuffer.getName());
			}

			std::vector<GasStop> gasBuffer;
			makeGasStop(vehicleInformationBuffer, gasBuffer);
			for (GasStop& gas : gasBuffer) {
				vehicleBuffer.NewGasStop(gas);
				Log(LogCode::ROUTINE, "Added a gas stop to " + vehicleBuffer.getName());
			}

			NewVehicle(vehicleBuffer); //add vehicle to the known list
		}
		
	}
	//end reading vehicle Files



	return true;
}

inline void Application::NewVehicle(Vehicle& vehicle) {
	m_vehicleList.push_back(vehicle); 
	Log(LogCode::ROUTINE, "Added " + vehicle.getName() + " to list of vehicles.");
}

const Vehicle Application::makeVehicle(std::string& text) {
	std::string vehNameBuf{};
	bool writeChar{ false };

	bool exitLoop{ false };
	
	int charactersRead{ 0 };
	for (const char& c : text) {
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
	return { vehNameBuf, 0 };
}
const void Application::makeRepair(std::string& text, std::vector<Repair>& repairList) {
	//Buffers to write to
	uint32_t mileBuf{};
	std::ostringstream typeBuf{};
	double costBuf{};
	std::ostringstream notesBuf{};
	bool thirdPartyBuf{};

	std::string foundRepairBuf;				//buffer of characters being read from text
	std::vector<std::string> repairStrings; //vector of repairs not yet formatted

	bool writeChar{ false };
	int charactersRead{ 1 };
	for (char& c : text) {
		++charactersRead;
		if (c == '>') {
			repairStrings.push_back(foundRepairBuf);
			foundRepairBuf.erase();
			writeChar = false;
		}

		if (writeChar) {
			foundRepairBuf += c;
		}
		if (c == '<') {
			writeChar = true;
		}
	}


	for (std::string currentString : repairStrings) {
		char separator{ '|' };

		readUntil(currentString, separator, mileBuf);
		typeBuf << readUntil(currentString, separator);
		readUntil(currentString, separator, costBuf);
		readUntil(currentString, separator, thirdPartyBuf);
		notesBuf << readUntil(currentString, separator);
		
		repairList.push_back(Repair { mileBuf, typeBuf.str(), costBuf, notesBuf.str(),thirdPartyBuf} );

		mileBuf = 0;
		typeBuf.str("");
		costBuf = 0;
		thirdPartyBuf = false;
		notesBuf.str("");
	}

}
const void Application::makeGasStop(std::string& text, std::vector<GasStop>& gasList) {
	//Buffers to write to
	
	uint32_t mileBuf{};
	short gallonsBuf{};
	double ppgBuf{};
	std::ostringstream notesBuf{};

	std::string foundGasBuf;				//buffer of characters being read from text
	std::vector<std::string> gasStrings;	//vector of gas stops not yet formatted

	bool writeChar{ false };
	int charactersRead{ 1 };
	for (char& c : text) {
		++charactersRead;
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


	for (std::string currentString : gasStrings) {
		char separator{ '|' };

		readUntil(currentString, separator, mileBuf);
		readUntil(currentString, separator, ppgBuf);
		readUntil(currentString, separator, gallonsBuf);
		notesBuf << readUntil(currentString, separator);

		gasList.push_back(GasStop{ mileBuf, gallonsBuf, ppgBuf, notesBuf.str()});

		mileBuf = 0;
		gallonsBuf = 0;
		ppgBuf = 0;
		notesBuf.str("");
	}

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

		else{
			if (c != limit) {
				bufferText += c;
			}
			text.erase(0, charRead);
			return bufferText;
		}

	}
}

const std::ostringstream Application::ListVehicles() {
	std::ostringstream os;
	os << "   Vehicles:       Miles:\n";
	int i{ 1 };

	for (Vehicle& currentVehicle : m_vehicleList) {
		os << i << ". " << std::setw(currentVehicle.maxVehicleNameSize + 1) << std::left << currentVehicle.getName() << std::setw(6) << currentVehicle.getMileage() << "\n";
		++i;
		os << "\tRepairs:\n";
		for (Repair& rep : currentVehicle.getRepairList()) {
			os << '\t' << rep << "\n";
		}
		os << '\n';

		os << "\tGas Stops:\n";
		for (GasStop& gas : currentVehicle.getGasStopList()) {
			os << '\t' << gas << "\n";
		}
		os << "\n";
	}
	os << "\n";

	return os;
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

	// THIS DISPLAYS THE TIME IN HH : MM:SS format

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