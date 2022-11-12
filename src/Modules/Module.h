#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>


class Module{
public:
    Module(const std::string setName);
    //virtual ~Module();
    virtual ~Module();

    std::string getName()   const   { return m_name;    }
    bool        getShown()  const   { return m_shown;   }


protected:
    std::string m_name;     // Defines the name of the module to be used
    bool        m_shown;    // Whether or not this module is currently opened in ImGui

};


#endif