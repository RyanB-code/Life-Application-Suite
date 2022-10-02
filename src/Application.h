#pragma once
#include "FileSystem.h"
#include "Vehicle.h"
#include "Log.h"


class Application
{
public:
	Application();
	~Application() {

	}
	void run();

	//Add vehicle to the list of known vehicles
	inline void NewVehicle(Vehicle& veh){
		m_vehicleList.push_back(veh);
		Log(LogCode::ROUTINE, "Added " + veh.getName() + " to list of vehicles.");
	}
	
	//Saves vehicles by writing to file. The path is specified in the Application class \return True if could save to file. False if could not.
	bool const saveVehicles();

	// Reads \param Text string from file. Deletes chars read \return Name
	const std::string makeVehicle(std::string& text);
	//Make a Repair from a text stream. \param repairList: Adds found repairs to this vector
	const void makeRepair(std::string& text, Vehicle& veh);
	// Make a GasStop from a text stream. \param gasList : Adds found gas stops to this vector
	const void makeGasStop(std::string& text, Vehicle& veh);

	friend bool const FileSystem::writeToFile(const std::string setPath, Vehicle& vehicle);

	std::vector<Vehicle>& getVehicleList() { return m_vehicleList; };

	//Show protected file paths =============
	std::string showMainDirectory()		{ return DIRECTORY_PATH.string(); };
	std::string showDebugDirectory()	{ return DEBUG_PATH.string(); };
	std::string showVehicleDirectory()	{ return VEHICLE_PATH.string(); };
	std::string showLogFilePath()		{ return m_currentInstanceLogFile.string(); };
	//=======================================
private:
	Application* app{};

	bool m_initialized{ false };
	const std::filesystem::path DIRECTORY_PATH		{ "C:/Users/ryanb/Desktop/LAS Folder/" };
	const std::filesystem::path	DEBUG_PATH			{ DIRECTORY_PATH.string() + "Debug/"};
	const std::filesystem::path	VEHICLE_PATH		{ DIRECTORY_PATH.string() + "Vehicles/"};
	std::filesystem::path 		m_currentInstanceLogFile{ };
	
	//Displays the date for Log file. Ex: SUN, SEP 25, 2022
	std::ostringstream LogFileName();

	//Once the application is initialized, start reading files.
	void Startup();


	//===Used for creating data from module files===============

	//Reads text until limit and deletes characters read \param text: what will be read, limit: char to read until, returnType: will be overwritten with the data
	void readUntil(std::string& text, const char limit, auto& returnType);
	//Reads text until limit, and deletes characters read \param text: what will be read limit: char to read until \return String of read text
	std::string readUntil(std::string& text, const char limit);
	//==========================================================

	std::vector<Vehicle> m_vehicleList;
};

