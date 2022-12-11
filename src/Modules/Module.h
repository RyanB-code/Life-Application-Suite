#ifndef MODULE_H
#define MODULE_H


#include <string>
#include <vector>

#include <RST/RST.h>
#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_mods.h>

// Forward declaration
class Application;

class Module{
public:
    Module(const std::string setName="No Name", Application* app=nullptr);
    virtual ~Module();

    const std::string getName()   const   { return m_name;    }
    const bool        getShown()  const   { return m_shown;   }

    virtual void Display(); // Handles ImGUI implementation of the Module
    virtual bool Setup();   // Handles initialization upon startup

    friend void MenuBar(bool &demoWindow);

protected:
    std::string m_name;     // Defines the name of the module to be used
    bool        m_shown;    // Whether or not this module is currently opened in ImGui

    Application* m_app;

private:

};


#endif