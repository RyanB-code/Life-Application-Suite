#include "FileSystem.h"

namespace FileSystem {
	bool init(const std::string path) {
		bool success{ false };


		if (doesFileExist(path)) {
			success = true;
		}
		else {
			std::filesystem::create_directory(path);
			success = true;
		}
		return success;
	}
	
	bool createFile(const std::string setPath) {
		std::ostringstream path{};
		path << setPath;

		//if file doesn't exist, create it
		if (!doesFileExist(path.str())) {
			std::ofstream newFile{path.str()};

			if (!doesFileExist(path.str()))
				return false;
		}
		else {
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


	bool const writeToFile (const std::string setPath, Vehicle& vehicle) {

		std::ostringstream path{ setPath };

		//If the file doesnt exist, create the path, and then it writes to it again. If that writeToFile fails again, return with unsuccessful
		if (!doesFileExist(path.str())) {
			createFile(path.str());
			return writeToFile(path.str(), vehicle);
		}
		else {
			std::ofstream file{ path.str(), std::ios_base::app};

			file << '(' << vehicle.getName() << ")\n";
			file << "{\n";

			//Repair List
			file << "<RepairList>\n";
			file << "{\n";
			
			for (Repair& repair : vehicle.getRepairList()) {
				file << repair;
			}
			file << "}\n";
			//End Repair List

			//Gas Stop List
			file << "<GasStopList>\n";
			file << "{\n";
			for (GasStop& gasStop : vehicle.getGasStopList()) {
				file << gasStop;
			}
			file << "}\n";
			//End Gas Stop List

			file << "}\n\n";	//End of Vehicle
			return true;
		}

		return false;

	}
	
}