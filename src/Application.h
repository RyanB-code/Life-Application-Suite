#pragma once
#include "FileSystem.h"
#include "Vehicle.h"
#include "Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_impl_glfw.h>
#include <DearImGUI/imgui_impl_opengl3.h>



class Application
{
public:
	Application(){};
	~Application(){};
	
	// Once the application is initialized, start reading files.
	void Startup();
	void Run();

	// Add vehicle to the list of known vehicles
	inline void NewVehicle(Vehicle& veh){
		m_vehicleList.push_back(veh);
		Log(LogCode::ROUTINE, "Added " + veh.getName() + " to list of vehicles.");
	}
	bool deleteVehicle(Vehicle& veh);

	// Saves vehicles by writing to file. The path is specified in the Application class \return True if could save to file. False if could not.
	bool saveVehicles();

	std::vector<Vehicle>& getVehicleList() { return m_vehicleList; };

	// Show prviate variables =============
	std::string showMainDirectory()		const	{ return DIRECTORY_PATH.string(); };
	std::string showDebugDirectory()	const	{ return DEBUG_PATH.string(); };
	std::string showVehicleDirectory()	const	{ return VEHICLE_PATH.string(); };
	std::string showLogFilePath()		const	{ return m_currentInstanceLogFile.string(); };

	std::string getWindowTitle()		const	{ return WINDOW_TITLE; };
	// =======================================

	GLFWwindow* 	m_window 		{nullptr};
	int 			m_window_x		{1280};
	int 			m_window_y		{720};
	const ImVec2 	MIN_WIN_SIZE	{1280, 720};
	ImGuiStyle* 	appStyle		{nullptr};


private:
	Application* m_app{ nullptr };

	bool m_initialized{ false };
	const std::filesystem::path DIRECTORY_PATH		{ "C:/Users/ryanb/Desktop/LAS Folder/" };
	const std::filesystem::path	DEBUG_PATH			{ DIRECTORY_PATH.string() + "Debug/"};
	const std::filesystem::path	VEHICLE_PATH		{ DIRECTORY_PATH.string() + "Vehicles/"};

	const std::string 	WINDOW_TITLE 		{"Life Application Suite"};
	bool m_vsync {true};


	std::filesystem::path 		m_currentInstanceLogFile{ };

	bool SetupFileSystem();
	bool SetupGLFW();
	bool SetupImGUI();
	void SetupVehicleManager();
	
	//Displays the date for Log file. Ex: SUN, SEP 25, 2022
	std::ostringstream LogFileName();
	// Makes date\param Text string from file
	Date makeDate(std::stringstream& text) const;
	// Reads name\param Text string from file, deletes read characters
	std::string makeVehicleName(std::string& text) const;
	//Reads mileage \param Text string from file, deletes read characters
	uint32_t makeVehicleMiles(std::string& text) const;
	//Make a Repair from a text stream. \param repairList: Adds found repairs to this vehicle
	void makeRepair(std::string& text, Vehicle& veh);
	// Make a GasStop from a text stream. \param gasList : Adds found gas stops to this vehicle
	void makeGasStop(std::string& text, Vehicle& veh);

	//===Used for creating data from module files===============

	//Reads text until limit and deletes characters read \param text: what will be read, limit: char to read until, returnType: will be overwritten with the data
	void readUntil(std::string& text, const char limit, auto& returnType);
	//Reads text until limit, and deletes characters read \param text: what will be read limit: char to read until \return String of read text
	std::string readUntil(std::string& text, const char limit);
	//==========================================================

	std::vector<Vehicle> m_vehicleList;
};

