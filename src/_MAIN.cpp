#include "Display.h"
#include "Application.h"


int main(int, char**) {

    Application* app = new Application{};

    try{
        app->Startup();
        app->Run();
    }
    catch(Log){
        Log(LogCode::FATAL, "Fatal error in application was caught in main");
    }
}