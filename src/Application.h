#pragma once
#include "FileSystem.h"
#include "Log.h"

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_impl_glfw.h>
#include <DearImGUI/imgui_impl_opengl3.h>

// Forward Declarations
class Application;
class Vehicle;
void Home					(Application* app);		// Needed in the Run() main lopp. 
void SetupVehicleManager	(Application* app);		// Needed for initialization of the Application class

class Application
{
public:
	Application(){};
	~Application(){};
	
	void Startup();		// Initializes directories, GLFW, ImGui, and Modules
	void Run();			// Contians main loop


	std::string getMainDirectory()		const	{ return DIRECTORY_PATH.string(); 			};
	std::string getDebugDirectory()		const	{ return DEBUG_PATH.string(); 				};
	std::string getVehicleDirectory()	const	{ return VEHICLE_PATH.string(); 			};
	std::string getLogFilePath()		const	{ return m_currentInstanceLogFile.string(); };
	std::string getWindowTitle()		const	{ return WINDOW_TITLE; 						};


	const ImVec2 	MIN_WIN_SIZE	{1280, 720};
	GLFWwindow* 	m_window 		{nullptr};
	int 			m_window_x		{1280};
	int 			m_window_y		{720};
	ImGuiStyle* 	appStyle		{nullptr};


private:
	Application* m_app{ nullptr };

	bool m_initialized	{ false };
	bool m_vsync 		{ true };

	std::filesystem::path 	m_currentInstanceLogFile	{ };

	const 	std::string 			WINDOW_TITLE 		{"Life Application Suite"};
	const 	std::filesystem::path 	DIRECTORY_PATH		{ "C:/Users/ryanb/Desktop/LAS Folder/" };
	const 	std::filesystem::path	DEBUG_PATH			{ DIRECTORY_PATH.string() + "Debug/"};
	const 	std::filesystem::path	VEHICLE_PATH		{ DIRECTORY_PATH.string() + "Vehicles/"};

	bool SetupFileSystem();
	bool SetupGLFW();
	bool SetupImGUI();
	
	std::ostringstream LogFileName();	//Displays the date for Log file. Ex: SUN, SEP 25, 2022

};

