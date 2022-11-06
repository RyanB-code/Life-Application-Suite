#include "Home.h"


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


