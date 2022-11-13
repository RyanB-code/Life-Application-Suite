#include "Module.h"


Module::Module(const std::string setName, Application* app)  : m_name{setName}, m_app{app} {
    m_shown = false;
}  

Module::~Module(){
    
}

void Module::Display(){
    if(ImGui::Begin("Default Name", &m_shown, 0)){
        ImGui::Text("There is no override funciton specified for %s", m_name.c_str());
    }
    ImGui::End(); //End Window
}

bool Module::Setup(){
    Log(LogCode::WARNING, "A Module's Setup function called, no override function was found");

    return false;
}

// NEED A WAY FOR APPLICATION TO KEEP TRACK OF MODULES