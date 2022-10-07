#include "Application.h"

#pragma warning(disable : 4996)		// Disables use of time_t and tm in creating the Log file name

static void glfw_error_callback(int error, const char* description)
{
	std::ostringstream logText;
	logText << "GLFW Error " << error << ": " << description;
   	Log(LogCode::WARNING, logText.str());
}


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
			app = this;
			m_currentInstanceLogFile = debugFilePath.str();
			Log::m_path = m_currentInstanceLogFile.string();  //Sets the Log class m_path to m_currentInstanceLogFile in order for new log messages to be displayed there

			m_initialized = true;
			Log(LogCode::LOG, "Initialization successful.");
		}
	}
}
void Application::Startup() {
	if (!m_initialized) {
		throw Log(LogCode::FATAL, "Application initialization failed.");
		return;
	}
	else {		
	glfwSetErrorCallback(glfw_error_callback);		// Setup window
	if (!glfwInit()){
		throw Log(LogCode::FATAL, "GLFW could not be initialized");
	}
	else{
		const char* glsl_version = "#version 460";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

		m_window = glfwCreateWindow(m_window_x, m_window_y, WINDOW_TITLE.c_str(), NULL, NULL);
		if(!m_window){
			throw Log(LogCode::FATAL, "GLFW could not create window");
		}
			else {
				glfwMakeContextCurrent(m_window);
				gladLoadGL(); 
				glfwSwapInterval(m_vsync); // Enable vsync	

				// Setup Dear ImGui context
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				m_io = ImGui::GetIO();
			//	m_io = ImGui::GetIO(); (void)m_io; 							// I changed this to init m_io in App default constructor
				m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     	// Enable Gamepad Controls
				m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
				m_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
				//io.ConfigViewportsNoAutoMerge = true;
				//io.ConfigViewportsNoTaskBarIcon = true;
				ImGui::StyleColorsDark(); 									//Sets ImGUI style to dark

				// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
				ImGuiStyle& style = ImGui::GetStyle();
				if (m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					style.WindowRounding = 0.0f;
					style.Colors[ImGuiCol_WindowBg].w = 1.0f;
				}

				// Setup Platform/Renderer backends
				ImGui_ImplGlfw_InitForOpenGL(m_window, true);
				ImGui_ImplOpenGL3_Init(glsl_version);

				// -----------First party setup------------------

				//Go through the vehicle folder and store the names of the files
				std::vector <std::string> vehicleFiles;
				FileSystem::filesInDirectory(VEHICLE_PATH.string(), vehicleFiles);

				//Iterate through the files in the folder found and create Vehicle types
				for (const std::string& fileName : vehicleFiles) {
					std::ostringstream fileText;

					//Opens file, reads text, outputs to fileText
					if (!FileSystem::readFile(fileName, fileText)) {
						//If vehicle file couldn't be opened to read, do nothing
					}
					else {
						std::string vehInfoBuf{ fileText.str() };	//Stores the file text in string format
						Vehicle vehicleBuffer{ makeVehicleName(vehInfoBuf), makeVehicleMiles(vehInfoBuf) };
					
						makeRepair(vehInfoBuf, vehicleBuffer);		//Make repairs from the remaining text and add to vehicle
						makeGasStop(vehInfoBuf, vehicleBuffer);		//Make Gas stop from the remaining text

						NewVehicle(vehicleBuffer);					//Add vehicleBuffer to the master application m_vehicleList
					}
				}		
			}
		}
	}

	Log(LogCode::ROUTINE, "Startup processes finised.");
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

//========END PUBLIC=========

//==========PRIVATE==========
const std::string Application::makeVehicleName(std::string& text) {
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
			return vehNameBuf; 
		}

	}
	return vehNameBuf;
}
const uint32_t Application::makeVehicleMiles(std::string& text){
	if (text == "") { //Check if the text string exists
		return 0;
	}

	std::stringstream mileStringBuf{};
	uint32_t mileBuf;

	bool writeChar{ false };
	bool exitLoop{ false };
	int charactersRead{ 0 };
	for (const char& c : text) { //Reads text between ( ) to write to vehNameBuf and deletes read characters
		++charactersRead;
		if (c == '}') {
			writeChar = false;
			exitLoop = true;
		}
		if (writeChar) {
			mileStringBuf << c;
		}
		if (c == '{') {
			writeChar = true;
		}

		if (exitLoop) {
			for (int i{ 0 }; i < charactersRead; ++i) {
				text.erase(0, 1);
			}
			mileStringBuf >> mileBuf;
			return mileBuf;
		}

	}
	return mileBuf;
}
const void Application::makeRepair(std::string& text, Vehicle& veh) {
	std::string repBuf;					//Buffer of characters being read from text
	std::vector<std::string> repairStrings; 		//Vector of strings of repair info not yet formatted

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
		
		veh.NewRepair(mileBuf, typeBuf.str(), costBuf, notesBuf.str(), thirdPartyBuf);
	}

}
const void Application::makeGasStop(std::string& text, Vehicle& veh) {

	std::string foundGasBuf;				//Buffer of characters being read from text
	std::vector<std::string> gasStrings;			//Vector of gas stops not yet formatted

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
		double				gallonsBuf{};
		double				ppgBuf{};
		std::ostringstream	notesBuf{};

		readUntil(currentString, separator, mileBuf);
		readUntil(currentString, separator, ppgBuf);
		readUntil(currentString, separator, gallonsBuf);
		notesBuf << readUntil(currentString, separator);

		veh.NewGasStop(mileBuf, gallonsBuf, ppgBuf, notesBuf.str());
	}

	return;
}

void Application::readUntil(std::string& text, const char limit, auto& returnType) {
	std::stringstream bufferText;

	int charRead{0};
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
	for (char& c : text) {
		++charRead;
		if (c != limit && charRead < text.length()) {	//Make sure c is not the limit, and that it does not go over the end of the string.
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

	return bufferText;
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

	return date;
}
//===========================
