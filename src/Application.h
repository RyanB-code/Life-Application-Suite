#ifndef APPLICATION_H
#define APPLICATION_H

// User-created include
#include "FileSystem.h"
#include "Modules/Module.h"
#include "Modules/DebugLog.h"
#include "Modules/Settings.h"
#include "Modules/Vehicle.h"

// Standard library include
#include <vector>
#include <Windows.h>

// 3rd Party Library include
#include <RST/RST.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_impl_glfw.h>
#include <DearImGUI/imgui_impl_opengl3.h>


class Application
{
public:
	Application(){RST::SetLogLevel(LogLevel::FATAL);};
	~Application(){};
	
	void Startup();		// Goes through all Setup member functions
	void Run();			// Contians main loop


	// Get functions
	std::string getMainDirectory()		const	{ return DIRECTORY_PATH.string(); 	}
	std::string getVehicleDirectory()	const	{ return VEHICLE_PATH.string(); 	}
	std::string getWindowTitle()		const	{ return WINDOW_TITLE; 				}		

	// Window variables
	const ImVec2 	MIN_WIN_SIZE	{1280, 720};
	GLFWwindow* 	m_window 		{nullptr};
	int 			m_window_x		{1280};
	int 			m_window_y		{720};
	ImGuiStyle* 	appStyle		{nullptr};


private:

	const std::string VERSION = "v0.2.0-WIP";

	bool m_initialized	{ false };
	bool m_vsync 		{ true };

	// For RST
	LogTarget 	m_logTarget		{LogTarget::ALL};
	LogLevel 	m_logLevel		{LogLevel::ALL};

	const 	std::string 			WINDOW_TITLE 		{"Life Application Suite"};
	
	// Paths necessary to run. The interaction and variables
	std::string getExeParentPath() const;						// Returns the parent directory of the EXE path
	void 		AssignPaths(const std::string parentPath);		// Assigns member variable paths given the parentPath
	std::filesystem::path 	DIRECTORY_PATH;						// Main parent directory that houses the executable
	std::filesystem::path	VEHICLE_PATH;						// Directory holdng all of the Vehicle information files


	// Setup functions. Does setup in the order listed here
	bool SetupFileSystem();										// Set member path varialbes and initializes RST
	bool SetupGLFW();											
	bool SetupImGUI();											// Creates window flags and sets colors
	bool SetupModules();										// Iterates through s_moduleList and runs the Setup() function

	bool FirstTimeSetup(); 										// Called if SetupFileSystem() could not find directories necessary, thereby assuming FirstTimeSetup
	
	
	static std::vector<Module*> s_moduleList;							// Holds all Modules
	void AddModule(Module* module) { s_moduleList.push_back(module); }	// Adds given Module parameter to list of known modules

	// Friend functions
	friend void Home(Application* app);
	friend void MenuBar(bool &demoWindow);

};




#endif