#pragma once
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Vehicle.h"
#include "Log.h"
namespace FileSystem {
	
	

	bool createDirectory(const std::string path);

	bool	createFile		(const std::string path);
	bool	doesFileExist	(const std::string path);

	//Through operator overloading, the goal is to call the read and write functions depending on what format you intend to see\
	// \param Path of file, Object that will be written to the file
	bool const writeToFile		(const std::string setPath, Vehicle& vehicle);

	// /param Path of file, Where output of the file will be passed to
	bool const readFile(const std::string setPath, std::ostringstream& output);

	std::ostringstream filesInDirectory(const std::string directoryToRead);


	GasStop& MakeGasStopFromText(std::string text);
}