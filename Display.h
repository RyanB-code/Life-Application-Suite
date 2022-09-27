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

	//===========Menus/Screens======================

	void Home(Application* app);
	void VehicleHome(Application* app);
	void Settings(Application* app);

	//Shows all vehicles in the vector
	void ShowFullVehicleInformation(std::vector<Vehicle>& vehList);
	//Shows just one vehicle
	std::ostringstream ShowFullVehicleInformation(Vehicle* veh);
	void EditVehicle(Vehicle* veh);
	//Vehicle sub-menu. Choose a vehicle from vehList \return Pointer to vehicle chosen
	Vehicle* SelectVehicle(std::vector<Vehicle>& vehList);
	//=============================================

	//List vehicles. \param detailed: if set to true, displays full data. If false, just displays the name and miles \return Stream of formatted list
	std::ostringstream const ListVehicles(std::vector<Vehicle>& vehList, bool detailed=false);

	//Displays in format DDD MMM XX, YYYY -> XX meaning day number ie. 12th or 1st of the month
	std::ostringstream readableDayMonthYear();

	// Displays the date and title text \param The banner text
	void DisplayBanner(const std::string title);
}

