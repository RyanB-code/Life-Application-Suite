#include <iostream>

#include "Application.h"


int main() {
	/*
	Vehicle testVehicle1{ "Jeep Liberty", 1 };
	Repair testRepair1{ 10, "New Repair", 20, "NSTR", false };
	Repair testRepair2{ 5, "PSK", 20, "NSTR", false };
	Repair testRepair3{ 29, "REPAIR", 20, "NSTR", false };
	Repair testRepair4{ 29, "REPAIR 2", 20, "NSTR", false };
	

	GasStop testGasStop1{ 76, 20, 4.99, "Notes..." };
	GasStop testGasStop2{ 76, 20, 4.99, "Notes 2" };


	testVehicle1.NewRepair(testRepair1);
	testVehicle1.NewRepair(testRepair2);


	Vehicle testVehicle2{ "New Car", 1 };

	testVehicle2.NewRepair(testRepair1);
	testVehicle2.NewRepair(testRepair2);
	testVehicle2.NewRepair(testRepair3);
	testVehicle2.NewRepair(testRepair4);
	testVehicle2.NewGasStop(testGasStop1);
	testVehicle2.NewGasStop(testGasStop2);
	*/

	

	//start release code

	Application* app = new Application();

//	app->NewVehicle(testVehicle1);
//	app->NewVehicle(testVehicle2);

	try {
		app->run();

	}
	catch (Log& log) {
		std::cout << "log caught\n";
	}
	
	return 0;
}