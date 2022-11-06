#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Log.h"

namespace FileSystem {
	// Reads text until limit and deletes characters read \param text: what will be read, limit: char to read until, returnType: will be overwritten with the data
	template <typename T>
	void readUntilAuto(std::string& text, const char limit, T& returnType){
		std::stringstream bufferText;

		int charRead{0};
		for (char& c : text) {
			++charRead;
			if (c != limit) {
				bufferText << c;
			}
			else {
				bufferText >> returnType;
				text.erase(0, charRead);
				return;
			}
		}
	}
	// Reads text until limit, and deletes characters read \param text: what will be read limit: char to read until \return String of read text
	std::string readUntilString(std::string& text, const char limit);
	
	// \return True if directory was found/created. False if directory could not be created
	bool	createDirectory	(const std::string path);
	// \return True if file was found/created. False if file could not be created
	bool	createFile		(const std::string path);

	bool	doesFileExist	(const std::string path);

	bool	deleteFile		(const std::string path);

	// Through operator overloading, the goal is to call the read and write functions depending on file format
	// \param setPath: path of file to write, \param text - text to write to file
	bool writeToFile(const std::string path, const std::string& text);
	// \param Path of file, Where output of the file will be passed to \return Overrides output stream with text of the file. True if could read, false if could not
	bool readFile(const std::string setPath, std::ostringstream& output);

	// Iterates through the directory \return Overrides writeTo with file names found
	void filesInDirectory(const std::string directoryToRead, std::vector<std::string>& writeTo);

}

#endif