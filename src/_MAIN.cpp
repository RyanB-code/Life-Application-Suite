#include "Application.h"

#include <RST/RST.h>


int main(int, char**) {

    Application* app = new Application{};

    try{
        app->Startup();
        app->Run();
    }
    catch(bool log){
        RST::Log("Application error throw was caught in main.", LogCode::FATAL);
    }

}