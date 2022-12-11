#include "Application.h"

#include <iostream> // Specifically for first time setup

#pragma warning(disable : 4996)		// Disables use of time_t and tm in creating the Log file name

// Declare static variables
std::vector<Module*> Application::s_moduleList{};


// Needed for GLFW Setup
static void glfw_error_callback(int error, const char* description)
{
	std::ostringstream logText;
	logText << "GLFW Error " << error << ": " << description;
   	RST::Log(logText.str(), LogCode::ERROR);
}

void Application::Startup() {
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
	
	// Checks if directories exist yet. If they do not exist, start first time setup
	if (!FileSystem::doesFileExist(DIRECTORY_PATH.string()) 
			|| !FileSystem::doesFileExist(VEHICLE_PATH.string()))
	{
		if(!FirstTimeSetup()) { return false; }	// If first time setup was not successful, abort
	}
	
	// Creates directories for the files
	if (!FileSystem::createDirectory(DIRECTORY_PATH.string()) 
		|| !FileSystem::createDirectory(VEHICLE_PATH.string()))
	{
		RST::Log("Could not initialize the file system.", LogCode::FATAL);
		success = false;
	}
	else {
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


	// The order of adding will determine the order they are in the menu
	AddModule(&vehicleManager);
	AddModule(&settings);
	AddModule(&debugManager);

	// Iterate through the module list and setup
	for(Module* module : s_moduleList){
		if(!module->Setup()){
			std::string msg{"Could not setup " + module->getName() + ". Did not add to Module list"};
			RST::Log(msg, LogCode::WARNING);
		}
		else { 
			RST::Log("Successfully initialized Module " + module->getName(), LogCode::LOG_HIGH);
		}
	}

	success = true;

	return success;
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
	VEHICLE_PATH	= DIRECTORY_PATH.string() + "Vehicles\\";

	RST::Log("Set parent directory to " + parentPath, LogCode::LOG_HIGH);

	return;
}


// Modules that need App definitions

void Settings::Display(){
	static bool cmd 	{false};
	static bool file	{false};
    if( ImGui::Begin("Settings", &m_shown, 0)){
		ImGui::Text("Main path:  \t\t%s", m_app->getMainDirectory().c_str());
		ImGui::Text("Vehicle path:   \t%s", m_app->getVehicleDirectory().c_str());

		ImGui::Separator();

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

		static Vehicle* selectedVehicle{nullptr}; // Holds current selected vehicle information
		static bool changesMade {false};

		// Displaying child window variables
		static bool detailedVehView     {false};
		static bool addRepair 			{false};
		static bool addGasStop 			{false};
		static bool createVehicle		{false};
		// Child Window sizes

		const static float mainWinWidthMax 	{ 700 };					// The main Vehicle Tracker Window
		const static float mainWinWidthMin 	{ 500 };
		static float mainWinHeight			{ 400 };
		static float currentMainWinSize 	{mainWinWidthMax}; 			// Saves the current size

		const static float detailedVehWinWidthMax { 700 };				// For the Table view of Gas Stops and Repairs
		const static float detailedVehWinWidthMin { 500 };
		static float currentDetailedWinWidth {detailedVehWinWidthMax}; 	// Saves current size

		const static float addToVehicleWidthMax	{ 1000 }; 				// For Adding Gas Stop / Repair
		const static float addToVehicleWidthMin { 500 };
		static float currentAddToWinWidth {addToVehicleWidthMax}; 		// Saves current size

		const static float createVehWidthMax { 700 };
		const static float createVehWidthMin { 500 };
		static float currentCreateVehWidth {createVehWidthMax};

		static ImVec2 buttonSize {150.0f, 30.0f};

		// Used for Child Window Setup
		static bool sameLine { false };
		static float parentWindowWidth{};
		parentWindowWidth = ImGui::GetWindowSize().x;

		// Dispays header
		ImGuiMods::Header("Vehicle Manager", "This Vehicle Manager stores all relevant information about a vehicle such as its name mileage,"
			" and repair and gas stop information. ");

		ImGui::Spacing();
		ImGui::Spacing();

		if(vehListIsEmpty())	{ 
			ImGui::Text("There are no tracked vehicles"); 

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 100);
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - 25);
			if(ImGui::Button("Create Vehicle", ImVec2 (200, 50))) { createVehicle = true; }
		}
		else{

			// This draws the child window for the vehicle list and resizes
			if(ImGui::GetWindowWidth() > (mainWinWidthMax + detailedVehWinWidthMax + 20)){
				sameLine = true;	// If the window is large enough to hold both windows side by size, it does
				
				// Center the Tracked Vehicle window if it is the only window displayed
				if(!detailedVehView){ ImGuiMods::CenterChildWindow(parentWindowWidth, currentMainWinSize); }
			}
			else{
				sameLine = false;	// If the window cannot have windows side by side, default spacing
				ImGuiMods::CenterChildWindow(parentWindowWidth, currentMainWinSize);
			}
			
		
			if(ImGuiMods::BeginResizeableChild("Tracked Vehicles", mainWinWidthMin, mainWinWidthMax, mainWinHeight)){
				ImGui::SameLine(); ImGuiMods::HelpMarker("Select a vehicle then choose from the options below");
				ImGui::Spacing();
				currentMainWinSize = ImGui::GetWindowWidth();

				selectedVehicle = SelectableVehicleList();	// Show available vehicles list and return with selected vehicle
				static float vehWinX; vehWinX = ImGui::GetWindowContentRegionWidth();

				if(selectedVehicle){
					
					if(ImGui::Button("Create New Vehicle", ImVec2(vehWinX, 30))) { createVehicle = true; }
					ImGui::NewLine();

					static float buttonX_1 {150};

					ImGuiMods::CenterText("Vehicle Information");
					ImGui::Text("Total Repairs:\t   %d", selectedVehicle->getRepairList().size());

					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonX_1 - 10);
					if(ImGui::Button("Add Repair", ImVec2(buttonX_1, buttonSize.y))){
						addRepair = true;
						addGasStop = false;
					}

					ImGui::Text("Total Gas Stops: \t%d", selectedVehicle->getGasStopList().size());

					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonX_1 - 10);
					if(ImGui::Button("Add Gas Stop", ImVec2(buttonX_1, buttonSize.y))){
						addGasStop = true;
						addRepair = false;
					}

					ImGui::NewLine();
					ImGui::Text("Last Updated: \t   %s", selectedVehicle->getLastUpdated().string().c_str());
					
					if(changesMade){
						mainWinHeight = 450;
						ImGui::NewLine();
						ImGui::NewLine();

						ImGuiMods::CenterText(std::string{"Changes have been made to a Vehicle that have not yet been saved."}.c_str());
					}

					// Once/if vehicle is selected, these buttons appear. Display at bottom of screen
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - buttonSize.y - 20);

					//Size the buttons correctly
					constexpr float MIN_BUTTON_SIZE = 70;
					static float buttonX;
					static float winPadding{20};

					// See if there are 3 or 4 buttons shown
					static int buttonsShown {2};
					if		( (detailedVehView 	|| addRepair || addGasStop) && !changesMade)	{ buttonsShown = 3; }
					else if ( (detailedVehView 	|| addRepair || addGasStop)	&& changesMade)		{ buttonsShown = 4; }
					else if ( !detailedVehView && !addRepair && !addGasStop && changesMade)		{ buttonsShown = 3; }
					else																		{ buttonsShown = 2; }

					// Ensure buttons are at least the min size specified
					if((vehWinX - winPadding) < (MIN_BUTTON_SIZE * buttonsShown)){
						buttonX = MIN_BUTTON_SIZE;
					}
					else{ // Math the buttonSize to change with the size of the parent window
						buttonX = (vehWinX - winPadding)  / buttonsShown;	
					}

					buttonSize.x = buttonX;
					
					ImGui::Spacing();
					if(ImGui::Button("Detailed View", ImVec2(buttonX, buttonSize.y))){detailedVehView = true; }
					ImGui::SameLine();
					if(ImGui::Button("Delete Vehicle", ImVec2(buttonX, buttonSize.y))){ ImGui::OpenPopup("Delete?"); } // Open Pop-up
					ImGui::SameLine();
					
					if(changesMade) {
						if(ImGui::Button("Save All", buttonSize)) { 
							ImGui::OpenPopup("Save All Vehicles?");
						}
					}

					if(ImGui::BeginPopupModal("Save All Vehicles?", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
						ImGui::Text("This will overwrite ALL Vehicle information.\n\nThis operation cannot be undone!\n\n");
						if(ImGui::Button("Save", ImVec2(120, 30))) { changesMade = !SaveAllVehicles(); ImGui::CloseCurrentPopup(); }
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}

					// Button to close the detailed info window
					if(detailedVehView || addRepair || addGasStop){
						ImGui::SameLine(); 
						if(ImGui::Button("Close All", buttonSize)){
							detailedVehView = false;
							addRepair = false;
							addGasStop = false;
							createVehicle = false;
						}
					}

					// Pop-up Modal
					// Always center this window when appearing
					ImVec2 center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

					if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						static bool failed {false};
						if(!failed){
							ImGui::Text("This will delete all of the Vehicle's information\nThis operation cannot be undone!\n\n");
							ImGui::Separator();

							if (ImGui::Button("OK", ImVec2(120, 0))) 
							{
								Vehicle vehBufferToDel = *selectedVehicle;
								selectedVehicle = nullptr;
								detailedVehView = false;
								addRepair = false;
								addGasStop = false;
								if(!DeleteVehicle(vehBufferToDel)){
									failed = true;
								}
								ImGui::CloseCurrentPopup(); 
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(120, 0))) 
							{ 
								ImGui::CloseCurrentPopup();
								failed = false;
							}
						}
						else{
							ImGui::Text("Failed to Delete Vehicle");
							if (ImGui::Button("Exit", ImVec2(120, 0))) 
							{ 
								ImGui::CloseCurrentPopup(); 
								failed = false;
							}
						}
						
						ImGui::EndPopup();
					} // End popup modal code 


				}
				else{
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (vehWinX / 2));
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
					if(ImGui::Button("Create New Vehicle", ImVec2(vehWinX, 30))) { createVehicle = true; }
				}
				ImGui::EndChild(); // Ending main window viewing tracked vehicles
			}

			// Make the detailed window view be on the same line if it can hold them both side by side
			if(sameLine) { ImGui::SameLine(); }
			else {  ImGuiMods::CenterChildWindow(parentWindowWidth, currentDetailedWinWidth); }

			// If there is a selected Vehicle
			if(selectedVehicle){
				static bool defaultInputs{true}; // Used for displaying current info in the text boxes
				if(detailedVehView){
					// Set window sizes
					if(!sameLine)	{ ImGuiMods::BeginResizeableChild("Full Vehicle Information", detailedVehWinWidthMin, 1920, 600); }
					else			{ ImGuiMods::BeginResizeableChild("Full Vehicle Information", detailedVehWinWidthMin, detailedVehWinWidthMax, 600); }

					currentDetailedWinWidth = ImGui::GetWindowWidth();
					ImGui::SameLine(); ImGuiMods::HelpMarker("This shows all the vehicle information");
					ImGui::Spacing();

					// Edit Vehicle Portion
					static uint32_t milesBuffer{0};
					static char nameBuf[64];


					// See if the selected vehicle has changed
					static Vehicle* currentVehicle = nullptr; 
					if(currentVehicle != selectedVehicle){
						defaultInputs = true;
						currentVehicle = selectedVehicle;
					}

					// Resets the default variables
					if(defaultInputs){
						int i{0};
						// Clear all of the nameBuffer
						for( char& c : nameBuf){
							nameBuf[i] = NULL;
							++i;
						}
						i = 0;
						// Write the Vehicles name to the buffer to display in the text box
						for(const char& c : selectedVehicle->getName()){
							nameBuf[i] = c;
							++i;
						}
						milesBuffer = selectedVehicle->getMileage();	// Write the vehicle mileage to the buffer

						defaultInputs = false; // Once set, remove the flag to allow editing
					}

					// Actual GUI Drawing below
					ImGui::Spacing();
					ImGui::Text("Name:  "); ImGui::SameLine();
					ImGui::InputText("##Name", nameBuf, 64);
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonSize.x - 10);
					if(ImGui::Button("Save Changes", buttonSize)) {
						if(nameBuf != selectedVehicle->getName())	{ ImGui::OpenPopup("Are You Sure You Want To Save?");	}
						else 										{ if(selectedVehicle->setMileage(milesBuffer)) { changesMade = true; }} 
						
					}

					static bool showSaveFailed{false};
					if(ImGui::BeginPopupModal("Saving Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
						ImGui::Text("Could not save the Vehicle's Name.\nChanges were not made. Please try again!");
						ImGui::NewLine();
						if(ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); showSaveFailed= false;};
						ImGui::EndPopup();
					}
					if(showSaveFailed) { ImGui::OpenPopup("Saving Failed"); }

					if(ImGui::BeginPopupModal("Are You Sure You Want To Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
						ImGui::Text("This will overwrite the selected Vehicle information.\n\nThis operation cannot be undone!\n\n");

						if(ImGui::Button("Save", ImVec2(120, 30))) {
							std::string oldName{selectedVehicle->getName()};
							std::ostringstream oldFileName{m_app->getVehicleDirectory() + oldName + ".dat"};
							std::ostringstream newFileName{m_app->getVehicleDirectory() + nameBuf + ".dat"};  


							if(!selectedVehicle->setName(nameBuf) || !FileSystem::renameFile(oldFileName.str(), newFileName.str())){
								ImGui::CloseCurrentPopup();
								selectedVehicle->setName(oldName);
								showSaveFailed = true;
							}
							else{
								selectedVehicle->setMileage(milesBuffer);
								SaveVehicle(selectedVehicle);
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if(ImGui::Button("Cancel", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
						
						ImGui::EndPopup();
					}

			
					
					ImGui::Text("Miles: "); ImGui::SameLine();
					ImGui::InputScalar("##Miles",  ImGuiDataType_U32,  &milesBuffer, nullptr, NULL, "%u");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonSize.x - 10); 
					if(ImGui::Button("Clear", buttonSize)){ defaultInputs = true; }

					ImGui::NewLine();

					ShowFullVehicleInformation(selectedVehicle);


					ImGui::NewLine();
					ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - (buttonSize.x / 2) );
					if (ImGui::Button("Close", buttonSize)) {detailedVehView = false; }

					ImGui::EndChild();	

					
					
				}
				else{ defaultInputs = true; }

				
				ImGui::NewLine();
				if(addRepair){
					ImGuiMods::CenterChildWindow(parentWindowWidth, currentAddToWinWidth);
					if(ImGuiMods::BeginResizeableChild("Add Repair", addToVehicleWidthMin, addToVehicleWidthMax, 500)){
						currentAddToWinWidth = ImGui::GetWindowSize().x;

						AddRepair(selectedVehicle, changesMade);

						if(changesMade){
							ImGui::NewLine();
							ImGuiMods::CenterText("Changes have been made that need to be saved to the file");
						}

						ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - (buttonSize.x / 2) );
						ImGui::SetCursorPosY( ImGui::GetWindowHeight() - buttonSize.y -10 );
						if (ImGui::Button("Close", buttonSize)) {addRepair = false; }

						ImGui::EndChild();
					}
				}
		
				if(addGasStop){
					ImGuiMods::CenterChildWindow(parentWindowWidth, currentAddToWinWidth);
					if(ImGuiMods::BeginResizeableChild("Add Gas Stop", addToVehicleWidthMin, addToVehicleWidthMax, 500)){
						currentAddToWinWidth = ImGui::GetWindowSize().x;

						AddGasStop(selectedVehicle, changesMade);

						if(changesMade){
							ImGui::NewLine();
							ImGuiMods::CenterText("Changes have been made that need to be saved to the file");
						}

						ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - (buttonSize.x / 2) );
						ImGui::SetCursorPosY( ImGui::GetWindowHeight() - buttonSize.y -10 );
						if (ImGui::Button("Close", buttonSize)) {addGasStop = false; }
						
					ImGui::EndChild(); }
				}
			}
			else{

				ImGui::NewLine();
				
				ImGui::Spacing();
				ImGui::TextWrapped("A repair can track:");
				ImGui::BulletText("Mileage it was done");
				ImGui::BulletText("The type of repair it was (oil change, light replacement, washer fluid fill, etc.)");
				ImGui::BulletText("How much the total cost was");
				ImGui::BulletText("Was it done by a third party?");
				ImGui::BulletText("Any notes you would like to add");
				ImGui::BulletText("And the date it was done");
			}

		} // End if vehicle list is not empty

		if(createVehicle){
			if(ImGui::GetWindowWidth() > (createVehWidthMin + 20)){
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (currentCreateVehWidth / 2) );
			}

			ImGui::NewLine();
			ImGuiMods::BeginResizeableChild("Create New Vehicle", createVehWidthMin, createVehWidthMax, 350);
			currentCreateVehWidth = ImGui::GetWindowWidth();

			static uint32_t milesBuffer{0};
			static char nameBuf[64];

			static bool defaultInputs{true}; // Used for displaying current info in the text boxes
			if(defaultInputs){	// Resets the default variables
				int i{0};
				// Clear all of the nameBuffer
				for( char& c : nameBuf){
					nameBuf[i] = NULL;
					++i;
				}
				i = 0;
				// Write the Vehicles name to the buffer to display in the text box
				for(const char& c :"Vehicle"){
					nameBuf[i] = c;
					++i;
				}
				milesBuffer = 0;	// Write the vehicle mileage to the buffer

				defaultInputs = false; // Once set, remove the flag to allow editing
			}
			ImGui::NewLine(); ImGui::NewLine();
			ImGui::Text("Name:  "); ImGui::SameLine();
			ImGui::InputText("##Name", nameBuf, 64);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100 - 10);
			if(ImGui::Button("Create", ImVec2(100, 30))) {
				ImGui::OpenPopup("Create New Vehicle?");
			}
			ImGui::Text("Miles: "); ImGui::SameLine();
			ImGui::InputScalar("##Miles",  ImGuiDataType_U32,  &milesBuffer, nullptr, NULL, "%u");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 100 - 10); 
			if(ImGui::Button("Clear",  ImVec2(100, 30))){ defaultInputs = true; }

			if(ImGui::BeginPopupModal("Create New Vehicle?", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
				ImGui::NewLine();
				ImGui::Text("This will create a new Vehicle.");
				ImGui::NewLine();

				if(ImGui::Button("Create", ImVec2(120, 30))) {
					Vehicle vehBuf {nameBuf, milesBuffer};
					addToVehicleList(vehBuf);
					changesMade = true;
					ImGui::CloseCurrentPopup();
					createVehicle = false;
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if(ImGui::Button("Cancel", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
				
				ImGui::EndPopup();
			}


			ImGui::EndChild();
		}
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