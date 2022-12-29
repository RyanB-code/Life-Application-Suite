#include "Application.h"

#include <iostream> // Specifically for first time setup. Not needed in every header include

#pragma warning(disable : 4996)		// Disables use of time_t and tm in creating the Log file name

// Declare static variable
std::vector<Module*> Application::s_moduleList{};


// Needed for GLFW Setup
static void glfw_error_callback(int error, const char* description)
{
	// Log the error
	std::ostringstream logText;
	logText << "GLFW Error " << error << ": " << description;
   	RST::Log(logText.str(), LogCode::FATAL);
}

void Application::Startup() {

	// Ensure all proper subsystems are initialized
	if (SetupFileSystem()) {
		RST::Log("Application file system was setup successfully", LogCode::RUNTIME_HIGH);
		if(SetupGLFW()){
			RST::Log("GLFW was setup successfully", LogCode::RUNTIME_HIGH);
			if(SetupImGUI()){
				RST::Log("ImGUI was setup successfully", LogCode::RUNTIME_HIGH);
				if(SetupModules()){
					RST::Log("All Modules are setup", LogCode::RUNTIME_HIGH);
					m_initialized = true;
					RST::Log( "Application was successfully setup", LogCode::LOG_HIGH);
				}
			}
		}
	}
	
	// Exit if every subsystem could noe be initialized
	if(!m_initialized){
		throw RST::Log("Application initialization failed.", LogCode::FATAL);
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
	
	// Checks if member variable directories exist yet. If they do not exist, start first time setup
	if (!FileSystem::doesFileExist(getMainDirectory()) || !FileSystem::doesFileExist(getVehicleDirectory()))
	{
		if(!FirstTimeSetup()) { return false; }	// If first time setup was not successful, return FALSE thereby aborting further execution
	}		
	
	std::cout << getVehicleDirectory() << "\n";

	// Creates directories for the files if the files did exist/first time setup complete
	if (!FileSystem::createDirectory(getMainDirectory()) || !FileSystem::createDirectory(getVehicleDirectory()))
	{
		RST::Log("Could not initialize the file system.", LogCode::FATAL);
		success = false;
	}
	else 
	{
		// Once the child paths created, Initialize RST
		RST::init(DIRECTORY_PATH.string());
		RST::SetLogTarget(m_logTarget);
		RST::SetLogLevel(m_logLevel);
		
		success = true;
	}

	return success;
}
bool Application::SetupGLFW(){
	bool success {false};

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()){
		throw RST::Log("GLFW could not be initialized", LogCode::FATAL);
	}
	else{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

		m_window = glfwCreateWindow(m_window_x, m_window_y, WINDOW_TITLE.c_str(), NULL, NULL);
		if(!m_window){
			throw RST::Log("GLFW could not create window", LogCode::FATAL);
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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     	// Enable Keyboard Controls
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
	// Sets the Module's m_app member variables to this instance
	static Debug 			debugManager	{this};
	static Settings 		settings		{this};
	static VehicleManager 	vehicleManager 	{this};

	// The order of calling AddModule() will determine the order they are in the menu bar
	AddModule(&vehicleManager);
	AddModule(&settings);
	AddModule(&debugManager);

	// If there are no modules, exit
	if(s_moduleList.empty())
	{
		RST::Log("There are no Modules for this application", LogCode::FATAL);
		return false;
	}

	// Iterate through the module list and run the setup function
	for(Module* module : s_moduleList){
		if(!module->Setup()){
			std::string msg{"Could not setup [" + module->getName() + "] and did not add to Module list"};
			RST::Log(msg, LogCode::WARNING);
		}
		else { 
			RST::Log("Successfully initialized Module " + module->getName(), LogCode::LOG_HIGH);
		}
	}

	return true;
}
bool Application::FirstTimeSetup(){
	RST::Log("Necessary directories were not found. Performing first time setup.", LogCode::LOG_HIGH);
	bool success {false};

	std::string desiredDirectoryBuffer{};

	bool inputAccepted{false};
	do {
		bool confirmDesiredDirectoryAccepted{false};
		char desiredDirectoryInput{};


		system("cls");
		char input{};
		std::cout << "Required file directories were not found in " << this->getMainDirectory() << "\nWould you like to use this directory to store the application's files? (y/n)\n";
		std::cout << "  Note: This creates necessary folders within the Program Files folder of your computer.\n>";
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
					std::cout << "\n  Note: As of v0.2.0-WIP, the .exe needs to be in the same parent directory.\n  The folder containing the .exe will also contain the files\n>";
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

	std::filesystem::path pathBuffer{};

#ifdef _WIN32
	RST::Log("Windows OS was detected", LogCode::LOG_HIGH);
	
	#ifdef DEV
		RST::Log("Development version " + VERSION, LogCode::LOG_HIGH);
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);			// Windows specific call to return parent directory of the EXE
		pathBuffer = buffer;								// Gets the parent directory path
		return pathBuffer.parent_path().string() + "\\";	// return the directory name followed by a backslash
	#endif

	#ifndef DEV
		RST::Log("Release version " + VERSION, LogCode::LOG_HIGH);

		// If the OS is x64 bit windows, set to normal program files
		#ifdef _WIN64
			pathBuffer 	= "C:\\Program Files\\LAS\\";			
		#endif

		// If the OS is x32 bit windows, set to x86 program files
		#ifndef _WIN64
			pathBuffer = "C:\\Program Files (x86)\\LAS\\";
		#endif
	#endif

#endif


#ifndef _WIN32
	RST::Log("Windows OS was not detected. This version of LAS does not support other operating systems. Exiting Application", LogCode::LOG_HIGH);
	throw (true);
#endif

	return pathBuffer.string();
}
void Application::AssignPaths(const std::string parentPath){
	// Assigns the variables to be used
	DIRECTORY_PATH 	= parentPath;
	VEHICLE_PATH 	= DIRECTORY_PATH.string() + "Vehicles\\";	// Assign member variables but does NOT create the directory

	RST::Log("Set parent directory to " + parentPath, LogCode::LOG_HIGH);

	return;
}


// Modules that need Application definitions

void Settings::Display(){
	static bool cmd 	{false};

	static bool file	{false};
    if( ImGui::Begin("Settings", &m_shown, 0)){
		ImGui::Text("Main path:  \t\t%s", m_app->getMainDirectory().c_str());
		ImGui::Text("Vehicle path:   \t%s", m_app->getVehicleDirectory().c_str());

		ImGui::Separator();

		ImGui::NewLine();
		ImGui::Text("As of v0.0.1, this currently is not working");
		ImGui::Checkbox("CMD", &cmd); ImGui::SameLine();
		ImGui::Checkbox("File", &file);

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
		m_currentModuleWindowWidth = ImGui::GetWindowSize().x;

		// Forward declaration of Pop-Up Modal
		// Pop-up informing that Information could not be saved
		if(ImGuiMods::BeginPopupModal("Saving Failed")){
			ImGuiMods::CenterText("Could not save the Vehicle's Information.\nChanges were not made. Please try again!"); ImGui::NewLine(); 	// Message
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 50);
			if(ImGui::Button("Close", ImVec2(100, 30))) { ImGui::CloseCurrentPopup(); m_saveVehInfoFailed = false;}								// Close button
			ImGui::EndPopup();
		}

		// Dispays header
		ImGuiMods::Header("Vehicle Manager", "This Vehicle Manager stores all relevant information about a vehicle such as its name mileage,"
			" and repair and gas stop information. "); ImGui::Spacing(); ImGui::Spacing();
		

		if(vehListIsEmpty())	{ 
			ImGui::Text("There are no tracked vehicles");

			ImGui::NewLine();
			
			ImGui::Spacing();
			ImGui::TextWrapped("A repair can track:");
			ImGui::BulletText("Mileage it was done");
			ImGui::BulletText("The type of repair it was (oil change, light replacement, washer fluid fill, etc.)");
			ImGui::BulletText("How much the total cost was");
			ImGui::BulletText("Was it done by a third party?");
			ImGui::BulletText("Any notes you would like to add");
			ImGui::BulletText("And the date it was done");

			// If the Create Vehicle Window is true, center the CreateVehicle Window and not display the Create button
			if(m_showCreateVehicleWin){ ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - (m_createVehWinHeight / 2)); }
			else{
				// Center button horrizontally and vertically
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 100);
				ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - 25);
				if(ImGui::Button("Create Vehicle", ImVec2 (200, 50))) { m_showCreateVehicleWin = true; }
			}
		} // End if Vehicle List is empty
		else{

			// This draws the child window for the vehicle list and resizes
			if(ImGui::GetWindowWidth() > (m_mainWinWidthMax + m_detailedVehWinWidthMax + 20)){
				m_childWindowSameLine = true;	// If the window is large enough to hold both windows side by size, it does
				
				// Center the Tracked Vehicle window if it is the only window displayed
				if(!m_showDetailedVehWin) { ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentMainWinWidth); }
			}
			else{
				m_childWindowSameLine = false;	// If the window cannot have windows side by side, default spacing
				ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentMainWinWidth);
			}
			
			// Draw the Tracked Vehicles Window
			if(ImGuiMods::BeginResizeableChild("Tracked Vehicles", m_mainWinWidthMin, m_mainWinWidthMax, m_mainWinHeight)){
				m_currentMainWinWidth = ImGui::GetWindowWidth();

				ImGui::SameLine(); ImGuiMods::HelpMarker("Select a vehicle then choose from the options below"); ImGui::Spacing();	// Help marker
				m_selectedVehicle = SelectableVehicleList(m_delVehCalled);														// Show available vehicles list and return with the selected vehicle

				if(m_selectedVehicle){
					
					if(ImGui::Button("Create New Vehicle", ImVec2(m_currentMainWinWidth, 30))) { m_showCreateVehicleWin = true; }	// Make the button span the length of the window

					ImGui::NewLine(); ImGuiMods::CenterText("Vehicle Information");		// Header for second portion of window

					// Show number of Repairs and Add Repair button on the same line
					ImGui::Text("Total Repairs:\t   %d", m_selectedVehicle->getRepairList().size()); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_addRepOrGasStopButtonSize.x - m_winPadding);
					if(ImGui::Button("Add Repair", m_addRepOrGasStopButtonSize)){
						m_showAddRepairWin = true;
						m_showAddGasStopWin = false;
					}

					// Show number of Gas Stops and Add Gas Stop button on same line
					ImGui::Text("Total Gas Stops: \t%d", m_selectedVehicle->getGasStopList().size()); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_addRepOrGasStopButtonSize.x - m_winPadding);
					if(ImGui::Button("Add Gas Stop", m_addRepOrGasStopButtonSize)) { m_showAddGasStopWin = true; m_showAddRepairWin = false;	}

					ImGui::NewLine(); ImGui::Text("Last Updated: \t   %s", m_selectedVehicle->getLastUpdated().string().c_str());	// Displaying when the vehicle was last updated
					
					// If changes were made, display message saying changes need to be saved
					if(m_changesNeedToBeSaved){
						m_mainWinHeight = 450;
						ImGui::NewLine(); ImGui::NewLine(); ImGuiMods::CenterText(std::string{"Changes have been made to a Vehicle that have not yet been saved."}.c_str());
					}

					// Once/if vehicle is selected, these buttons appear. Display at bottom of screen
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - m_mainWinButtonSize.y - 20);

					// Based on which windows are open, change how many buttons are shown in order to size them appropriately
					if		( (m_showDetailedVehWin 	|| m_showAddRepairWin || m_showAddGasStopWin || m_showCreateVehicleWin) 	&& !m_changesNeedToBeSaved)	{ m_buttonsShown = 3; }
					else if ( (m_showDetailedVehWin 	|| m_showAddRepairWin || m_showAddGasStopWin || m_showCreateVehicleWin)	&& m_changesNeedToBeSaved)		{ m_buttonsShown = 4; }
					else if ( !m_showDetailedVehWin 	&& !m_showAddRepairWin && !m_showAddGasStopWin && !m_showCreateVehicleWin 	&& m_changesNeedToBeSaved)		{ m_buttonsShown = 3; }
					else																											{ m_buttonsShown = 2; }

					// Ensure buttons are at least the min size specified
					if((m_currentMainWinWidth - m_winPadding) < (m_minButtonSize * m_buttonsShown))	{ m_mainWinButtonSize.x = m_minButtonSize; }		// If the size of the main window is too small, set the button size to the minimum
					else{ m_mainWinButtonSize.x = (m_currentMainWinWidth - m_winPadding)  / m_buttonsShown; }	 											// Math the buttonSize to change with the size of the parent window
					
					ImGui::Spacing(); 	if(ImGui::Button("Detailed View", m_mainWinButtonSize)){m_showDetailedVehWin = true; }
					ImGui::SameLine(); 	if(ImGui::Button("Delete Vehicle", m_mainWinButtonSize)){ ImGui::OpenPopup("Delete?"); } // Open Pop-up
					
					// If changes have been made, show the Save All button
					if(m_changesNeedToBeSaved) { ImGui::SameLine(); if(ImGui::Button("Save All", m_mainWinButtonSize)) { ImGui::OpenPopup("Save All Vehicles?"); } }

					// Pop-up Modal for overwriting all Vehicle Information
					if(ImGuiMods::BeginPopupModal("Save All Vehicles?")){
						ImGui::Text("This will overwrite ALL Vehicle information.\n\nThis operation cannot be undone!\n\n");							// Message
						if(ImGui::Button("Save", ImVec2(120, 30))) { m_changesNeedToBeSaved = !SaveAllVehicles(); ImGui::CloseCurrentPopup(); }					// Savae Buttom
						ImGui::SetItemDefaultFocus(); ImGui::SameLine(); if(ImGui::Button("Cancel", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }	// Cancel button with default focus
						ImGui::EndPopup();
					}

					// Button to close the detailed info window
					if(m_showDetailedVehWin || m_showAddRepairWin || m_showAddGasStopWin || m_showCreateVehicleWin){
						ImGui::SameLine(); 
						if(ImGui::Button("Close All", m_mainWinButtonSize)){ m_showDetailedVehWin = false; m_showAddRepairWin = false; m_showAddGasStopWin = false; m_showCreateVehicleWin = false; }
					}

					if (ImGuiMods::BeginPopupModal("Delete?")) {
						// Change the message of the Popup depending if already failed to delete a Vehicle
						if(!m_delVehFailed){
							ImGuiMods::CenterText("This will delete all of the Vehicle's information\nThis operation cannot be undone!\n\n"); ImGui::Separator();

							if (ImGui::Button("Delete", ImVec2(120, 0))) {
								Vehicle vehBufferToDel = *m_selectedVehicle; // Assign buffer the selected Vehicle

								m_delVehCalled	 = true;																			// Throw the deleteVehicleCalled to reset other buffers
								m_selectedVehicle	= nullptr;																			// Set selectedVehicle to NULL
								m_showDetailedVehWin = false; m_showAddRepairWin = false; m_showAddGasStopWin = false; m_showCreateVehicleWin = false;	// Close other windows
								
								// If the vehicle could not be deleted, set flag to true
								if(!DeleteVehicle(vehBufferToDel)){ m_delVehFailed = true; }
								else{ m_selectedVehicle = nullptr; }

								ImGui::CloseCurrentPopup(); 
							}

							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(120, 0)))  { ImGui::CloseCurrentPopup(); m_delVehFailed = false; }		// If canceled, revert flags
						}
						else{
							ImGuiMods::CenterText("Failed to Delete Vehicle");
							if (ImGui::Button("Exit", ImVec2(120, 0))) 	 {  ImGui::CloseCurrentPopup(); m_delVehFailed = false; }
						}
						ImGui::EndPopup();
					} // End popup modal code 
				}
				else{
					// If there is no selected vehicle, display button at bottom of screen
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (m_currentMainWinWidth / 2));		// Center horizontally
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);								// Center vertically
					if(ImGui::Button("Create New Vehicle", ImVec2(m_currentMainWinWidth, 30))) { m_showCreateVehicleWin = true; }
				}
				ImGui::EndChild(); // Ending main window viewing tracked vehicles
			} // End Tracked Vehicles Window

			// Make the detailed window view be on the same line if it can hold them both side by side
			if(m_childWindowSameLine) { ImGui::SameLine(); }
			else {  ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentDetailedWinWidth); }

			// If there is a selected Vehicle
			if(m_selectedVehicle){
				if(m_showDetailedVehWin){
					// Set window sizes based on if the windows are on the same line
					if(!m_childWindowSameLine)	{ ImGuiMods::BeginResizeableChild("Full Vehicle Information", m_detailedVehWinWidthMin, 1920, 600); }
					else			{ ImGuiMods::BeginResizeableChild("Full Vehicle Information", m_detailedVehWinWidthMin, m_detailedVehWinWidthMax, 600); }
					m_currentDetailedWinWidth = ImGui::GetWindowWidth();

					ImGui::SameLine(); ImGuiMods::HelpMarker("This shows all the vehicle information"); ImGui::Spacing();	// Help marker

					// Buffers for text boxes to edit vehicle name and mileage
					static uint32_t milesBuf{0};
					static char 	nameBuf[64];

					static bool showMilesageTooBigMsg { false };

					// See if the selected vehicle has changed, and if so, throw flag to reset values
					static Vehicle* currentVehicle = nullptr; 
					if(currentVehicle != m_selectedVehicle){
						m_resetInputBufDetailedViewWin = true;
						currentVehicle = m_selectedVehicle;
					}

					// Resets the default variables
					if(m_resetInputBufDetailedViewWin){
						int i{0};
						for( char& c : nameBuf){ nameBuf[i] = NULL; ++i; } 							// Clear all of the nameBuffer
						i = 0;																		// Set index to front of array
						for(const char& c : m_selectedVehicle->getName())	{ nameBuf[i] = c; ++i; } 	// Write the Vehicles name to the buffer char by char to display in the text box
						milesBuf = m_selectedVehicle->getMileage();									// Set the buffer equal to the current Vehicle's mileage
						m_resetInputBufDetailedViewWin = false; 								// Once set, remove the flag to allow editing
					}

					// Show Name input box and Save Changes button on the same line
					ImGui::Spacing(); ImGui::Text("Name:  "); ImGui::SameLine();
					ImGui::InputText("##Name", nameBuf, 64); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_mainWinButtonSize.x - m_winPadding);
					if(ImGui::Button("Save Changes", m_mainWinButtonSize)) {
						if(nameBuf != m_selectedVehicle->getName())	{ ImGui::OpenPopup("Are You Sure You Want To Save?");	}				// Open Pop-up ensuring you want to change Vehicle's name
						else 										{ if(m_selectedVehicle->setMileage(milesBuf)) { m_changesNeedToBeSaved = true; }} 	// If just mileage is changed, update Vehicle and throw that changes were made
					}

					if(m_saveVehInfoFailed) { ImGui::OpenPopup("Saving Failed"); }

					if(ImGui::BeginPopupModal("Are You Sure You Want To Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
						if(!CheckStringSize(nameBuf, Vehicle::maxVehicleNameSize)) 	{ 
							ImGui::Text("Vehicle name out of range. Max size allowed is %d characters.\nVehicle was not created.", Vehicle::maxVehicleNameSize); 
							ImGui::NewLine();
							if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
						}
						else {
							ImGui::Text("This will overwrite current Vehicle Information to: ");
							ImGui::Text("Name:    %s\nMileage: %d", &nameBuf, milesBuf);
							ImGui::NewLine();

							if(ImGui::Button("Save", ImVec2(120, 30))) {	
								std::string oldName{m_selectedVehicle->getName()};									// Buffer to save current name in case renaming failed to revert back to original
								std::ostringstream oldFileName{m_app->getVehicleDirectory() + oldName + ".dat"};	// Old Vehicle file path
								std::ostringstream newFileName{m_app->getVehicleDirectory() + nameBuf + ".dat"};  	// Renamed target vehicle file path

								// If the Vehicle renaming was not accepted OR file name not valid, revert back to original name
								if(!m_selectedVehicle->setName(nameBuf) || !FileSystem::renameFile(oldFileName.str(), newFileName.str())){
									ImGui::CloseCurrentPopup();
									m_selectedVehicle->setName(oldName);
									m_saveVehInfoFailed = true;
								}
								else{
									// If vehicle renaming and file rename successful, change mileage, name and save to file
									m_selectedVehicle->setMileage(milesBuf);
									SaveVehicle(m_selectedVehicle);
									ImGui::CloseCurrentPopup();
								}
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if(ImGui::Button("Cancel", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
						}
						ImGui::EndPopup();
					}

					// Show Mileage text box and clear on the same line
					ImGui::Text("Miles: "); ImGui::SameLine();
					ImGui::InputScalar("##Miles",  ImGuiDataType_U32,  &milesBuf, nullptr, NULL, "%u"); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_mainWinButtonSize.x - m_winPadding); 
					if(ImGui::Button("Clear Inputs", m_mainWinButtonSize)){ m_resetInputBufDetailedViewWin = true; }

					if(milesBuf >= 500000) { milesBuf = 0; showMilesageTooBigMsg = true;} else {showMilesageTooBigMsg = false; }
					if(showMilesageTooBigMsg) 	{ImGui::Text("Mileage out of range. Mileage must be real. \nVehicle was not created");}

					ImGui::NewLine();
					ShowFullVehicleInformation(m_selectedVehicle);	// Display two tables with Repairs and Gas Stops
					ImGui::NewLine();

					// Center close button on the bottom  of window
					ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - (m_addRepOrGasStopButtonSize.x / 2) );
					if (ImGui::Button("Close", m_addRepOrGasStopButtonSize)) {m_showDetailedVehWin = false; }

					ImGui::EndChild();	
				} 
				else{ m_resetInputBufDetailedViewWin = true; }
				// End showing detailed vehicle window

				
				ImGui::NewLine();
				if(m_showAddRepairWin){
					ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentAddToWinWidth);
					if(ImGuiMods::BeginResizeableChild("Add Repair", m_addToVehicleWidthMin, m_addToVehicleWidthMax, 500)){
						m_currentAddToWinWidth = ImGui::GetWindowSize().x;

						AddRepair(m_selectedVehicle, m_changesNeedToBeSaved); // Display input fields for adding repair

						if(m_changesNeedToBeSaved) { ImGui::NewLine();	ImGuiMods::CenterText("Changes have been made that need to be saved to the file");	}

						// Center button on bottom of window
						ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - (m_mainWinButtonSize.x / 2) );
						ImGui::SetCursorPosY( ImGui::GetWindowHeight() - m_mainWinButtonSize.y - m_winPadding );
						if (ImGui::Button("Close", m_mainWinButtonSize)) {m_showAddRepairWin = false; }

						ImGui::EndChild();
					}
				}
		
				if(m_showAddGasStopWin){
					ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentAddToWinWidth);
					if(ImGuiMods::BeginResizeableChild("Add Gas Stop", m_addToVehicleWidthMin, m_addToVehicleWidthMax, 500)){
						m_currentAddToWinWidth = ImGui::GetWindowSize().x;

						AddGasStop(m_selectedVehicle, m_changesNeedToBeSaved);	// Display input fields for adding Gas Stop

						if(m_changesNeedToBeSaved){ ImGui::NewLine(); ImGuiMods::CenterText("Changes have been made that need to be saved to the file"); }

						ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - (m_mainWinButtonSize.x / 2) );
						ImGui::SetCursorPosY( ImGui::GetWindowHeight() - m_mainWinButtonSize.y - m_winPadding );
						if (ImGui::Button("Close", m_mainWinButtonSize)) {m_showAddGasStopWin = false; }
						
						ImGui::EndChild(); 
					}
				}
			}
		} // End if vehicle list is not empty

		if(m_showCreateVehicleWin){
			ImGui::NewLine();
			ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentCreateVehWidth);
			ImGuiMods::BeginResizeableChild("Create New Vehicle", m_createVehWidthMin, m_createVehWidthMax, m_createVehWinHeight);
			m_currentCreateVehWidth = ImGui::GetWindowWidth();

			static uint32_t mileBuf{0};
			static char 	nameBuf[64];

			static bool showMilesageTooBigMsg	{ false };					// If mileage entered > 500,000, show message saying mileage cannot be that high

			if(m_resetInputBufCreateVehWin){						// Resets the default variables
				int i{0};
				for( char& c : nameBuf) { nameBuf[i] = NULL; ++i; }			// Clear all of the nameBuffer
				i = 0;
				for(const char& c :"Vehicle Name") { nameBuf[i] = c; ++i; }	// Write the Vehicles name to the buffer to display in the text box
				mileBuf = 0;												// Reset mileage buffer to zero
				m_resetInputBufCreateVehWin = false; 				// Once set, remove the flag to allow editing
			}

			// Show name field and create on same line
			ImGui::NewLine(); ImGui::NewLine();
			ImGui::Text("Name:  "); ImGui::SameLine();
			ImGui::InputText("##Name", nameBuf, 64); ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150 - m_winPadding);
			if(ImGui::Button("Create", ImVec2(150, 30))) {
				ImGui::OpenPopup("Create New Vehicle?");
			}

			// Show mile field and clear on same line
			ImGui::Text("Miles: "); ImGui::SameLine();
			ImGui::InputScalar("##Miles",  ImGuiDataType_U32,  &mileBuf, nullptr, NULL, "%u");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150 - m_winPadding); 
			if(ImGui::Button("Clear Inputs",  ImVec2(150, 30))){ m_resetInputBufCreateVehWin = true; }

			ImGui::NewLine();

			// Check inputs for bad inputs
			if(mileBuf >= 500000) { mileBuf = 0; showMilesageTooBigMsg = true;} else {showMilesageTooBigMsg = false; }

			// Show if bad inputs were found
			if(showMilesageTooBigMsg) 	{ImGui::Text("Mileage out of range. Mileage must be real. \nVehicle was not created");}

			if(ImGui::BeginPopupModal("Create New Vehicle?", NULL, ImGuiWindowFlags_AlwaysAutoResize)){

				if(!CheckStringSize(nameBuf, Vehicle::maxVehicleNameSize)) 	{ 
					ImGui::Text("Vehicle name out of range. Max size allowed is %d characters.\nVehicle was not created.", Vehicle::maxVehicleNameSize); 
					ImGui::NewLine();
					if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
				}
				else{
					ImGui::NewLine();
					ImGui::Text("This will create a new Vehicle:");
					ImGui::Text("Name:    %s\nMileage: %d", &nameBuf, mileBuf);
					ImGui::NewLine();

					if(ImGui::Button("Create", ImVec2(120, 30))) {
						Vehicle* vehBuf = new Vehicle{nameBuf, mileBuf};

						addToVehicleList(*vehBuf);
						if(!SaveVehicle(vehBuf)) { ImGui::CloseCurrentPopup(); 	ImGui::OpenPopup("Saving Failed"); }
						else { 
							m_showCreateVehicleWin = false;			// Close the Child Window
							showMilesageTooBigMsg = false;		// Reset buffer for future calls to window
		
							ImGui::CloseCurrentPopup();
						}
						delete vehBuf;
					}
					ImGui::SameLine(); if(ImGui::Button("Close Wihtout Creating", ImVec2(215, 30))) { ImGui::CloseCurrentPopup(); }
					
				}

				ImGui::EndPopup();
			}

			ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - 75);
			ImGui::SetCursorPosY( ImGui::GetWindowHeight() - 40 );
			if (ImGui::Button("Close", ImVec2{150, 30})) {m_showCreateVehicleWin = false; }

			ImGui::EndChild();
		}
		else { m_resetInputBufCreateVehWin = true; }
		// End Create Vehicle Window


	}
	ImGui::End(); //End Vehicle Manager window
	
	return;
}
bool VehicleManager::SaveVehicle(Vehicle* veh) {
	std::ostringstream saveFileName{m_app->getVehicleDirectory() + veh->getName() + ".dat"};	// Sets the name to the Vehicle's m_name
	std::ostringstream fileText; 															// Buffer of what will be written to the file

	// Vehicle file "Header" portion of name and mileage
	fileText << '(' << veh->getName() << ')';
	fileText << '{' << veh->getMileage() << "}\n";
	
	for (Repair& repair : veh->getRepairList()) {
		fileText << '<';
		fileText << repair;
		fileText << '>';
		fileText << '\n';
	}
	for (GasStop& gasStop : veh->getGasStopList()) {
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