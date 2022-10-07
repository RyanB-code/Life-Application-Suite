#include "Display.h"
#include "Application.h"


int main(int, char**) {

    Application* app = new Application{};

    try{
        app->Startup();
        Display::Run(app);
    }
    catch(Log){
        Log(LogCode::FATAL, "Fatal error in application was caught in main");
    }
}