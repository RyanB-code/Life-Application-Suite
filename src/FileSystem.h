#pragma once
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Vehicle.h"
#include "Log.h"
namespace FileSystem {
	// \return True if directory was found/created. False if directory could not be created
	bool	createDirectory	(const std::string path);
	// \return True if file was found/created. False if file could not be created
	bool	createFile		(const std::string path);

	bool	doesFileExist	(const std::string path);

	bool	deleteFile		(const std::string path);

	//Through operator overloading, the goal is to call the read and write functions depending on file format
	// \param setPath: path of file to write, vehicle: what will be written to the file
	bool const writeToFile	(const std::string setPath, Vehicle& vehicle);
	// \param Path of file, Where output of the file will be passed to \return Overrides output stream with text of the file. True if could read, false if could not
	bool const readFile(const std::string setPath, std::ostringstream& output);


	// Iterates through the directory \return Overrides writeTo with file names found
	void filesInDirectory(const std::string directoryToRead, std::vector<std::string>& writeTo);
}