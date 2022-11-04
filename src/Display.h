#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <Windows.h>

#include "Application.h"

namespace Display
{
	static void HelpMarker(const char* desc);

	//===========Menus/Screens======================
	
	// Creates Home screen
	void Home(Application* app);
	void MenuBar(bool& showVehMan, bool& showSettings, bool &demoWindow, bool &debugLog);

	void VehicleManager(Application* app, bool &shown);
	void Settings(Application* app, bool &shown);
	void DebugLog(bool &shown);

	//Shows just one vehicle
	void ShowFullVehicleInformation(Vehicle* veh);
	void EditVehicle(Vehicle* veh);
	Vehicle* ListSelectableVehicles(std::vector<Vehicle>& vehList);

	bool AddRepair(Vehicle* veh);
	bool AddGasStop(Vehicle* veh);
	bool AddVehicle(Application* app);

}

