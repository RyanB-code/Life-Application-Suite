#include "Home.h"


void Home(Application* app)
{
	// State variables for displaying modules
	static bool showVehicleManager 	= false;
	static bool showSettings 		= false;
	static bool showDebugLog 		= false;

	// Intent to remove in release ---------
	static bool showDemoWindow 		= false;
	if(showDemoWindow) 			{ ImGui::ShowDemoWindow(); }
	// -------------------------------------


	static ImGuiIO& io = ImGui::GetIO();
	// Create Docking Window over full viewport----------------------------------------
	if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
		static const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;


		if(ImGui::Begin("Main Dockspace", nullptr, window_flags)) {
			static ImGuiID dockspace_id;
			dockspace_id = ImGui::GetID("MainDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

			// The difference in the call to BeginMenuBar() here comapred to the else statement's BeginMainMenuBar() is
			// that the dockspace occludes the MainMenuBar since the dockspace is itself a window
			if(ImGui::BeginMenuBar()){
				MenuBar(showVehicleManager, showSettings, showDemoWindow, showDebugLog);
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	} 
	// -----------------------------------------------------------
	// No Docking else statement
	else {
		if(ImGui::BeginMainMenuBar()){
			MenuBar(showVehicleManager, showSettings, showDemoWindow, showDebugLog);
			ImGui::EndMainMenuBar();
		}
	}
	
	// Show modules
	if(showVehicleManager)		{ VehicleManager(app, showVehicleManager); }
	if(showSettings)			{ Settings(app, showSettings); }
	if(showDebugLog)			{ DebugLog(showDebugLog); }

	return;
}

void MenuBar(bool& showVehMan, bool& showSettings, bool &demoWindow, bool &debugLog){
	// Creates Dropdown item in the menu bar labeled "Modules"
	if(ImGui::BeginMenu("Modules")){
		ImGui::MenuItem("Vehicle Manager", nullptr, &showVehMan);
		ImGui::MenuItem("Debug Log", nullptr, &debugLog);
		ImGui::MenuItem("Settings", nullptr, &showSettings);
		ImGui::EndMenu();
	}

	// Creates Dropdown item in the menu bar labeled "View"
	if(ImGui::BeginMenu("View")){
		ImGui::MenuItem("Demo Window", nullptr, &demoWindow);
		ImGui::EndMenu();
	}
}


