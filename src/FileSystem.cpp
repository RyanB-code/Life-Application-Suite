#include "FileSystem.h"


namespace FileSystem{
	std::string readUntilString(std::string& text, const char limit) {
		std::string bufferText;

		// Ignore delimiter, once a string is found return it
		int charRead{ 0 };
		for (char& c : text) {
			++charRead;
	
			if (c != limit && charRead < text.length()) { bufferText += c; }	//Make sure c is not the limit, and that it does not go over the end of the string.
			else {
				if (c != limit) { bufferText += c; }
				text.erase(0, charRead);					// Deletes read characters
				return bufferText;
			}
		}

		return bufferText;
	}

	bool createDirectory(const std::string path) {
		bool success{ false };

		// Before creating, check if it already exists. If found, create
		if (doesFileExist(path)) {
			Log(LogCode::ROUTINE, path + " was found.");
			success = true;
		}
		else {
			// IF the directory could not be created, log and return false
			if (!std::filesystem::create_directory(path)) {
				Log(LogCode::WARNING, "Directory " + path + " could not be created.");
				success = false;
			}
			else{
				Log(LogCode::ROUTINE, path + " was created.");
				success = true;
			}
		}

		return success;
	}
	
	bool createFile(const std::string path) {
		
		// Error check if the file exists already
		if (!doesFileExist(path)) {
			std::ofstream file(path);	// Creates the files here

			// If the file still does not exist, return false		
			if (!doesFileExist(path)) {
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

	bool deleteFile(const std::string path){
		std::filesystem::path file{path};
		if(std::filesystem::remove(file)){
			Log(LogCode::LOG, "Deleted file " + file.string());
			return true;
		}
		else{
			Log(LogCode::WARNING, "Could not delete file " + file.string());
			return false;
		}
	}
	
	bool writeToFile(const std::string path, const std::string& text){
		//If the file doesnt exist, try to create and it and then write again. If that fails, returns false
		if (!doesFileExist(path)) {
			createFile(path);
			return writeToFile(path, text);
		}
		else {
			std::ofstream file{ path }; //Open file to write
			file << text;
			return true;
		}
		return false;
	}

	bool readFile(const std::string setPath, std::ostringstream& output) {
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
				std::string line;
				while (file.good()) {
					std::getline(file, line);
					output << line;
				}
				return true;
			}
		}
	}

	void filesInDirectory(const std::string directoryToRead, std::vector<std::string>& writeTo) {
		std::ostringstream unbufferedText;
		std::string fileNameBuffer;

		for (std::filesystem::path dirEntry : std::filesystem::directory_iterator{ directoryToRead }) {
			unbufferedText << dirEntry.string() << "\n";
		}
		
		//Iterate through the string stream of files found in the directory
		bool writeChar{ true };
		for (const char& c : unbufferedText.str()) {

			//Iterate through the string and write each file name to the vehicleFile vector
			if (c == '\n') {								//once newline is reached, save the current buffer to string
				writeTo.push_back(fileNameBuffer); //save current string buffer to vector
				fileNameBuffer = "";					//reset buffer
				writeChar = false;						//do not copy the newling character
			}
			else {
				writeChar = true;						//allow writing of char
			}

			if (writeChar) {
				fileNameBuffer += c; //save the current character
			}
		}

		//Log how many files were found
		std::ostringstream logText;
		logText << "Found " << writeTo.size() << " file(s)";
		Log(LogCode::ROUTINE, logText.str());

		return;
	}

}