#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include <RST/RST.h>


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
	

	std::string readUntilString(std::string& text, const char limit);					// Reads text until limit char found and deletes characters read	
	bool	createDirectory	(const std::string path);									// Return TRUE if directory was found/created. FALSE if directory could not be created
	bool	createFile		(const std::string path);									// Return TRUE if file was found/created. FALSE if file could not be created
	bool	doesFileExist	(const std::string path);									// Return TRUE if file exists. FALSE if it does not exist
	bool	deleteFile		(const std::string path);									// Return TRUE if file could be deleted, FALSE is file could not be deleted
	bool 	renameFile		(const std::string oldPath, const std::string newPath);		// Return TRUE if target file could be found and renamed, FALSE if target file could not be found

	bool writeToFile		(const std::string path, const std::string& text);			// Writes the given text to the file. Overwrites everything in the file. Returns TRUE if file was written, FALSE if file could not be found
	bool readFile			(const std::string path, std::ostringstream& output);		// Reads all text from a file and overwrites parameter output with the text. Returns TRUE if file was opened and read from, FALSE if file could not be found

	void filesInDirectory(const std::string directoryToRead, std::vector<std::string>& writeTo);	// Iterates through the given directory and adds the file name to the writeTo vector

}

#endif