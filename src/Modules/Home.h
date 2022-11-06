#ifndef HOME_H
#define HOME_H

#include "DebugLog.h"
#include "Vehicle.h"
#include "Settings.h"

#include "../Application.h"


static void HelpMarker(const char* desc);

void Home(Application* app);
void MenuBar(bool& showVehMan, bool& showSettings, bool &demoWindow, bool &debugLog);

#endif

