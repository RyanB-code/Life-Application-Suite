#include <iostream>

#include "Application.h"
#include "Display.h"

int main() {

	Application* app = new Application();

	try {
		app->run();
		Display::Home(app);
	}
	catch (Log& log) {
		Log(LogCode::FATAL, "Log error throw was caught in main.");
	}
	
	return 0;
}
