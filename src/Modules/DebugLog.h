#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include "Module.h"

#include <sstream>
#include <iomanip>



// Interact with and view logs created by the application
// AS OF 6 NOV 2022, DOES NOT WORK


class Debug : public Module {
public:
    Debug(Application* app);
    ~Debug() override;

    void    Display() override;
    bool    Setup()   override;

   std::vector<std::string>* m_logs;


};


#endif