#pragma once
#include "FileSystem.h"
#include "Display.h"
#include "Vehicle.h"


enum class Error {
	FUNCTION_FAILED,
	INITIALIZATION_FAILED,
};

class Application
{
public:
	Application() {
		if (!FileSystem::init(FILE_PATH.string())) {
			throw Error::FUNCTION_FAILED;
		}
		else {
			m_initialized = true;
		}
	}
	~Application() {

	}
	void run();

	//Add Vehicle to the list of known vehicles
	void NewVehicle(Vehicle& vehicle) { m_vehicleList.push_back(vehicle); }						//MAKE PRIVATE AFTER DISPLAY SYSTEM IS MADE
	//Saves vehicles by writing to file. The path is specified in the Application class
	bool const saveVehicles();																	//MAKE PRIVATE AFTER DISPLAY SYSTEM IS MADE

	friend bool const FileSystem::writeToFile(const std::string setPath, Vehicle& vehicle);

private:
	bool m_initialized{ false };
	const std::filesystem::path FILE_PATH{ "C:/Users/ryanb/Desktop/LAS Folder/" };
	const std::string			VEHICLE_FILE_NAME{ "Vehicles.dat" };

	std::vector<Vehicle> m_vehicleList;
};

