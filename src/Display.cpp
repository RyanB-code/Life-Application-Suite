#include "Display.h"

#pragma warning(disable : 4996)

namespace Display {

	void clear()
	{
		system("cls");
	}
	void clearLineAfterInput() {
		printf("\033[A"); //goes up a line
		printf("\33[2K"); //clears current line
	}

	int getInput(short low, short high) {
		int input{ 0 };

		bool exit{ false };
		while (!exit) {
			std::cout << ">";
			std::cin >> input;

			clearLineAfterInput();

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				if (input < low || input > high) {

				}
				else {
					exit = true;
				}
			}
		}

		return input;
	}
	bool getBoolInput() {
		bool success{false};

		bool exit{ false };
		while (!exit) {
			std::cout << "(y/n) >";
			char input;
			std::cin >> input;
			clearLineAfterInput();

			switch (input) {
			case 'y': case 'Y':
				std::cin.ignore(10000, '\n');
				success = true;
				exit = true;
				break;
			case 'n': case 'N':
				std::cin.ignore(10000, '\n');
				success = false;
				exit = true;
				break;
			default:
				std::cin.ignore(10000, '\n');
				exit = false;
				break;
			}

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				//Do Nothing
			}
		}

		return success;
	}
	
	//===========Menus/Screens=====================
	void Run(Application* app)
	{
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); //for testing

		while (!glfwWindowShouldClose(app->m_window))
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
			Home(app);
			//---------------------------------------------------
			// Rendering
			ImGui::Render();
			glfwGetFramebufferSize(app->m_window, &app->m_window_x, &app->m_window_y);
			glViewport(0, 0, app->m_window_x, app->m_window_y);
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
			glfwSwapBuffers(app->m_window);
		}

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(app->m_window);
		glfwTerminate();

		return;
	}

	void Home(Application* app)
	{
		// MenuBar variables. Must pass these variables to MenuBar();
		static bool showVehicleManager = false;
		static bool showSettings = false;
		static bool dockingEnabled = false;
		static bool showDemoWindow = false;
		static bool showDebugLog = false;

		ImGuiIO& io = ImGui::GetIO();
		dockingEnabled ? io.ConfigFlags |= ImGuiConfigFlags_DockingEnable : io.ConfigFlags = io.ConfigFlags & ~ImGuiConfigFlags_DockingEnable;
		if(showDemoWindow) ImGui::ShowDemoWindow();

		// Docking
		if(dockingEnabled){
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;


			if(ImGui::Begin("Main Dockspace", nullptr, window_flags)) {
				ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

				if(ImGui::BeginMenuBar()){
					MenuBar(showVehicleManager, showSettings, dockingEnabled, showDemoWindow, showDebugLog);
					ImGui::EndMenuBar();
				}
				ImGui::End();
			}
		}
		else {
			if(ImGui::BeginMainMenuBar()){
				MenuBar(showVehicleManager, showSettings, dockingEnabled, showDemoWindow, showDebugLog);
				ImGui::EndMainMenuBar();
			}
		}
		// -- End docking code
		

		if(showVehicleManager){
			VehicleManager(app, showVehicleManager);
		}
		if(showSettings){
			Settings(app, showSettings);
		}
		if(showDebugLog){
			DebugLog(showDebugLog);
		}
		

		return;
	}


	void MenuBar(bool& showVehMan, bool& showSettings, bool& dockingEnabled, bool &demoWindow, bool &debugLog){
		if(ImGui::BeginMenu("Modules")){
			ImGui::MenuItem("Vehicle Manager", nullptr, &showVehMan);
			ImGui::MenuItem("Debug Log", nullptr, &debugLog);
			ImGui::MenuItem("Settings", nullptr, &showSettings);
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("View")){
			ImGui::MenuItem("Docking", nullptr, &dockingEnabled);
			ImGui::MenuItem("Demo Window", nullptr, &demoWindow);
			ImGui::EndMenu();
		}
	}

	void VehicleManager(Application* app, bool &shown){
		if(ImGui::Begin("Vehicle Manager", &shown, 0)){
			Vehicle* selectedVehicle{ListSelectableVehicles(app->getVehicleList())};
		}
		ImGui::End();
	}
	void Settings(Application* app, bool &shown){
		if( ImGui::Begin("Settings", &shown, 0)){
			ImGui::Text("Another settings window");
		}
		ImGui::End();
	}
	void DebugLog(bool &shown){
		ImGuiTextBuffer     Buf;
   	 	ImGuiTextFilter     Filter;
    	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    	static bool         AutoScroll;  // Keep scrolling if already at the bottom.

		if(ImGui::Begin("Debug Log", &shown)){
			// Options menu
			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &AutoScroll);
				ImGui::EndPopup();
			}

			// Main window
			if (ImGui::Button("Options"))
				ImGui::OpenPopup("Options");
			ImGui::SameLine();
			bool clear = ImGui::Button("Clear");
			ImGui::SameLine();
			bool copy = ImGui::Button("Copy");
			ImGui::SameLine();
			Filter.Draw("Filter", -100.0f);

			ImGui::Separator();

			if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
			{	
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				const char* buf = Buf.begin();
				const char* buf_end = Buf.end();
				if (Filter.IsActive())
				{
					// In this example we don't use the clipper when Filter is enabled.
					// This is because we don't have random access to the result of our filter.
					// A real application processing logs with ten of thousands of entries may want to store the result of
					// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
					for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						if (Filter.PassFilter(line_start, line_end))
							ImGui::TextUnformatted(line_start, line_end);
					}
            	}
				else
				{
					ImGui::TextUnformatted("Currently does not work");
					/*
					ImGuiListClipper clipper;
					clipper.Begin(LineOffsets.Size);
					while (clipper.Step())
					{
						for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
						{
							const char* line_start = buf + LineOffsets[line_no];
							const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
							ImGui::TextUnformatted(line_start, line_end);
						}
					}
                	clipper.End();
					*/
				}

				ImGui::PopStyleVar();

				if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()){
					ImGui::SetScrollHereY(1.0f);
				}
				ImGui::EndChild();
       		}

		}
		ImGui::End();

	}


	Vehicle* ListSelectableVehicles(std::vector<Vehicle>& vehList) 
	{
		Vehicle* selVeh{nullptr};
		if(vehList.size() > 0){
			if (ImGui::TreeNodeEx("Current Vehicles", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				static int selected = -1;
				int vehNum{0};
				for (Vehicle& currentVehicle : vehList )
				{	
					++vehNum;
					char buf[32];
					sprintf(buf, "%d. %s", vehNum, currentVehicle.getName().c_str());
					if (ImGui::Selectable(buf, selected == vehNum))
					{
						selected = vehNum;
						selVeh = &currentVehicle;
					}
					ImGui::SameLine(200); ImGui::Text("%d miles", currentVehicle.getMileage()); 
				}
				ImGui::TreePop();
			}
		}
		else{
			ImGui::Text("There are no tracked vehicles");
		}
	
		return selVeh;
	}

	std::ostringstream ShowFullVehicleInformation(Vehicle* veh) {
		std::ostringstream os;

		if (!veh) {
			Log(LogCode::WARNING, "Could not show full vehicle information. Null pointer.");
		}
		else {
			short mileageWidth{ 8 };
			short typeWidth{ 15 };
			short costWidth{ 8 };
			short thirdPartyWidth{ 8 };
			short notesWidth{ 30 };
			int tableLineRepairs{ mileageWidth + typeWidth + costWidth + thirdPartyWidth + notesWidth + 12 }; //last number is number of separators

			short gallonsWidth{ 8 };
			int tableLineGas{ mileageWidth + gallonsWidth + costWidth + notesWidth + 9 };

			os << veh->getName() << " with " << veh->getMileage() << " miles\n";

			//The table header
			os << std::setw((tableLineRepairs / 2) + 7) << std::right << "===REPAIRS===" << std::left << '\n'; //make repairs be in the middle
			os << "   " << std::setw(mileageWidth) << "Miles"; os << " | ";
			os << std::setw(typeWidth) << "Type"; os << " | ";
			os << std::setw(costWidth) << "Cost"; os << " | ";
			os << std::setw(thirdPartyWidth) << "By Me?"; os << " | ";
			os << std::setw(notesWidth) << "Notes";
			os << std::setw(tableLineRepairs) << std::setfill('=');
			os << "\n   " << std::setfill(' ');

			//Fills in the table
			for (Repair& rep : veh->getRepairList()) {
				int				mileBuf;
				std::string		typeBuf;
				double			costBuf;
				std::string		notesBuf;
				bool			thirdPartyBuf;
				rep.getRepairInfo(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf);
				os << "\n   " << std::setw(mileageWidth) << std::right << mileBuf; os << " | ";
				os << std::setw(typeWidth) << typeBuf; os << " | ";
				os << '$' << std::setw(costWidth-1) << std::setprecision(2) << std::fixed << costBuf; os << " | ";
				os << std::setw(thirdPartyWidth) << std::boolalpha << thirdPartyBuf; os << " | ";
				os << std::setw(notesWidth) << std::left << notesBuf;
			}
			os << "\n\n";

			//The table header
			os << std::setw((tableLineGas / 2) + 8) << std::right << "===GAS STOPS===" << std::left << '\n'; //make text be in the middle
			os << "   " << std::setw(mileageWidth) << "Miles"; os << " | ";
			os << std::setw(gallonsWidth) << "Gallons"; os << " | ";
			os << std::setw(costWidth) << "PPG"; os << " | ";
			os << std::setw(notesWidth) << "Notes";
			os << std::setw(tableLineGas) << std::setfill('=');
			os << "\n   " << std::setfill(' ');

			//Fills in the table
			for (GasStop& gas : veh->getGasStopList()) {
				int			mileBuf;
				double		galBuf;
				double		costBuf;
				std::string notesBuf;
				gas.getGasStopInfo(mileBuf, galBuf, costBuf, notesBuf);
				os << "\n   " << std::setw(mileageWidth) << std::right << mileBuf; os << " | ";
				os << std::setw(gallonsWidth) << std::setprecision(2) << std::fixed << galBuf; os << " | ";
				os <<  '$' << std::setw(costWidth-1) << std::setprecision(2) << std::fixed << costBuf; os << " | ";
				os << std::setw(notesWidth) << std::left << notesBuf;
			}

			os << "\n\n";
		}

		return os;
	}
	
	bool AddVehicle(Application* app) {
		bool returnValue{false};
		
		std::string nameBuf;
		uint32_t mileBuf;

		std::cout << "Name\n>";
		std::cin.ignore(10000, '\n');
		std::getline(std::cin, nameBuf);

		bool exit{ false };
		do {
			std::cout << "Mileage >";
			std::cin >> mileBuf;
			clearLineAfterInput();

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				exit = true;
			}

		} while (!exit);

		std::cout << "Name:\t\t" << nameBuf << std::endl;
		std::cout << "Mileage:\t" << mileBuf << std::endl;

		std::cout << "\nSave Vehicle?\n";
		if (getBoolInput()) {
			Vehicle newVehicle{ nameBuf, mileBuf };
			app->NewVehicle(newVehicle);

			returnValue = true;
		}
		else {
			returnValue = false;
		}

		return returnValue;
	}
	bool AddRepair(Vehicle* veh) {
		bool success{false};

		uint32_t mileBuf{ 0 };
		std::string typeBuf;
		double costBuf{ 0.0 };
		std::string notesBuf;
		bool thirdPartyBuf;

		bool exit0{ false };
		do {
			bool exit1{ false };
			do {
				std::cout << "What mileage was the repair done at?\n>";
				std::cin >> mileBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit1 = true;
				}

			} while (!exit1);

			std::cout << "The Type of repair. [Max characters " << veh->maxRepairTypeSize << "]\nExamples: 'Oil Change', 'Power Steering', 'Body Work', etc...\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, typeBuf);


			bool exit2{ false };
			do {
				std::cout << "How much did it cost in total?\n>";
				std::cin >> costBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit2 = true;
				}

			} while (!exit2);

			std::cout << "Enter any notes here. [Max characters " << veh->maxNotesSize << "]\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, notesBuf);

		
			std::cout << "Did a third party perform the repair?" << std::endl;
			thirdPartyBuf = getBoolInput();

			std::cout << "Miles:\t\t" << mileBuf << std::endl;
			std::cout << "Type:\t\t" << typeBuf << std::endl;
			std::cout << "Cost:\t\t" << costBuf << std::endl;
			std::cout << "Notes:\t\t" << notesBuf << std::endl;
			std::cout << "3rd Party:\t" << std::boolalpha << thirdPartyBuf << std::endl;

			std::cout << "\nSave Repair?\n";
			if (getBoolInput()) {
				if (!veh->NewRepair(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf)) {
					std::cout << "\nCould not add repair to vehicle. \nTry Again?\n";
					if (!getBoolInput()) {
						exit0 = true;
					}
				}
				else {
					success = true;
					exit0 = true;
				}
			}
			else {
				exit0 = true;
			}
		} while (!exit0);

		return success;

	}
	bool AddGasStop(Vehicle* veh) {
		bool success{false};

		uint32_t mileBuf{ 0 };
		double galBuf{ 0.0 };
		double ppgBuf{ 0.0 };
		std::string notesBuf;

		bool exit0{ false };
		do {
			bool exit1{ false };
			do {
				std::cout << "What mileage did you fill up at?\n>";
				std::cin >> mileBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit1 = true;
				}

			} while (!exit1);

			bool exit2{ false };
			do {
				std::cout << "How many gallons did you fill up?\n>";
				std::cin >> galBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit2 = true;
				}

			} while (!exit2);

			bool exit3{ false };
			do {
				std::cout << "What was the price per gallon?\n>";
				std::cin >> ppgBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit3 = true;
				}

			} while (!exit3);

			std::cout << "Enter any notes here. [Max characters " << veh->maxNotesSize << "]\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, notesBuf);

			std::cout << std::setw(17) << std::right << "Miles:\t" << mileBuf << std::endl;
			std::cout << std::setw(17) << std::right << "Gallons:\t" << galBuf << std::endl;
			std::cout << std::setw(17) << std::right << "Price Per Gallon:\t" << ppgBuf << std::endl;
			std::cout << std::setw(17) << std::right << "Notes:\t" << notesBuf << std::endl;

			std::cout << "\nSave Gas Stop?\n";
			if (getBoolInput()) {
				if (!veh->NewGasStop(mileBuf, galBuf, ppgBuf, notesBuf)) {
					std::cout << "\nCould not add gas stop to vehicle. \nTry Again?\n";
					if (!getBoolInput()) {
						exit0 = true;
					}
				}
				else {
					success = true;
					exit0 = true;
				}
			}
			else {
				exit0 = true;
			}
		} while (!exit0);

		return success;
	}
}