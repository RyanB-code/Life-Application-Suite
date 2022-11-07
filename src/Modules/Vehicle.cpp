#include "Vehicle.h"

std::vector<Vehicle> Vehicle::s_vehicleList{}; // Initializer for s_vehicleList to be used



std::ostream& 	operator<<	(std::ostream& os, const Repair& repair) {
	os << 		repair.m_mileageDone 
	<< '|' << 	repair.m_type 
	<< '|' << 	repair.m_cost 
	<< '|' << 	repair.m_isThirdPartyRepair 
	<< '|' << 	repair.m_notes 
	<< '|' << 	repair.m_date;

	return os;
}
std::ostream& 	operator<<	(std::ostream& os, const GasStop& gasStop) {
	os << 		gasStop.m_mileageDone 
	<< '|' << 	gasStop.m_pricePerGallon 
	<< '|' << 	gasStop.m_gallons 
	<< '|' << 	gasStop.m_notes 
	<< '|' << 	gasStop.m_date;

	return os;
}
std::ostream& 	operator<<	(std::ostream& os, const RepairType& type){
	switch(type){
		case RepairType::BATTERY_REPLACEMENT:
			os << "Battery Replacement";
			break;
		case RepairType::BODYWORK:
			os << "Bodywork";
			break;
		case RepairType::LIGHTBULB_REPLACEMENT:
			os << "Lightbulb Replacement";
			break;
		case RepairType::MECHANICAL:
			os << "Mechanical";
			break;
		case RepairType::OIL_CHANGE:
			os << "Oil Change";
			break;
		case RepairType::OTHER:
			os << "Other";
			break;
		case RepairType::POWER_STEERING_FLUID_EXCHANGE:
			os << "Power Steering";
			break;
		case RepairType::TIRE_REPLACEMENT:
			os << "Tire Replacement";
			break;
		case RepairType::TIRE_ROTATION:
			os << "Tire Rotation";
			break;
		case RepairType::TRANSMISSION_FLUID_EXCHANGE:
			os << "Transmission Fluid";
			break;
		case RepairType::WIPER_REPLACEMENT:
			os << "Wiper Blade Replacement";
			break;
		default:
			os << "No Repair Type Found";
			break;
	}

	return os;
}
bool 			operator==	(const Vehicle& lhs, const Vehicle& rhs){
	if(lhs.getName() == rhs.getName()){
		return true;
	}
	else{
		return false;
	}
}



void Repair::getRepairInfo(int& mileage, std::string& typeStr, double& costDbl, std::string& notesVar, bool& wasThirdParty, std::string& dateVar) const {
	mileage 		= 	m_mileageDone;
	costDbl 		= 	m_cost;
	notesVar 		= 	m_notes;
	wasThirdParty 	= 	m_isThirdPartyRepair;
	dateVar 		= 	m_date.string();

	std::ostringstream typeVarBuf; 		// Makes a string stream bufferr to insert m_type
	typeVarBuf << m_type;				// (using he overloaded operator<< to write 
	typeStr = typeVarBuf.str();			// the type in readable form) and then overwrites the parameter with the string
	
	return;
}
void GasStop::getGasStopInfo(int& mileage, double& gal, double& ppg, std::string& notesVar, std::string& dateVar) const{
	mileage 	= m_mileageDone;
	gal 		= m_gallons;
	ppg 		= m_pricePerGallon;
	notesVar	= m_notes;
	dateVar 	= m_date.string();
	return;
}



Vehicle::Vehicle(const std::string setName, uint32_t setMileage) {
	
	// Make sure the name is not longer than maxVehicleNameSize
	if (setName.length() <= 0 || setName.length() > maxVehicleNameSize) {
		m_name = "Vehicle";

		// Log the error
		std::ostringstream logText;
		logText << "Vehicle name out of range. Max allowed is: " << maxVehicleNameSize << ". Current: " << setName.length();
		Log(LogCode::WARNING, logText.str());
	}
	else {
		m_name = setName;
	}

	// Make sure the mileage is not less than 0
	if (setMileage < 0) {
		Log(LogCode::WARNING, "Vehicle mileage out of range for " + m_name);
		m_mileage = 0;
	}
	else {
		m_mileage = setMileage;
	}

	return;
}
bool Vehicle::NewRepair(uint32_t setMiles, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, Date setDate) {

	// State variables used for error checking
	bool milesAccepted{ false }, costAccepted{ false }, notesAccepted{ false };


	if (setMiles > 0) 	{ milesAccepted = true; }
	if (setCost > 0) 	{ costAccepted 	= true; }

	// Ensure the notes length is not longer than maxNotesSize
	if (setNotes.length() <= 0 || setNotes.length() > maxNotesSize) {
		std::ostringstream logText;
		logText << "Repair notes out of range. Max size allowed is " << maxNotesSize << ". Current: " << setNotes.length();
		Log(LogCode::WARNING, logText.str());
		notesAccepted = false;
	}
	else { notesAccepted = true; }

	// If all conditions are valid, make the Repair and add it to the Vehicle's list of repairs
	if (milesAccepted && costAccepted && notesAccepted) {
		Repair repBuf{ setMiles, setType, setCost, setNotes, setThirdParty, setDate };
		repairList.push_back(repBuf);

		// Sorts the Vehicle's repair list.
		// Check if sorting is needed (not needed if repBuf is higher than the last repairList element)
		if (repairList.size() == 1 || repBuf.getMileage() > repairList[repairList.size() - 2].getMileage()) { } // Does nothing if no need to sort
		else { 
			// Sort the Vehicle's repair list by mileage
			for (int i{ 0 }; i < repairList.size() - 1; ++i) {
				for (int x{ i + 1 }; x < repairList.size(); ++x) {
					if (repairList[x].getMileage() < repairList[i].getMileage()) {
						std::swap(repairList[i], repairList[x]);
					}
				}
			}
		}

		//If the new repair's mileage is higher than the Vehicle's, set the vehicle's mileage to it
		if (repairList.back().getMileage() > m_mileage) {
			m_mileage = repairList.back().getMileage();
		}

		Log(LogCode::ROUTINE, "Added a repair to " + m_name);
		return true;
	}
	else {
		Log(LogCode::WARNING, "Data not in correct format for a repair");
		return false;
	}

}
bool Vehicle::NewGasStop(uint32_t setMiles, double setGal, double setPPG, std::string setNotes, Date setDate) {
	// Stae variables
	bool milesAccepted{ false }, galAccepted{ false }, ppgAccepted{ false }, notesAccepted{ false };

	if (setMiles > 0) 	{ milesAccepted = true; }
	if (setGal > 0) 	{ galAccepted 	= true; }
	if (setPPG > 0) 	{ ppgAccepted = true; }

	// Ensure the notes size is not longer than maxNotesSize
	if (setNotes.length() <= 0 || setNotes.length() > maxNotesSize) {
		std::ostringstream logText;
		logText << "Gas Stop notes out of range. Max size allowed is " << maxNotesSize << ". Current: " << setNotes.length();
		Log(LogCode::WARNING, logText.str());
	}
	else { notesAccepted = true; }

	// If all inputs are valid, make GasStop type and add to list
	if (milesAccepted && galAccepted && ppgAccepted && notesAccepted) {
		GasStop gsBuffer{ setMiles, setGal, setPPG, setNotes, setDate };
		gasList.push_back(gsBuffer);

		//If the New GasStop's mileage is higher than the last element in the vector, no need to sort.
		if (gasList.size() == 1 || gsBuffer.getMileage() > gasList[gasList.size() - 2].getMileage()) { } // Does nothing if no need to sort
		else { 
			// Sort by mileage
			for (int i{ 0 }; i < gasList.size() - 1; ++i) {
				for (int x{ i + 1 }; x < gasList.size(); ++x) {
					if (gasList[x].getMileage() < gasList[i].getMileage()) {
						std::swap(gasList[i], gasList[x]);
					}
				}
			}
		}

		//If the mileage is higher than the vehicle, set the vehicle's mileage
		if (gasList.back().getMileage() > m_mileage) {
			m_mileage = gasList.back().getMileage();
		}
		Log(LogCode::ROUTINE, "Added a gas stop to " + m_name);
		return true;
	}
	else {
		Log(LogCode::WARNING, "Data not in correct format for a gas stop");
		return false;
	}
}



std::string MakeVehicleName		(std::string& text){
	//Check if the text string exists. If not, exit functin
	if (text == "") { return ""; }

	std::string vehNameBuf{};

	// State Variables
	bool writeChar{ false }, exitLoop{ false };
	int charactersRead{ 0 };

	//Reads text between ( ) to write to vehNameBuf
	for (const char& c : text) { 
		++charactersRead;
		if (c == ')') 	{ writeChar = false; exitLoop = true; }
		if (writeChar) 	{ vehNameBuf += c; }
		if (c == '(') 	{ writeChar = true; }

		// Deletes the number of read characters from the text string
		if (exitLoop) {
			for (int i{ 0 }; i < charactersRead; ++i) {
				text.erase(0, 1);
			}
			return vehNameBuf; 
		}
	}

	return vehNameBuf;
}
uint32_t 	MakeVehicleMiles	(std::string& text){
	//Check if the text string exists. If not return 0 miles
	if (text == "") { return 0; }

	std::stringstream mileStringBuf{};
	uint32_t mileBuf{0};

	// Staate variables
	bool writeChar{ false }, exitLoop{ false };

	int charactersRead{ 0 };
	for (const char& c : text) { //Reads text between ( ) to write to vehNameBuf
		++charactersRead;
		if (c == '}') 	{ writeChar = false; exitLoop = true; }
		if (writeChar) 	{ mileStringBuf << c; }
		if (c == '{') 	{ writeChar = true; }
		
		// Deletes read characters
		if (exitLoop) {
			for (int i{ 0 }; i < charactersRead; ++i) {
				text.erase(0, 1);
			}
			mileStringBuf >> mileBuf;
			return mileBuf;
		}

	}
	return mileBuf;
}
void 		MakeRepair			(std::string& text, Vehicle& veh){
	std::string repBuf;							//Buffer of characters being read from text
	std::vector<std::string> repairStrings; 	//Vector of strings of repair info not yet formatted

	//Reads between < > to find raw repair strings and adds to list
	bool writeChar{ false };
	for (char& c : text) {
		if (c == '>') {
			repairStrings.push_back(repBuf); 	// Adds the string to list of unformatted repairs
			repBuf.erase();						// Resets the buffer to write again
			writeChar = false;
		}

		if (writeChar) 	{ repBuf += c; }
		if (c == '<') 	{ writeChar = true; }
	}

	//Reads unformatted repairStrings, formats them into Repair types and adds to vehicle
	char separator{ '|' };
	for (std::string currentString : repairStrings) {

		// Buffer variables
		uint32_t				mileBuf{};
		std::ostringstream		typeBuf{};
		double					costBuf{};
		std::ostringstream		notesBuf{};
		bool					thirdPartyBuf{};
		std::stringstream		dateStringBuf{};


		// Parsing the text to the buffers. Needs to be in the order of however the file is written
		// in order to write to the correct data types
		FileSystem::readUntilAuto(currentString, separator, mileBuf);
		typeBuf << FileSystem::readUntilString(currentString, separator);
		FileSystem::readUntilAuto(currentString, separator, costBuf);
		FileSystem::readUntilAuto(currentString, separator, thirdPartyBuf);
		notesBuf << FileSystem::readUntilString(currentString, separator);
		dateStringBuf << FileSystem::readUntilString(currentString, separator);


		// Make the typeBuf raw string into a RepairType enum for use
		// in creating a Repair. 
		std::string typeBufString{typeBuf.str()};
		RepairType enumTypeBuf;
		{
			if(typeBufString == "Battery Replacement"){
				enumTypeBuf = RepairType::BATTERY_REPLACEMENT;
			}
			else if(typeBufString == "Lightbulb Replacement"){
				enumTypeBuf = RepairType::LIGHTBULB_REPLACEMENT;
			}
			else if(typeBufString == "Bodywork"){
				enumTypeBuf = RepairType::BODYWORK;
			}
			else if(typeBufString == "Mechanical"){
				enumTypeBuf = RepairType::MECHANICAL;
			}
			else if(typeBufString == "Oil Change"){
				enumTypeBuf = RepairType::OIL_CHANGE;
			}
			else if(typeBufString == "Other"){
				enumTypeBuf = RepairType::OTHER;
			}
			else if(typeBufString == "Power Steering"){
				enumTypeBuf = RepairType::POWER_STEERING_FLUID_EXCHANGE;
			}
			else if(typeBufString == "Tire Replacement"){
				enumTypeBuf = RepairType::TIRE_REPLACEMENT;
			}
			else if(typeBufString == "Tire Rotation"){
				enumTypeBuf = RepairType::TIRE_ROTATION;
			}
			else if(typeBufString == "Transmission Fluid"){
				enumTypeBuf = RepairType::TRANSMISSION_FLUID_EXCHANGE;
			}
			else if(typeBufString == "Wiper Blade Replacement"){
				enumTypeBuf = RepairType::WIPER_REPLACEMENT;
			}
			else{
				enumTypeBuf = RepairType::OTHER;
			}
		}
		
		// Read the dateStringBuf string and make a Date type
		Date dateBuf{};
		dateBuf.makeDate(dateStringBuf);

		// Create the repair type
		veh.NewRepair(mileBuf, enumTypeBuf, costBuf, notesBuf.str(), thirdPartyBuf, dateBuf);
	}

	return;
}
void 		MakeGasStop			(std::string& text, Vehicle& veh){

	std::string foundGasBuf;				//Buffer of characters being read from text
	std::vector<std::string> gasStrings;	//Vector of unformatted GasStop strings

	//Read between [ ], write to gasStrings to be formatted to GasStops
	bool writeChar{ false };
	for (char& c : text) {
		if (c == ']') {
			gasStrings.push_back(foundGasBuf);
			foundGasBuf.erase();
			writeChar = false;
		}
		if (writeChar) 	{ foundGasBuf += c; }
		if (c == '[') 	{ writeChar = true; }
	}

	//Reads unformatted gasStrings, formats them into GasStop types and adds to Vehicle
	char separator{ '|' };
	for (std::string currentString : gasStrings) {
		// Buffer Variables
		uint32_t			mileBuf{};
		double				gallonsBuf{};
		double				ppgBuf{};
		std::ostringstream	notesBuf{};
		std::stringstream	dateStringBuf{};

		// Parsing the text to the buffers. Needs to be in the order of however the file is written
		// in order to write to the correct data types
		FileSystem::readUntilAuto(currentString, separator, mileBuf);
		FileSystem::readUntilAuto(currentString, separator, ppgBuf);
		FileSystem::readUntilAuto(currentString, separator, gallonsBuf);
		notesBuf << FileSystem::readUntilString(currentString, separator);
		dateStringBuf << FileSystem::readUntilString(currentString, separator);

		Date dateBuf{};
		dateBuf.makeDate(dateStringBuf);

		veh.NewGasStop(mileBuf, gallonsBuf, ppgBuf, notesBuf.str(), dateBuf);
	}

	return;
}

bool WriteToFile	(const Application& app, Vehicle& veh) {
	std::ostringstream saveFileName{app.getVehicleDirectory() + veh.getName() + ".dat"};	// Sets the name to the Vehicle's m_name
	std::ostringstream fileText; 															// Buffer of what will be written to the file

	// Vehicle file "Header" portion of name and mileage
	fileText << '(' << veh.getName() << ')';
	fileText << '{' << veh.getMileage() << "}\n";
	
	for (Repair& repair : veh.getRepairList()) {
		fileText << '<';
		fileText << repair;
		fileText << '>';
		fileText << '\n';
	}
	for (GasStop& gasStop : veh.getGasStopList()) {
		fileText << '[';
		fileText << gasStop;
		fileText << ']';
		fileText << '\n';
	}
	
	return FileSystem::writeToFile(saveFileName.str(), fileText.str());
}
bool SaveVehicles	(const Application& app) {
	bool success{ false };

	// Iterates through s_vehicleList and writes information to a file of it's m_name
	for (Vehicle& currentVehicle : Vehicle::s_vehicleList) {
		if (!WriteToFile(app, currentVehicle)) {
			Log(LogCode::WARNING, "Could not save vehicle information for " + currentVehicle.getName());
			success = false;
		}
		else {
			Log(LogCode::LOG, "Saved vehicle information for " + currentVehicle.getName());
			success = true;
		}
	}

	return success;
}
bool DeleteVehicle	(Application* app, Vehicle& veh){
	veh.delFromVehList(); // Delete from s_VehicleList

	// Delete the actual file itself
	std::string deletePath{app->getVehicleDirectory() + veh.getName() + ".dat"}; 
	return FileSystem::deleteFile(deletePath);
}




// ImGui Implementation Below

// Moved to top to get rid of compiler not finding friend function SelectableVehicleList()
Vehicle* SelectableVehicleList() {
	static Vehicle* selVeh{nullptr};

	// Create the selectable list of vehicles
	if(Vehicle::s_vehicleList.size() > 0){
		if (ImGui::BeginListBox("Current Vehicles", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y-20))) 
		{
			// Numbers the vehicles in the list
			static int selected = -1;
			int vehNum{0};
			for (Vehicle& currentVehicle : Vehicle::s_vehicleList )
			{	
				++vehNum;
				char buf[32];
				sprintf(buf, "%d. %s", vehNum, currentVehicle.getName().c_str());
				if (ImGui::Selectable(buf, selected == vehNum))
				{
					selected = vehNum;
					selVeh = &currentVehicle;
				}
				ImGui::SameLine(200); ImGui::Text("%d miles", currentVehicle.getMileage()); 
			}
			ImGui::EndListBox();
		}
	}
	else{
		ImGui::Text("There are no tracked vehicles");
	}

	return selVeh;
}
void VehicleManager(Application* app, bool &shown){
	if(ImGui::Begin("Vehicle Manager", &shown, 0)){
		static bool viewVeh = false;
		static bool editVeh = false;

		static Vehicle* selectedVehicle{nullptr};

		// Welcome message at top of window -------
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth   = ImGui::CalcTextSize("Vehicle Manager").x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text("Vehicle Manager");
		ImGui::Spacing();

		ImGui::TextWrapped(	"This Vehicle Manager stores all relevant information about a vehicle such as its name,"
							" mileage, and repair and gas stop information. ");
		ImGui::Spacing();
		ImGui::Spacing();
		// ----------------------------------------


		if(Vehicle::vehListIsEmpty()){
			ImGui::Text("There are no tracked vehicles");
		}
		else{	// This draws the child window for the vehicle list


			// Algorithm for adjusting child window size --------
			static float bigWindowX;
			bigWindowX = ImGui::GetWindowContentRegionMax().x;
			static float childY = 200;

			static float childX {500};
			if(bigWindowX > 510){
				// Do nothing therefore keep window at 500 pixels if the parent window is larger than specified
			}
			else{
				childX = bigWindowX - 10;	// If smaller than 510, rersize to the window size minus 10 pixels
			}
			// -------------------------------------------------

			float vehWinX; //Forward declaration to get the size of the vehicle draw window to size buttons appropriately

			// Create and draw the child window
			ImGui::Text("Tracked Vehicles");
			ImGui::SameLine(); ImGui::HelpMarker("Select a vehicle then choose from the options below");
			ImGui::BeginChild("#Current Vehicles", ImVec2(childX, childY), false, ImGuiWindowFlags_AlwaysAutoResize);
			selectedVehicle = SelectableVehicleList();
			vehWinX = ImGui::GetWindowContentRegionWidth();
			ImGui::EndChild();
			
			// Once/if vehicle is selected, these buttons appear
			if(selectedVehicle){

				//Size the buttons correctly
				constexpr float MIN_BUTTON_SIZE = 70;
				static float buttonX;
				static float buttonY{30};
				static ImVec2 buttonSize (0, buttonY);
				static float winPadding{20};

				// See if there are 3 or 4 buttons shown
				static int buttonsShown {3};
				if(viewVeh || editVeh){
					buttonsShown = 4;
				}
				else{
					buttonsShown = 3;
				}

				// Ensure buttons are at least the min size specified
				if((vehWinX - winPadding) < (MIN_BUTTON_SIZE * buttonsShown)){
					buttonX = MIN_BUTTON_SIZE;
				}
				else{ // Math the buttonSize to change with the size of the parent window
					buttonX = (vehWinX - winPadding)  / buttonsShown;	
				}

				buttonSize.x = buttonX;
				
				ImGui::Spacing();
				if(ImGui::Button("View", buttonSize)){
					editVeh = false;
					viewVeh = true;
				}
				ImGui::SameLine();
				if(ImGui::Button("Edit", buttonSize)){
					viewVeh = false;
					editVeh = true;
				}
				ImGui::SameLine();  
				if(ImGui::Button("Delete", buttonSize)){
					ImGui::OpenPopup("Delete?");
				}

				// Code for popup modal
				// Always center this window when appearing
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("This will delete all of the Vehicle's information\nThis operation cannot be undone!\n\n");
					ImGui::Separator();

					if (ImGui::Button("OK", ImVec2(120, 0))) 
					{
						Vehicle vehBufferToDel = *selectedVehicle;
						selectedVehicle = nullptr;
						viewVeh = false;
						editVeh = false;
						if(DeleteVehicle(app, vehBufferToDel)){
							Log(LogCode::WARNING, "Failed to delete vehicle. In Display.cpp");
						}
						ImGui::CloseCurrentPopup(); 
					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) 
					{ 
						ImGui::CloseCurrentPopup(); 
					}
					ImGui::EndPopup();
				} // End popup modal code 


				// Button to close the detailed info window
				if(viewVeh || editVeh){
					ImGui::SameLine(); 
					if(ImGui::Button("Close", buttonSize)){
						viewVeh = false;
						editVeh = false;
					}
				}
			} 


			//What happens when a vehicle action button is hit
			if(viewVeh || editVeh){
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				static float bigWindowX = ImGui::GetWindowContentRegionMax().x;				

				static float childX {700};
				static float childY {410};
				if(bigWindowX > 710){

				}
				else{
					childX = bigWindowX - 20;
				}

				//Starting new child window with vehicle info
				if(viewVeh) ImGui::Text("View %s", selectedVehicle->getName().c_str());
				if(editVeh) ImGui::Text("Edit %s", selectedVehicle->getName().c_str());
				ImGui::SameLine(); ImGui::HelpMarker("This shows all the vehicle information");
				ImGui::SameLine(); 


				if(ImGui::Button("Inc Width", (ImVec2 (90, 20)))){
					childX += 10;
				}
				ImGui::SameLine(); 
				if(ImGui::Button("Dec Width", (ImVec2 (90, 20)))){
					childX -= 10;
				}

				if(viewVeh){
					if(!selectedVehicle){ 
						//Ensure there is a selected Vehicle
					}
					else{
						if(ImGui::BeginChild("#View Full Vehicle Info", ImVec2(childX, childY), false)){
							ShowFullVehicleInformation(selectedVehicle);
						}
						ImGui::EndChild();
					}
					
				}

				if(editVeh){
					if(ImGui::BeginChild("#Edit Vehicle Info", ImVec2(childX, childY), false)){
						EditVehicle(selectedVehicle);
					}
					ImGui::EndChild();
				}

			}
			else{
				ImGui::Spacing();
				ImGui::TextWrapped("A repair can track:");
				ImGui::BulletText("Mileage it was done");
				ImGui::BulletText("The type of repair it was (oil change, light replacement, washer fluid fill, etc.)");
				ImGui::BulletText("How much the total cost was");
				ImGui::BulletText("Was it done by a third party?");
				ImGui::BulletText("Any notes you would like to add");
				ImGui::BulletText("And the date it was done");
			}
		}
	}
	ImGui::End(); //End Vehicle Manager window
	return;
}
void SetupVehicleManager(Application* app){

	//Go through the vehicle folder and store the names of the files
	std::vector<std::string> vehicleFiles;
	FileSystem::filesInDirectory(app->getVehicleDirectory(), vehicleFiles);

	//Iterate through the files in the folder found and create Vehicle types
	for (const std::string& fileName : vehicleFiles) {
		std::ostringstream fileText;

		//Opens file, reads text, outputs to fileText
		if (!FileSystem::readFile(fileName, fileText)) {
			//If vehicle file couldn't be opened to read, do nothing
		}
		else {
			std::string vehInfoBuf{ fileText.str() };	//Stores the file text in string format
			Vehicle vehicleBuffer{ MakeVehicleName(vehInfoBuf), MakeVehicleMiles(vehInfoBuf) };
		
			MakeRepair(vehInfoBuf, vehicleBuffer);		//Make repairs from the remaining text and add to vehicle
			MakeGasStop(vehInfoBuf, vehicleBuffer);		//Make Gas stop from the remaining text

			vehicleBuffer.addToVehicleList();			//Add vehicleBuffer to the master vehicle list
		}
	}		
}
void EditVehicle(Vehicle* veh){
	static ImVec2 buttonSize;
	buttonSize.x = ImGui::GetWindowWidth() / 2;
	buttonSize.y = 30;
	
	ImGui::Text("Name: \t%s", veh->getName().c_str());
	ImGui::Text("Miles:            \t   %d", veh->getMileage());
	ImGui::Text("Number of repairs \t   %d", veh->getRepairList().size());
	ImGui::Text("Number of gas stops: \t%d", veh->getGasStopList().size());
	ImGui::Button("Add Repair", buttonSize); ImGui::SameLine();
	ImGui::Button("Add Gas Stop", buttonSize);

	return;
}

void ShowFullVehicleInformation(Vehicle* veh) {
	static const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
	ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);

	ImGui::Text("Repairs");
	ImGui::Spacing();
	if(ImGui::BeginTable("Repairs", 6, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg , outer_size ))
	{
		ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Miles", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Cost", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Third Party", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Notes", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableHeadersRow();

		for (Repair& rep : veh->getRepairList())
		{
			ImGui::TableNextRow();
			int				mileBuf;
			std::string		typeBuf;
			double			costBuf;
			std::string		notesBuf;
			bool			thirdPartyBuf;
			std::string		dateBuf;
			rep.getRepairInfo(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf, dateBuf);

			int column{0};
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", dateBuf.c_str());
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%d", mileBuf);
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", typeBuf.c_str());
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%f", costBuf);
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", thirdPartyBuf ? "true" : "false");
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", notesBuf.c_str());	
		}
	ImGui::EndTable();
	}

	ImGui::Spacing();
	ImGui::Text("Gas Stops");
	ImGui::Spacing();

	if(ImGui::BeginTable("Gas Stops", 5, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg , outer_size ))
	{
		ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Miles", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Gallons", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Per Gal", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Notes", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableHeadersRow();

		for (GasStop& gas : veh->getGasStopList())
		{
			ImGui::TableNextRow();
			int			mileBuf;
			double		galBuf;
			double		costBuf;
			std::string notesBuf;
			std::string dateBuf;
			gas.getGasStopInfo(mileBuf, galBuf, costBuf, notesBuf, dateBuf);

			int column{0};

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", dateBuf.c_str());
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%d", mileBuf);
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%f", galBuf);
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%f", costBuf);
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", notesBuf.c_str());	
		}
	ImGui::EndTable();
	}
	return;
}