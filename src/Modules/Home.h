#ifndef HOME_H
#define HOME_H

#include "DebugLog.h"
#include "Vehicle.h"
#include "Settings.h"

#include "../Application.h"

// Creates the homepage for the application
void Home(Application* app);

// Handles menu bar funcationality at the top of the window
void MenuBar(bool& showVehMan, bool& showSettings, bool &demoWindow, bool &debugLog);

#endif

