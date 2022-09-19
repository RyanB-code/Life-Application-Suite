#include "FileSystem.h"

#include <iostream>
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
				file << '[';
				file << repair;
				file << ']';
				file << '\n';
			}
			file << "}\n";
			//End Repair List

			//Gas Stop List
			file << "<GasStopList>\n";
			file << "{\n";
			for (GasStop& gasStop : vehicle.getGasStopList()) {
				file << '[';
				file << gasStop;
				file << ']';
				file << '\n';
			}
			file << "}\n";
			//End Gas Stop List

			file << "}\n\n";	//End of Vehicle
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
				return false;
			}
			else {
				char fileChar;

				std::string fileText;
				
				while(file) {			
					file >> fileChar;					
					fileText = fileText + fileChar;
				}
				
				Vehicle newVehicle{ readBetween(fileText, '(', ')' ), 0 };


				if (readBetween(fileText, '<', '>') == "RepairList") {
					//IN REPAIR LIST
				}

				if (readBetween(fileText, '<', '>') == "GasStopList") {
					newVehicle.NewGasStop( MakeGasStopFromText(readBetween(fileText, '[', ']')) );
				}

			
				return true;
			}
		}
	}


	std::string readBetween(std::string& text, char beginningCharacter, char endingCharacter) {
		std::ostringstream textBetween;
		
		bool endCharFound{ false };

		bool write{ false };
		for (char& c : text) {
			if (c == endingCharacter) {
				c = ' ';
				endCharFound = true;
				write = false;
			}

			if (write) {
				textBetween << c;
				c = ' ';
			}

			if (c == beginningCharacter) {
				write = true;
				c = ' ';
			}


			if (endCharFound) {
				return textBetween.str();
			}
		}

		return textBetween.str();
	}

	
	GasStop& MakeGasStopFromText(std::string text) {
		std::stringstream stream{ text };

		std::cout << stream.str() << "\n";

		uint32_t setMiles;
		uint8_t setGallons;
		double setPricePerGallon;
		std::string setNotes;

		for (char& c : stream.str()) {
			if (c == '|') {
				c = ' ';
			}
		}
		stream >> setMiles;
		stream >> setGallons;
		stream >> setPricePerGallon;

		std::ostringstream notes;
		for (std::string word; stream >> word;) {
			notes << word;
		}


		std::cout << setMiles << "\n";
		std::cout << setGallons << "\n";
		std::cout << setPricePerGallon << "\n";
		std::cout << notes.str() << "\n";




		GasStop buffer{ setMiles, setGallons, setPricePerGallon, notes.str() };
		return buffer;
	}
}