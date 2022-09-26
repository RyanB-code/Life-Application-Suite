#include <iostream>

#include "Application.h"
#include "Display.h"

void AddTestCases(Application& app);

int main() {

	Application* app = new Application();

	//AddTestCases(*app);

	try {
		app->run();
		Display::Home(app);
	}
	catch (Log& log) {
		Log(LogCode::FATAL, "Log error throw was caught in main.");
	}
	
	return 0;
}

void AddTestCases(Application& app) {

	Repair testRepair1{ 10, "New Repair", 20.10, "NSTR", false };
	Repair testRepair2{ 5, "PSK", 29, "NSTR", false };
	Repair testRepair3{ 29, "REPAIR", 40.99, "NSTR", false };
	Repair testRepair4{ 87, "REPAIR 2", 20, "NSTR", false };


	GasStop testGasStop1{ 76, 20, 4.99, "Notes..." };
	GasStop testGasStop2{ 80, 20, 4.99, "5" };


	Vehicle testVehicle2{ "New Car", 1 };

	testVehicle2.NewRepair(testRepair1);
	testVehicle2.NewRepair(testRepair2);
	testVehicle2.NewRepair(testRepair3);
	testVehicle2.NewRepair(testRepair4);
	testVehicle2.NewGasStop(testGasStop1);
	testVehicle2.NewGasStop(testGasStop2);

	app.NewVehicle(testVehicle2);
}