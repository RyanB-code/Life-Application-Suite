#include "Display.h"

#pragma warning(disable : 4996)

namespace Display {
	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

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
		static bool showDemoWindow = false;
		static bool showDebugLog = false;

		if(showDemoWindow) ImGui::ShowDemoWindow();

		// Docking
		ImGuiIO& io = ImGui::GetIO();
		if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
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
					MenuBar(showVehicleManager, showSettings, showDemoWindow, showDebugLog);
					ImGui::EndMenuBar();
				}
				ImGui::End();
			}
		}
		else {
			if(ImGui::BeginMainMenuBar()){
				MenuBar(showVehicleManager, showSettings, showDemoWindow, showDebugLog);
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


	void MenuBar(bool& showVehMan, bool& showSettings, bool &demoWindow, bool &debugLog){
		if(ImGui::BeginMenu("Modules")){
			ImGui::MenuItem("Vehicle Manager", nullptr, &showVehMan);
			ImGui::MenuItem("Debug Log", nullptr, &debugLog);
			ImGui::MenuItem("Settings", nullptr, &showSettings);
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("View")){
			ImGui::MenuItem("Demo Window", nullptr, &demoWindow);
			ImGui::EndMenu();
		}
	}

	void VehicleManager(Application* app, bool &shown){
		if(ImGui::Begin("Vehicle Manager", &shown, 0)){
			static bool viewVeh = false;
			static bool editVeh = false;

			static Vehicle* selectedVehicle{nullptr};

			ImGui::TextWrapped(	"This Vehicle Manager stores all relevant information about a vehicle such as its name,"
								" mileage, and repair and gas stop information. ");
			ImGui::Spacing();
			ImGui::TextWrapped("A repair can track:");

			ImGui::BulletText("Mileage it was done");
			ImGui::BulletText("The type of repair it was (oil change, light replacement, washer fluid fill, etc.)");
			ImGui::BulletText("How much the total cost was");
			ImGui::BulletText("Was it done by a third party?");
			ImGui::BulletText("And any notes you would like to add");

			ImGui::Spacing();

			if(app->getVehicleList().empty()){
				ImGui::Text("There are no tracked vehicles");
			}
			else{	// This draws the child window for the vehicle list

				float bigWindowX = ImGui::GetWindowContentRegionMax().x;
				float childY = 200;

				static float childX {500};
				if(bigWindowX > 510){
					// Do nothing therefore keep window at 500 pixels
				}
				else{
					// If smaller than 510, rersize to the window size minus 10 pixels
					childX = bigWindowX - 10;
				}

				//Draw Tracked Vehicles window
				if(ImGui::GetContentRegionMax().x > 510){
					ImGui::Text("Tracked Vehicles");
					ImGui::SameLine(); HelpMarker("Select a vehicle then choose from the options below");
					ImGui::BeginChild("#Current Vehicles", ImVec2(childX, childY), false, ImGuiWindowFlags_AlwaysAutoResize);
				
					selectedVehicle = ListSelectableVehicles(app->getVehicleList());		// Display selectable list of vehicles
				}
				ImGui::EndChild();

				// Once/if vehicle is selected, these buttons appear
				if(selectedVehicle){
					static ImVec2 buttonSize {100, 30};
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
							Vehicle* vehBufferToDel = selectedVehicle;
							selectedVehicle = nullptr;
							viewVeh = false;
							editVeh = false;
							if(!app->deleteVehicle(vehBufferToDel)){
								
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
				if(viewVeh){
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
					ImGui::Text("View %s", selectedVehicle->getName().c_str());
					ImGui::SameLine(); HelpMarker("This shows all the vehicle information");
					ImGui::SameLine(); 


					if(ImGui::Button("Inc Width", (ImVec2 (90, 20)))){
						childX += 10;
					}
					ImGui::SameLine(); 
					if(ImGui::Button("Dec Width", (ImVec2 (90, 20)))){
						childX -= 10;
					}
					ImGui::BeginChild("#View Full Vehicle Info", ImVec2(childX, childY), false);
					ShowFullVehicleInformation(selectedVehicle);
					ImGui::EndChild();
				}

				if(editVeh){
					EditVehicle(selectedVehicle);
				}
			}
		}
		ImGui::End();
	}
	void Settings(Application* app, bool &shown){
		if( ImGui::Begin("Settings", &shown, 0)){
			ImGui::Text("Main path:  \t\t%s", app->showMainDirectory().c_str());
			ImGui::Text("Debug path: \t\t%s", app->showDebugDirectory().c_str());
			ImGui::Text("Vehicle path:   \t%s", app->showVehicleDirectory().c_str());
			ImGui::Text("Log File:   \t\t%s", app->showLogFilePath().c_str());
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
	void EditVehicle(Vehicle* veh){
		ImGui::Text("Editing %s", veh->getName().c_str());

		return;
	}

	Vehicle* ListSelectableVehicles(std::vector<Vehicle>& vehList) 
	{
		static Vehicle* selVeh{nullptr};
		if(vehList.size() > 0){
			if (ImGui::BeginListBox("Current Vehicles", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y-20))) 
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
				ImGui::EndListBox();
			}
		}
		else{
			ImGui::Text("There are no tracked vehicles");
		}
	
		return selVeh;
	}
	void ShowFullVehicleInformation(Vehicle* veh) {
		static const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);

		ImGui::Text("Repairs");
		ImGui::Spacing();
		if(ImGui::BeginTable("Repairs", 5, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg , outer_size ))
		{
			ImGui::TableSetupColumn("Miles", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Cost", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Third Party", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Notes", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableHeadersRow();

			for (Repair& rep : veh->getRepairList())
			{
				ImGui::TableNextRow();
				int				mileBuf;
				std::string		typeBuf;
				double			costBuf;
				std::string		notesBuf;
				bool			thirdPartyBuf;
				rep.getRepairInfo(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf);

				int column{0};

				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%d", mileBuf);
				++column;

				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%s", typeBuf.c_str());
				++column;
				
				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%f", costBuf);
				++column;
				
				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%s", thirdPartyBuf ? "true" : "false");
				++column;

				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%s", notesBuf.c_str());	
			}
		ImGui::EndTable();
		}

	ImGui::Spacing();

		ImGui::Text("Gas Stops");
		ImGui::Spacing();

		if(ImGui::BeginTable("Gas Stops", 4, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg , outer_size ))
		{
			ImGui::TableSetupColumn("Miles", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Gallons", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Per Gal", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableSetupColumn("Notes", ImGuiTableColumnFlags_WidthFixed, 100.0f);
			ImGui::TableHeadersRow();

			for (GasStop& gas : veh->getGasStopList())
			{
				ImGui::TableNextRow();

				int			mileBuf;
				double		galBuf;
				double		costBuf;
				std::string notesBuf;
				gas.getGasStopInfo(mileBuf, galBuf, costBuf, notesBuf);

				int column{0};

				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%d", mileBuf);
				++column;

				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%f", galBuf);
				++column;
				
				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%f", costBuf);
				++column;
				
				ImGui::TableSetColumnIndex(column);
				ImGui::Text("%s", notesBuf.c_str());	
			}
		ImGui::EndTable();
		}
		return;
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