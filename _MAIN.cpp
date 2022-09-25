#include <iostream>

#include "Application.h"

void AddTestCases(Application& app);

int main() {

	//start release code
	Application* app = new Application();

	//AddTestCases(*app);

	try {
		app->run();

	}
	catch (Log& log) {
		Log(LogCode::WARNING, "Log error throw was caught.");
	}
	
	return 0;
}

void AddTestCases(Application& app) {

	Vehicle testVehicle1{ "Jeep Liberty", 1 };
	Repair testRepair1{ 10, "New Repair", 20.10, "NSTR", false };
	Repair testRepair2{ 5, "PSK", 29, "NSTR", false };
	Repair testRepair3{ 29, "REPAIR", 40.99, "NSTR", false };
	Repair testRepair4{ 87, "REPAIR 2", 20, "NSTR", false };


	GasStop testGasStop1{ 76, 20, 4.99, "Notes..." };
	GasStop testGasStop2{ 80, 20, 4.99, "5" };


	testVehicle1.NewRepair(testRepair1);
	testVehicle1.NewRepair(testRepair2);


	Vehicle testVehicle2{ "New Car", 1 };

	testVehicle2.NewRepair(testRepair1);
	testVehicle2.NewRepair(testRepair2);
	testVehicle2.NewRepair(testRepair3);
	testVehicle2.NewRepair(testRepair4);
	testVehicle2.NewGasStop(testGasStop1);
	testVehicle2.NewGasStop(testGasStop2);

	testVehicle1.NewGasStop(testGasStop1);

	app.NewVehicle(testVehicle1);
	app.NewVehicle(testVehicle2);
}