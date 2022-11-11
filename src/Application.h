#pragma once
#include "FileSystem.h"
#include "Log.h"
#include "Modules/Module.h"

#include <vector>
#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_impl_glfw.h>
#include <DearImGUI/imgui_impl_opengl3.h>

// Forward Declarations
class Application;									// For these Functions
void Home					(Application* app);		// Needed in the Run() main lopp. 
void SetupVehicleManager	(Application* app);		// Needed for initialization of the Application class
class Module;										// For the Module list

class Application
{
public:
	Application(){};
	~Application(){};
	
	void Startup();		// Goes through all Setup member functions
	void Run();			// Contians main loop


	// Get functions
	std::string getMainDirectory()		const	{ return DIRECTORY_PATH.string(); 			};
	std::string getDebugDirectory()		const	{ return DEBUG_PATH.string(); 				};
	std::string getVehicleDirectory()	const	{ return VEHICLE_PATH.string(); 			};
	std::string getLogFilePath()		const	{ return m_currentInstanceLogFile.string(); };
	std::string getWindowTitle()		const	{ return WINDOW_TITLE; 						};

	// Window variables
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
	
	// This is written to in SetupFileSystem() to make the file's directory's
	std::filesystem::path 	DIRECTORY_PATH;
	std::filesystem::path	DEBUG_PATH;
	std::filesystem::path	VEHICLE_PATH;

	// Setup functions. Does setup in the order listed here
	bool SetupFileSystem();
	bool SetupGLFW();
	bool SetupImGUI();
	bool SetupModules();
	
	std::vector<Module> m_moduleList;
	
	std::ostringstream LogFileName();	//Displays the date for Log file. Ex: SUN, SEP 25, 2022

};

