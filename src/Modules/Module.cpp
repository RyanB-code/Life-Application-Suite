#include "Module.h"

Module::Module(const std::string setName)  : m_name{setName} {
    m_shown = false;
}

Module::~Module(){
    
}


// NEED A WAY FOR APPLICATION TO KEEP TRACK OF MODULES