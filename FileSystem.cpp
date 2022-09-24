#include "FileSystem.h"


namespace FileSystem {
	bool createDirectory(const std::string path) {
		bool success{ false };


		if (doesFileExist(path)) {
			Log(LogCode::ROUTINE, path + " was found.");
			success = true;
		}
		else {
			std::filesystem::create_directory(path);
			Log(LogCode::ROUTINE, path + " was created.");

			success = true;
		}
		return success;
	}
	
	bool createFile(const std::string path) {
	

		//if file doesn't exist, create it
		if (!doesFileExist(path)) {
			std::ofstream file(path);	//creates the files here

			if (!doesFileExist(path)) {//if the file still does not exist, return false		
				Log(LogCode::WARNING, "Could not create " + path);
				return false;
			}
			else {
				Log(LogCode::ROUTINE, "Created file " + path);
				return true;
			}
		}
		else {
			Log(LogCode::ROUTINE, path + " was already found.");
			return true;
		}

		
	}
	bool doesFileExist(const std::string path) {
		const std::filesystem::path checkFile(path);

		if (std::filesystem::exists(checkFile) ){
			return true;
		}
		else
		{
			return false;
		}

	}


	bool const writeToFile (const std::string path, Vehicle& vehicle) {

		//If the file doesnt exist, create the path, and then it writes to it again. If that writeToFile fails again, return with unsuccessful
		if (!doesFileExist(path)) {
			createFile(path);
			return writeToFile(path, vehicle);
		}
		else {
			std::ofstream file{ path };

			file << '(' << vehicle.getName() << ")\n";
			for (Repair& repair : vehicle.getRepairList()) {
				file << '<';
				file << repair;
				file << '>';
				file << '\n';
			}
			for (GasStop& gasStop : vehicle.getGasStopList()) {
				file << '[';
				file << gasStop;
				file << ']';
				file << '\n';
			}

			return true;
		}

		return false;

	}
	

	bool const readFromFile(const std::string setPath, Vehicle& vehicle) {
		std::ostringstream path{ setPath };

		if (!doesFileExist(path.str())) {
			return false;
		}
		else {
			std::ifstream file{ path.str(), std::ios_base::in};
			if(!file) {
				Log(LogCode::WARNING, "Failed to open " + path.str() + " for reading.");
				return false;
			}
			else {
				Log(LogCode::LOG, "Successfully opened " + path.str() + " for reading.");
			
				return true;
			}
		}
	}
}