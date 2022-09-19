#include "Application.h"

bool const Application::saveVehicles() {
	bool success{ false };
	std::ostringstream savePath{ DIRECTORY_PATH.string() + VEHICLE_FILE_NAME };
	for (Vehicle& currentVehicle : m_vehicleList) {
		if (!FileSystem::writeToFile(savePath.str(), currentVehicle)) {
			success = false;
		}
		else
			success = true;
	}

	return success;
}

void Application::run() {
	if (!m_initialized) {
		throw Error::INITIALIZATION_FAILED;
		return;
	}
	else {
		Display::clear();
		Display::Home();
	}

	return;
}