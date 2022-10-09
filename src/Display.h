#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <Windows.h>

#include "Application.h"
namespace Display
{
	//Clears the display
	void clear();
	void clearLineAfterInput();

	int getInput(short low, short high);
	bool getBoolInput();

	//===========Menus/Screens======================

	void Run(Application* app);
	void Home(Application* app);

	void MenuBar(bool& showVehMan, bool& showSettings, bool& dockingEnabled, bool &demoWindow, bool &debugLog);

	void VehicleManager(Application* app, bool &shown);
	void Settings(Application* app, bool &shown);
	void DebugLog(bool &shown);

	//Shows just one vehicle
	std::ostringstream ShowFullVehicleInformation(Vehicle* veh);
	bool AddRepair(Vehicle* veh);
	bool AddGasStop(Vehicle* veh);
	bool AddVehicle(Application* app);
	
	Vehicle* ListSelectableVehicles(std::vector<Vehicle>& vehList);
}

