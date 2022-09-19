#pragma once
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Vehicle.h"
namespace FileSystem {
	
	

	bool init(const std::string path);

	bool	createFile		(const std::string path);
	bool	doesFileExist	(const std::string path);

	//Through operator overloading, the goal is to call the read and write functions depending on what format you intend to see

	bool const writeToFile		(const std::string setPath, Vehicle& vehicle);
	bool const readFromFile		(const std::string setPath, Vehicle& vehicle);

	// \return The text between the specified characters
	std::string readBetween(std::string& text, char beginningCharacter, char endingCharacter);

	GasStop& MakeGasStopFromText(std::string text);
}