#include "Application.h"

#pragma warning(disable : 4996)		// Disables use of time_t and tm in creating the Log file name

// Declare static variables
std::vector<Module*> Application::s_moduleList{};


// Needed for GLFW Setup
static void glfw_error_callback(int error, const char* description)
{
	std::ostringstream logText;
	logText << "GLFW Error " << error << ": " << description;
   	Log(LogCode::WARNING, logText.str());
}

void Application::Startup() {
	if (SetupFileSystem()) {
		if(SetupGLFW()){
			if(SetupImGUI()){
				if(SetupModules()){
					m_initialized = true;
					Log(LogCode::LOG, "Application was successfully setup");
				}
			}
		}
	}
	
	if(!m_initialized){
		throw Log(LogCode::FATAL, "Application initialization failed.");
		return;
	}
}
void Application::Run()
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); //for testing

	while (!glfwWindowShouldClose(this->m_window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// My code goes here for window calls ------------
		Home(this);
		// -----------------------------------------------

		// Rendering
		ImGui::Render();
		glfwGetFramebufferSize(this->m_window, &this->m_window_x, &this->m_window_y);
		glViewport(0, 0, this->m_window_x, this->m_window_y);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		glfwSwapBuffers(this->m_window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->m_window);
	glfwTerminate();

	return;
}

//-----END PUBLIC-------


//-----PRIVATE-----
bool Application::SetupFileSystem(){
	bool success{false};

	AssignPaths(getExeParentPath());
	
	// Checks if directories exist yet. If they do not exist, start first time setup
	if (!FileSystem::doesFileExist(DIRECTORY_PATH.string()) 
			|| !FileSystem::doesFileExist(DEBUG_PATH.string()) 
			|| !FileSystem::doesFileExist(VEHICLE_PATH.string()))
	{
		if(!FirstTimeSetup()) { return false; }	// If first time setup was not successful, abort
	}
	
	// Creates directories for the files
	if (!FileSystem::createDirectory(DIRECTORY_PATH.string()) 
		|| !FileSystem::createDirectory(DEBUG_PATH.string()) 
		|| !FileSystem::createDirectory(VEHICLE_PATH.string()))
	{
		Log(LogCode::FATAL, "Could not initialize the file system.");
		success = false;
	}
	else {
		// Once the app folders are made,
		// Make debug log file for the instance
		std::ostringstream debugFilePath{ DEBUG_PATH.string() + LogFileName().str() + ".dat" };	

		if (!FileSystem::createFile(debugFilePath.str())) { Log(LogCode::FATAL, "Could not create debug log file."); }
		else {
			m_currentInstanceLogFile = debugFilePath.str(); 	// Sets the current Applications log file to the name created in this function
			Log::m_path = m_currentInstanceLogFile.string();  	//Sets the Log class m_path to m_currentInstanceLogFile in order for new log messages to be displayed there

			success = true;
		}
	}

	return success;
}
bool Application::SetupGLFW(){
	bool success {false};

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()){
		throw Log(LogCode::FATAL, "GLFW could not be initialized");
	}
	else{
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
			glfwSetWindowSizeLimits(m_window, MIN_WIN_SIZE.x, MIN_WIN_SIZE.y, GLFW_DONT_CARE, GLFW_DONT_CARE);
			gladLoadGL(); 
			glfwSwapInterval(m_vsync); // Enable vsync
			success = true;
		}
	}
	return success;
}
bool Application::SetupImGUI(){
	const char* glsl_version = "#version 460";
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     	// Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.FontGlobalScale = (1.3f); 
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
				

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	appStyle = &ImGui::GetStyle();
	if (io.ConfigFlags)
	{
		appStyle->WindowRounding = 0.0f;
		appStyle->WindowBorderSize = 1.0f;
		appStyle->FramePadding = ImVec2(8, 4);

		appStyle->Colors[ImGuiCol_MenuBarBg] 		= ImColor(0, 162, 237, 255);
		appStyle->Colors[ImGuiCol_HeaderHovered] 	= ImColor(30, 30, 30, 255);

		appStyle->Colors[ImGuiCol_TitleBg] 			= ImColor(115, 115, 115, 255);
		appStyle->Colors[ImGuiCol_TitleBgActive] 	= ImColor(115, 115, 115, 255);
		appStyle->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(115, 115, 115, 135);

		appStyle->Colors[ImGuiCol_TabUnfocused] 		= ImColor(60, 60, 60, 255);
		appStyle->Colors[ImGuiCol_TabUnfocusedActive] 	= ImColor(60, 60, 60, 255);
		appStyle->Colors[ImGuiCol_TabHovered] 			= ImColor(60, 60, 60, 135);
		appStyle->Colors[ImGuiCol_TabActive] 			= ImColor(30, 30, 30, 255);
		appStyle->Colors[ImGuiCol_Tab] 					= ImColor(60, 60, 60, 255);
		appStyle->TabRounding = 0.0f;
	}

	return true;
}
bool Application::SetupModules(){
	bool success{false};

	static Debug 			debugManager	{this};
	static Settings 		settings		{this};
	static VehicleManager 	vehicleManager 	{this};


	AddModule(&debugManager);
	AddModule(&settings);
	AddModule(&vehicleManager);

	// Iterate through the module list and setup
	for(Module* module : s_moduleList){
		if(!module->Setup()){
			std::string msg{"Could not setup " + module->getName() + ". Did not add to Module list"};
			Log(LogCode::WARNING, msg);
		}
		else { 
			Log(LogCode::ROUTINE, "Successfully initialized Module " + module->getName());
		}
	}

	success = true;

	return success;
}
bool Application::FirstTimeSetup(){
	Log(LogCode::WARNING, "Necessary directories were not found. Performing first time setup.");
	bool success {false};

	std::string desiredDirectoryBuffer{};

	bool inputAccepted{false};
	do {
		bool confirmDesiredDirectoryAccepted{false};
		char desiredDirectoryInput{};


		system("cls");
		char input{};
		std::cout << "Required file directories were not found.\nWould you like to use default directories? (y/n)\n";
		std::cout << "  Note: This creates necessary folders within the parent folder of this .exe\n> ";
		std::cin  >> input;

		switch(input){
			case 'y': case 'Y':
				inputAccepted = true;
				success = true;
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				// Do nothing thereby using the already assignged folder paths of where the exe is
			break;
			case 'n': case 'N':
				std::cin.clear();
				std::cin.ignore(1000, '\n');

				// Enter desired file path AND confirm it loop
				do{
					system("cls");
					std::cout << "Enter desired parent directory for files: ";
					std::cout << "  Note: As of v0.0.1, the .exe needs to be in the same parent directory.\n  The folder containing the .exe will also contain the files";
					std::getline(std::cin, desiredDirectoryBuffer);

					// Ensure there is a backslash at the end
					if(desiredDirectoryBuffer.back() != '\\' && desiredDirectoryBuffer.back() != '/'){
						desiredDirectoryBuffer += "\\";
					}
							
					std::cout << "\nDesired directory >\n" << desiredDirectoryBuffer << "\n\n";
					std::cout << "Are you sure this is correct? (y/n)\n";
					std::cin >> desiredDirectoryInput;

					switch(desiredDirectoryInput){
						case 'y': case 'Y':
							confirmDesiredDirectoryAccepted = true;
							success = true;
							inputAccepted = true;
							AssignPaths(desiredDirectoryBuffer); // Assign the paths to the confirmed string
							std::cin.clear();
							std::cin.ignore(1000, '\n');
						break;
						case 'n': case 'N':
							confirmDesiredDirectoryAccepted = false;
							std::cin.clear();
							std::cin.ignore(1000, '\n');
						break;
						default:
							std::cout << "\nInvalid input. Restarting...";
							confirmDesiredDirectoryAccepted = false;
							Sleep(2000);
						break;
					}
				}
				while(!confirmDesiredDirectoryAccepted);
				
			break;
			default:
				std::cout << "\nInvalid input. Restarting...";
				inputAccepted = false;
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				Sleep(2000);
			break;
		}
	}
	while(!inputAccepted);

	system("cls");

	return success;
}

std::string Application::getExeParentPath() const {
	// Gets the EXE file path
	char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::filesystem::path pathBuffer{buffer}; // Gets the parent directory path

	return pathBuffer.parent_path().string() + "\\";	
}
void Application::AssignPaths(std::string parentPath){
	// Assigns the variables to be used
	DIRECTORY_PATH 	= parentPath; // Makes the home directory the exePath's
	DEBUG_PATH 		= DIRECTORY_PATH.string() + "Debug\\";
	VEHICLE_PATH	= DIRECTORY_PATH.string() + "Vehicles\\";

	Log(LogCode::LOG, "Set parent directory to " + parentPath);

	return;
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
		date << "FEB";
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
//--------------------


// Modules that need App definitions

void Settings::Display(){
    if( ImGui::Begin("Settings", &m_shown, 0)){
            ImGui::Text("Main path:  \t\t%s", m_app->getMainDirectory().c_str());
            ImGui::Text("Debug path: \t\t%s", m_app->getDebugDirectory().c_str());
            ImGui::Text("Vehicle path:   \t%s", m_app->getVehicleDirectory().c_str());
            ImGui::Text("Log File:   \t\t%s", m_app->getLogFilePath().c_str());
        }
        ImGui::End();
}

bool VehicleManager::Setup(){

	// Go through the vehicle folder and store the names of the files
	std::vector<std::string> vehicleFiles;
	FileSystem::filesInDirectory(m_app->getVehicleDirectory(), vehicleFiles);

	// Iterate through the files in the folder found and create Vehicle types
	for (const std::string& fileName : vehicleFiles) {
		std::ostringstream fileText;

		//Opens file, reads text, outputs to fileText
		if (!FileSystem::readFile(fileName, fileText)) {
			// If vehicle file couldn't be opened to read, do nothing
			// Unsuccessful open logged in readFile()
		}
		else {
			std::string vehInfoBuf{ fileText.str() };	//Stores the file text in string format
			Vehicle vehicleBuffer{ MakeVehicleName(vehInfoBuf), MakeVehicleMiles(vehInfoBuf) };
		
			MakeRepair(vehInfoBuf, vehicleBuffer);		//Make repairs from the remaining text and add to vehicle
			MakeGasStop(vehInfoBuf, vehicleBuffer);		//Make Gas stop from the remaining text

			addToVehicleList(vehicleBuffer);			//Add vehicleBuffer to the master vehicle list
		}
	}

	return true;
}
void VehicleManager::Display() {
	if(ImGui::Begin("Vehicle Manager", &m_shown, 0)){
		static bool viewVeh = false;
		static bool editVeh = false;

		static Vehicle* selectedVehicle{nullptr};

		// Welcome message at top of window -------
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth   = ImGui::CalcTextSize("Vehicle Manager").x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text("Vehicle Manager");
		ImGui::Spacing();

		ImGui::TextWrapped(	"This Vehicle Manager stores all relevant information about a vehicle such as its name,"
							" mileage, and repair and gas stop information. ");
		ImGui::Spacing();
		ImGui::Spacing();
		// ----------------------------------------


		if(vehListIsEmpty()){
			ImGui::Text("There are no tracked vehicles");
		}
		else{	// This draws the child window for the vehicle list


			// Algorithm for adjusting child window size --------
			static float bigWindowX;
			bigWindowX = ImGui::GetWindowContentRegionMax().x;
			static float childY = 200;

			static float childX {500};
			if(bigWindowX > 510){
				// Do nothing therefore keep window at 500 pixels if the parent window is larger than specified
			}
			else{
				childX = bigWindowX - 10;	// If smaller than 510, rersize to the window size minus 10 pixels
			}
			// -------------------------------------------------

			float vehWinX; //Forward declaration to get the size of the vehicle draw window to size buttons appropriately

			// Create and draw the child window
			ImGui::Text("Tracked Vehicles");
			ImGui::SameLine(); ImGui::HelpMarker("Select a vehicle then choose from the options below");
			ImGui::BeginChild("#Current Vehicles", ImVec2(childX, childY), false, ImGuiWindowFlags_AlwaysAutoResize);
			selectedVehicle = SelectableVehicleList();
			vehWinX = ImGui::GetWindowContentRegionWidth();
			ImGui::EndChild();
			
			// Once/if vehicle is selected, these buttons appear
			if(selectedVehicle){

				//Size the buttons correctly
				constexpr float MIN_BUTTON_SIZE = 70;
				static float buttonX;
				static float buttonY{30};
				static ImVec2 buttonSize (0, buttonY);
				static float winPadding{20};

				// See if there are 3 or 4 buttons shown
				static int buttonsShown {3};
				if(viewVeh || editVeh){
					buttonsShown = 4;
				}
				else{
					buttonsShown = 3;
				}

				// Ensure buttons are at least the min size specified
				if((vehWinX - winPadding) < (MIN_BUTTON_SIZE * buttonsShown)){
					buttonX = MIN_BUTTON_SIZE;
				}
				else{ // Math the buttonSize to change with the size of the parent window
					buttonX = (vehWinX - winPadding)  / buttonsShown;	
				}

				buttonSize.x = buttonX;
				
				ImGui::Spacing();
				if(ImGui::Button("View", buttonSize)){
					editVeh = false;
					viewVeh = true;
				}
				ImGui::SameLine();
				if(ImGui::Button("Edit", buttonSize)){
					viewVeh = false;
					editVeh = true;
				}
				ImGui::SameLine();  
				if(ImGui::Button("Delete", buttonSize)){
					ImGui::OpenPopup("Delete?");
				}

				// Code for popup modal
				// Always center this window when appearing
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("This will delete all of the Vehicle's information\nThis operation cannot be undone!\n\n");
					ImGui::Separator();

					if (ImGui::Button("OK", ImVec2(120, 0))) 
					{
						Vehicle vehBufferToDel = *selectedVehicle;
						selectedVehicle = nullptr;
						viewVeh = false;
						editVeh = false;
						if(DeleteVehicle(vehBufferToDel)){
							Log(LogCode::WARNING, "Failed to delete vehicle. In Display.cpp");
						}
						ImGui::CloseCurrentPopup(); 
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) 
					{ 
						ImGui::CloseCurrentPopup(); 
					}
					ImGui::EndPopup();
				} // End popup modal code 


				// Button to close the detailed info window
				if(viewVeh || editVeh){
					ImGui::SameLine(); 
					if(ImGui::Button("Close", buttonSize)){
						viewVeh = false;
						editVeh = false;
					}
				}
			} 


			//What happens when a vehicle action button is hit
			if(viewVeh || editVeh){
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				static float bigWindowX = ImGui::GetWindowContentRegionMax().x;				

				static float childX {700};
				static float childY {410};
				if(bigWindowX > 710){

				}
				else{
					childX = bigWindowX - 20;
				}

				//Starting new child window with vehicle info
				if(viewVeh) ImGui::Text("View %s", selectedVehicle->getName().c_str());
				if(editVeh) ImGui::Text("Edit %s", selectedVehicle->getName().c_str());
				ImGui::SameLine(); ImGui::HelpMarker("This shows all the vehicle information");
				ImGui::SameLine(); 


				if(ImGui::Button("Inc Width", (ImVec2 (90, 20)))){
					childX += 10;
				}
				ImGui::SameLine(); 
				if(ImGui::Button("Dec Width", (ImVec2 (90, 20)))){
					childX -= 10;
				}

				if(viewVeh){
					if(!selectedVehicle){ 
						//Ensure there is a selected Vehicle
					}
					else{
						if(ImGui::BeginChild("#View Full Vehicle Info", ImVec2(childX, childY), false)){
							ShowFullVehicleInformation(selectedVehicle);
						}
						ImGui::EndChild();
					}
					
				}

				if(editVeh){
					if(ImGui::BeginChild("#Edit Vehicle Info", ImVec2(childX, childY), false)){
						EditVehicle(selectedVehicle);
					}
					ImGui::EndChild();
				}

			}
			else{
				ImGui::Spacing();
				ImGui::TextWrapped("A repair can track:");
				ImGui::BulletText("Mileage it was done");
				ImGui::BulletText("The type of repair it was (oil change, light replacement, washer fluid fill, etc.)");
				ImGui::BulletText("How much the total cost was");
				ImGui::BulletText("Was it done by a third party?");
				ImGui::BulletText("Any notes you would like to add");
				ImGui::BulletText("And the date it was done");
			}
		}
	}
	ImGui::End(); //End Vehicle Manager window
	return;
}
bool VehicleManager::WriteToFile(const Application& app, Vehicle& veh) {
	std::ostringstream saveFileName{app.getVehicleDirectory() + veh.getName() + ".dat"};	// Sets the name to the Vehicle's m_name
	std::ostringstream fileText; 															// Buffer of what will be written to the file

	// Vehicle file "Header" portion of name and mileage
	fileText << '(' << veh.getName() << ')';
	fileText << '{' << veh.getMileage() << "}\n";
	
	for (Repair& repair : veh.getRepairList()) {
		fileText << '<';
		fileText << repair;
		fileText << '>';
		fileText << '\n';
	}
	for (GasStop& gasStop : veh.getGasStopList()) {
		fileText << '[';
		fileText << gasStop;
		fileText << ']';
		fileText << '\n';
	}
	
	return FileSystem::writeToFile(saveFileName.str(), fileText.str());
}
bool VehicleManager::DeleteVehicle	(Vehicle& veh){
	delFromVehList(veh); // Delete from s_VehicleList

	// Delete the actual file itself
	std::string deletePath{m_app->getVehicleDirectory() + veh.getName() + ".dat"}; 
	return FileSystem::deleteFile(deletePath);
}