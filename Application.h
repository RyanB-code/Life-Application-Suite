#pragma once
#include "FileSystem.h"
#include "Display.h"
#include "Vehicle.h"
#include "Log.h"

class Application
{
public:
	Application();
	~Application() {

	}
	void run();

	//Add Vehicle to the list of known vehicles
	inline void NewVehicle(Vehicle& vehicle);
	
	//MAKE PRIVATE AFTER DISPLAY SYSTEM IS MADE
	//Saves vehicles by writing to file. The path is specified in the Application class
	bool const saveVehicles();	//MAKE PRIVATE AFTER DISPLAY SYSTEM IS MADE

	//Make a Vehicle type from the text. Deletes read characters
	const Vehicle makeVehicle(std::string& text);
	//Make a Repair type from the text. Deletes read characters. \param repairList: Adds found repairs to the vector
	const void makeRepair(std::string& text, std::vector<Repair>& repairList);
	// Make a GasStop type from the text. Deletes read characters. \param gasList : Adds found gas stops to the vector
	const void makeGasStop(std::string& text, std::vector<GasStop>& gasList);

	std::ostringstream const ListVehicles();

	friend bool const FileSystem::writeToFile(const std::string setPath, Vehicle& vehicle);

private:
	bool m_initialized{ false };
	const std::filesystem::path DIRECTORY_PATH		{ "C:/Users/ryanb/Desktop/LAS Folder/" };
	const std::filesystem::path	DEBUG_PATH			{ DIRECTORY_PATH.string() + "Debug/"};
	const std::filesystem::path	VEHICLE_FOLDER		{ DIRECTORY_PATH.string() + "Vehicles/"};

	std::filesystem::path m_currentInstanceLogFile{};
	
	//Creates the Log file name using date and time.
	std::ostringstream LogFileName();

	//Once App is initialized, start reading files.
	bool Startup();

	void readUntil(std::string& text, const char limit, auto& returnType);
	std::string readUntil(std::string& text, const char limit);
	
	std::vector<Vehicle> m_vehicleList;
};

