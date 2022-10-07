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
	Application();
	~Application() {

	}
	
	// Once the application is initialized, start reading files.
	void Startup();

	// Add vehicle to the list of known vehicles
	inline void NewVehicle(Vehicle& veh){
		m_vehicleList.push_back(veh);
		Log(LogCode::ROUTINE, "Added " + veh.getName() + " to list of vehicles.");
	}
	
	// Saves vehicles by writing to file. The path is specified in the Application class \return True if could save to file. False if could not.
	bool const saveVehicles();

	std::vector<Vehicle>& getVehicleList() { return m_vehicleList; };

	// Show prviate variables =============
	const std::string showMainDirectory()		{ return DIRECTORY_PATH.string(); };
	const std::string showDebugDirectory()		{ return DEBUG_PATH.string(); };
	const std::string showVehicleDirectory()	{ return VEHICLE_PATH.string(); };
	const std::string showLogFilePath()			{ return m_currentInstanceLogFile.string(); };

	const std::string& getWindowTitle()			{ return WINDOW_TITLE; };
	// =======================================

	GLFWwindow* m_window {nullptr};
	ImGuiIO m_io{};
	int 	m_window_x		{1280};
	int 	m_window_y		{720};
private:
	Application* app{ nullptr };

	bool m_initialized{ false };
	const std::filesystem::path DIRECTORY_PATH		{ "C:/Users/ryanb/Desktop/LAS Folder/" };
	const std::filesystem::path	DEBUG_PATH			{ DIRECTORY_PATH.string() + "Debug/"};
	const std::filesystem::path	VEHICLE_PATH		{ DIRECTORY_PATH.string() + "Vehicles/"};


	const std::string WINDOW_TITLE 		{"Life Application Suite"};
	bool m_vsync {true};

	std::filesystem::path 		m_currentInstanceLogFile{ };
	
	//Displays the date for Log file. Ex: SUN, SEP 25, 2022
	std::ostringstream LogFileName();

	// Reads name\param Text string from file, deletes read characters
	const std::string makeVehicleName(std::string& text);
	//Reads mileage \param Text string from file, deletes read characters
	const uint32_t makeVehicleMiles(std::string& text);
	//Make a Repair from a text stream. \param repairList: Adds found repairs to this vehicle
	const void makeRepair(std::string& text, Vehicle& veh);
	// Make a GasStop from a text stream. \param gasList : Adds found gas stops to this vehicle
	const void makeGasStop(std::string& text, Vehicle& veh);

	//===Used for creating data from module files===============

	//Reads text until limit and deletes characters read \param text: what will be read, limit: char to read until, returnType: will be overwritten with the data
	void readUntil(std::string& text, const char limit, auto& returnType);
	//Reads text until limit, and deletes characters read \param text: what will be read limit: char to read until \return String of read text
	std::string readUntil(std::string& text, const char limit);
	//==========================================================

	std::vector<Vehicle> m_vehicleList;
};

