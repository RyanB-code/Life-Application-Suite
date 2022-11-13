#ifndef SETTINGS_H
#define SETTINGS_H

#include "Module.h"
#include <DearImGUI/imgui.h>

// View, edit, and save settings for the entire application
// AS OF 6 NOV 2022, Can only view file paths used in the application


class Settings : public Module {
public:
    Settings(Application* app);
    ~Settings() override;

    void Display()  override;
    bool Setup()    override;
};

#endif