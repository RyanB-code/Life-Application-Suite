#include "Settings.h"


void Settings(Application* app, bool &shown){
    // Show file paths used by the application
    if( ImGui::Begin("Settings", &shown, 0)){
        ImGui::Text("Main path:  \t\t%s", app->getMainDirectory().c_str());
        ImGui::Text("Debug path: \t\t%s", app->getDebugDirectory().c_str());
        ImGui::Text("Vehicle path:   \t%s", app->getVehicleDirectory().c_str());
        ImGui::Text("Log File:   \t\t%s", app->getLogFilePath().c_str());
    }
    ImGui::End();
    
    return;
}