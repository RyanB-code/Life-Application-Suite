#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include "Module.h"

// Interact with and view logs created by the application
// AS OF 6 NOV 2022, DOES NOT WORK


class Debug : public Module {
public:
    Debug(Application* app);
    ~Debug() override;

    void    Display() override;
    bool    Setup()   override;
};


#endif