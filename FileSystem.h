#pragma once
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Vehicle.h"
namespace FileSystem {
	
	

	bool init(const std::string path);

	bool	createFile		(const std::string path);
	bool	doesFileExist	(const std::string path);

	bool const writeToFile		(const std::string setPath, Vehicle& vehicle);
}