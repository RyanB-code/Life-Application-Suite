#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <Windows.h>

namespace Display
{
	//Clears the display
	void clear();

	//Displays home screen
	void Home();
	void VehicleHome();

	//Displays in format DDD MMM XX, YYYY -> XX meaning day number ie. 12th or 1st of the month
	std::ostringstream readableDayMonthYear();


	// Displays the date and title text \param The banner text
	void DisplayBanner(const std::string title);
}

