#include "Vehicle.h"

std::vector<Vehicle> VehicleManager::s_vehicleList{}; // Initializer for s_vehicleList to be used


// Operator Overloads
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



Vehicle::Vehicle(const std::string name, uint32_t mileage) {
	
	setName(name);
	// Make sure the mileage is not less than 0
	if (mileage < 0) {
		RST::Log("Vehicle mileage for [" + m_name + "] was below zero. Set to 0", LogCode::WARNING);
		m_mileage = 0;
	}
	else {
		m_mileage = mileage;
	}


	return;
}
bool Vehicle::NewRepair(uint32_t setMiles, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, Date date) {

	// State variables used for error checking
	bool milesAccepted{ false }, costAccepted{ false }, notesAccepted{ false }, dateAccepted{false};

	if (setMiles > 0) 	{ milesAccepted = true; }
	if (setCost >= 0) 	{ costAccepted 	= true; }

	notesAccepted 	= CheckStringSize(setNotes, maxNotesSize);						// Ensure the notes size is not longer than maxNotesSize
	dateAccepted 	= DateTime::CheckDate(date.day, date.month, date.year);			// Ensure valid Date

	// If all conditions are valid, make the Repair and add it to the Vehicle's list of repairs
	if (milesAccepted && costAccepted && notesAccepted && dateAccepted) {
		Repair repBuf{ setMiles, setType, setCost, setNotes, setThirdParty, date };
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

		RST::Log("Added Repair to [" + getName() +']', LogCode::LOG_LOW);
		setMileage(setMiles);
		setLasUpdated(date);

		return true;
	}
	else {
		RST::Log("Could not add Repair to [" + getName() +']', LogCode::ERROR);
		if(!milesAccepted) 	{ RST::Log("Mileage was not accepted", LogCode::ERROR); }
		if(!costAccepted) 	{ RST::Log("Cost was not accepted", LogCode::ERROR); }
		if(!notesAccepted) 	{ RST::Log("Notes were not accepted", LogCode::ERROR); }
		if(!dateAccepted) 	{ RST::Log("Date was not accepted", LogCode::ERROR); }
		return false;
	}

}
bool Vehicle::NewGasStop(uint32_t setMiles, double setGal, double setPPG, std::string setNotes, Date date) {
	// Stae variables
	bool milesAccepted{ false }, galAccepted{ false }, ppgAccepted{ false }, notesAccepted{ false }, dateAccepted{false};

	if (setMiles > 0) 	{ milesAccepted = true; }
	if (setGal > 0) 	{ galAccepted 	= true; }
	if (setPPG >= 0) 	{ ppgAccepted = true; }

	// Ensure the notes size is not longer than maxNotesSize
	notesAccepted 	= CheckStringSize(setNotes, maxNotesSize);
	dateAccepted 	= DateTime::CheckDate(date.day, date.month, date.year);			// Ensure valid Date

	// If all inputs are valid, make GasStop type and add to list
	if (milesAccepted && galAccepted && ppgAccepted && notesAccepted && dateAccepted) {
		GasStop gsBuffer{ setMiles, setGal, setPPG, setNotes, date };
		gasList.push_back(gsBuffer);

		// Sort by mileage
		// If the New GasStop's mileage is higher than the last element in the vector, no need to sort.
		if (gasList.size() == 1 || gsBuffer.getMileage() > gasList[gasList.size() - 2].getMileage()) { } // Does nothing if no need to sort
		else { 
			for (int i{ 0 }; i < gasList.size() - 1; ++i) {
				for (int x{ i + 1 }; x < gasList.size(); ++x) {
					if (gasList[x].getMileage() < gasList[i].getMileage()) {
						std::swap(gasList[i], gasList[x]);
					}
				}
			}
		}

		RST::Log("Added Gas Stop to [" + getName() +']', LogCode::LOG_LOW);
		setMileage(setMiles);
		setLasUpdated(date);

		return true;
	}
	else {
		RST::Log("Could not add Gas Stop to [" + getName() +']', LogCode::ERROR);
		if(!milesAccepted) 	{ RST::Log("Mileage was not accepted", LogCode::ERROR); }
		if(!galAccepted) 	{ RST::Log("Number of Gallons was not accepted", LogCode::ERROR); }
		if(!ppgAccepted) 	{ RST::Log("Price Per Gallon was not accepted", LogCode::ERROR); }
		if(!notesAccepted) 	{ RST::Log("Notes were not accepted", LogCode::ERROR); }
		if(!dateAccepted) 	{ RST::Log("Date was not accepted", LogCode::ERROR); }

		return false;
	}
}

bool Vehicle::setMileage(const uint32_t setMileage){
	bool success{false};

	// If given mileage is greater, assign it to the vehicle
	if (setMileage > m_mileage) {
		m_mileage = setMileage;
		success = true;

		// Log mileage was changed
		std::ostringstream txt{}; txt <<"Set Vehicle [" << getName() <<"] mileage to [" << m_mileage << "]";
		RST::Log(txt.str(), LogCode::LOG_MED);
	}
	else{
		std::ostringstream txt{}; txt <<"Mileage was not set for Vehicle [" << getName() <<"]. Current mileage is [" << m_mileage << "], given mileage was [" << setMileage << "]";
		RST::Log(txt.str(), LogCode::RUNTIME_LOW);
	}

	return success;
}
bool Vehicle::setLasUpdated(const Date date){
	bool success{false};

	if(date.year < m_lastUpdated.year){
		RST::Log("Given year was prior to Last Updated year for [" + getName() + "]", LogCode::RUNTIME_LOW);
		return false;
	}
	else{
		if(date.month < m_lastUpdated.month){
			RST::Log("Given month was prior to Last Updated month for [" + getName() + "]", LogCode::RUNTIME_LOW);
			return false;
		}
		else{
			if(date.month == m_lastUpdated.month && date.day < m_lastUpdated.day){
				RST::Log("Given day was prior to Last Updated day for [" + getName() + "]", LogCode::RUNTIME_LOW);
				return false;
			}
			else{
				m_lastUpdated = date;
				std::ostringstream txt; txt << m_lastUpdated;
				RST::Log("Set Vehicle [" + getName() + "] last updated to [" + txt.str() + "]", LogCode::LOG_MED);
				success = true;
			}
		}
	}

	return success;
}
bool Vehicle::setName(const std::string setName){
	bool success{false};

	// Make sure the name is not longer than maxVehicleNameSize
	if (!CheckStringSize(setName, maxVehicleNameSize)) {
		m_name = "Vehicle";

		// Log the error
		std::ostringstream logText;
		logText << "Vehicle name [" << setName << "] was too long. The name was set to \"Vehicle\" ";
		RST::Log(logText.str(), LogCode::ERROR);
		
		success = false;
	}
	else {
		RST::Log("Vehicle [" + m_name + "] has been renamed to [" + setName + "]", LogCode::LOG_MED);
		m_name = setName;
		
		success = true;
	}

	return success;
}


// Vehicle Manager Implementation

VehicleManager::VehicleManager(Application* app) : Module("Vehicle Manager", app)		{ }
VehicleManager::~VehicleManager()														{ }
Vehicle* VehicleManager::SelectableVehicleList(bool &reset) {
	static Vehicle* selVeh{nullptr};

	if(reset){ reset = false; selVeh = nullptr; } // If reset is true, make selected vehicle null and put the flag back to false

	// Create the selectable list of vehicles
	if(s_vehicleList.size() > 0){
		if (ImGui::BeginListBox("Current Vehicles", ImVec2(ImGui::GetWindowSize().x, 120))) 
		{
			// Numbers the vehicles in the list
			static int selected = -1;
			int vehNum{0};
			for (Vehicle& currentVehicle : s_vehicleList )
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
	else{ ImGui::Text("There are no tracked vehicles"); }

	return selVeh;
}
void VehicleManager::ShowFullVehicleInformation(Vehicle* veh) {
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

			// Buffers for the specified repair that will be overweitten
			int				mileBuf;
			std::string		typeBuf;
			double			costBuf;
			std::string		notesBuf;
			bool			thirdPartyBuf;
			std::string		dateBuf;

			// Write to the buffers
			rep.getRepairInfo(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf, dateBuf);

			// Display in the table
			int column{0};
			ImGui::TableSetColumnIndex(column);	
			ImGui::Text("%s", dateBuf.c_str());						// Date
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%d", mileBuf);								// Mileage
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", typeBuf.c_str());						// Type of Repair
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("$ %4f", costBuf);							// Cost of Repair
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", thirdPartyBuf ? "true" : "false");	// Third Party
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", notesBuf.c_str());					// Notes
		}
	ImGui::EndTable();
	}

	ImGui::NewLine();
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

			// Buffers for the specified GasStop that will be overweitten
			int			mileBuf;
			double		galBuf;
			double		costBuf;
			std::string notesBuf;
			std::string dateBuf;

			// Write to the buffers
			gas.getGasStopInfo(mileBuf, galBuf, costBuf, notesBuf, dateBuf);

			int column{0};

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", dateBuf.c_str());		// Date
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%d", mileBuf);				// Mileage
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%f", galBuf);				// Gallons total
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("$ %f", costBuf);			    // Cost per gallon
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", notesBuf.c_str());	// Notes
		}
	ImGui::EndTable();
	}
	return;
}
bool VehicleManager::SaveAllVehicles	() {
	bool success{ false };

	// Iterates through s_vehicleList and writes information to a file of it's m_name
	for (Vehicle& currentVehicle : s_vehicleList) {
		if (!SaveVehicle(&currentVehicle)) {
			RST::Log("Could not save vehicle information for " + currentVehicle.getName(), LogCode::WARNING);
			success = false;
		}
		else {
			RST::Log("Saved vehicle information for " + currentVehicle.getName(), LogCode::LOG_HIGH);
			success = true;
		}
	}
	return success;
}

// -----------------------




// Helper Functions

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


	// For logging
	size_t 	numRepairsTotal{repairStrings.size()};
	unsigned short 	numRepSuccessfullyAddedToVehicle{0}, numRebFailedToAddToVehicle{0};

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
		DateTime::MakeDate(dateStringBuf, dateBuf);

		// Create the repair type
		if(veh.NewRepair(mileBuf, enumTypeBuf, costBuf, notesBuf.str(), thirdPartyBuf, dateBuf)) { ++numRepSuccessfullyAddedToVehicle; }
		else { ++numRebFailedToAddToVehicle; }
	}

	// Log total unformatted strings, how many were successful at adding to the vehicle and how many failed
	std::ostringstream msg; 
	msg << "Vehicle file [" << veh.getName() <<"] had [" << numRepairsTotal << "] unformatted Repair strings. [" 
		<< numRepSuccessfullyAddedToVehicle << "] were added to vehicle, [" << numRebFailedToAddToVehicle << "] could not be added to vehicle";
	RST::Log(msg.str(), LogCode::LOG_HIGH);

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

	// For logging
	size_t numGSTotal{gasStrings.size()};
	unsigned short 	numGSSuccessfullyAddedToVehicle{0}, numGSFailedToAddToVehicle{0};


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
		DateTime::MakeDate(dateStringBuf, dateBuf);

		if(veh.NewGasStop(mileBuf, gallonsBuf, ppgBuf, notesBuf.str(), dateBuf)){ ++numGSSuccessfullyAddedToVehicle; }
		else { ++numGSFailedToAddToVehicle; }
	}

	// Log total unformatted strings, how many were successful at adding to the vehicle and how many failed
	std::ostringstream msg; 
	msg << "Vehicle file [" << veh.getName() <<"] has [" << numGSTotal << "] unformatted Gas Stop strings. [" 
		<< numGSSuccessfullyAddedToVehicle << "] were added to vehicle, [" << numGSFailedToAddToVehicle << "] could not be added to vehicle";
	RST::Log( msg.str(), LogCode::LOG_HIGH);

	return;
}
bool 		CheckStringSize		(const std::string text, int maxAllowed){
	// If length is zero, it's below the max allowed so return TRUE
	if(text.length() == 0){
		return true;
	}

	// Ensure the notes size is not longer than maxNotesSize
	if (text.length() < 0 || text.length() > maxAllowed) {
		std::ostringstream logText;
		logText << "String [" << text << "] out of range at [" << text.length() << "] characters. Max allowed is [" << maxAllowed << "]";
		RST::Log(logText.str(), LogCode::RUNTIME_HIGH);

		return false;
	}
	else { return true; }
}

bool 		AddGasStop(Vehicle* veh, bool& wasSaved){
	bool success{false};

	// Flags for if input was not accepted, these set to TRUE
	static bool showMileageOutOfRangeMsg { false };
	static bool showGalOutOfRangeMsg	 { false };
	static bool showPPGOutOfRangeMsg	 { false };
	static bool badNotes	{false};
	static bool badDate		{false};

	// Input field buffers
	static uint32_t 	mileBuf		{ 0 };
	static double 		galBuf		{ 0.0 };
	static double 		ppgBuf		{ 0.0 };
	static char 		notesBuf	[veh->maxNotesSize];
	static uint16_t day{1}, month{1}, year{1900};

	// Check inputs as they are typed
	if(mileBuf >= 500000 || mileBuf <= 0)   { mileBuf = 0; showMileageOutOfRangeMsg = true;}	else {showMileageOutOfRangeMsg 	= false; }
	if(galBuf >= 300 || galBuf <= 0) 		{ galBuf = 0; showGalOutOfRangeMsg 		= true;} 	else {showGalOutOfRangeMsg 		= false; }
	if(ppgBuf >= 300 || ppgBuf < 0) 		{ ppgBuf = 0; showPPGOutOfRangeMsg 		= true;} 	else {showPPGOutOfRangeMsg 		= false; }


	ImGuiMods::CenterText("Enter in the required information to add a new Gas Stop to the Vehicle");
	ImGui::NewLine();

	ImGui::Text("Mileage          "); ImGui::SameLine();
 	ImGui::InputScalar("##MileBuf", ImGuiDataType_U32, &mileBuf, NULL, NULL, "%u");
	if(showMileageOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Mileage out of range. Mileage must be real.");}
	

	ImGui::Text("Gallons          "); ImGui::SameLine();
 	ImGui::InputScalar("##galBuf", ImGuiDataType_Double, &galBuf, NULL, NULL, "%lf");
	if(showGalOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Gallons out of range. Gallons must be real.");}

	ImGui::Text("Price Per Gallon "); ImGui::SameLine();
 	ImGui::InputScalar("##ppgBuf", ImGuiDataType_Double, &ppgBuf, NULL, NULL, "%lf");
	if(showPPGOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Price Per Gallon out of range. Price Per Gallon must be real.");}


	ImGui::Text("Notes            "); ImGui::SameLine();
	ImGui::InputText("##Notes", notesBuf, veh->maxNotesSize);
	if(badNotes) {ImGui::SameLine(); ImGui::Text("Notes out of range. Max size allowed is %d characters", Vehicle::maxVehicleNameSize); }


	ImGui::NewLine();
	if(badDate) {ImGui::Text("The date entered was invalid. Try again!"); }
	ImGui::Text("Day    "); 	ImGui::SameLine(); ImGui::InputScalar("##Day", ImGuiDataType_U16, &day, NULL, NULL, "%d");
	ImGui::Text("Month  "); 	ImGui::SameLine(); ImGui::InputScalar("##Month", ImGuiDataType_U16, &month, NULL, NULL, "%d");
	ImGui::Text("Year   "); 	ImGui::SameLine(); ImGui::InputScalar("##Year", ImGuiDataType_U16, &year, NULL, NULL, "%d");


	static bool inputsAccepted { false };
	static bool alreadySaved{ false };
	if(ImGuiMods::BeginPopupModal("New Gas Stop")){
	
		if(inputsAccepted){
			// Inputs were accepted
			if(!alreadySaved) { success = veh->NewGasStop(mileBuf, galBuf, ppgBuf, std::string{notesBuf}, Date{day, month, year}); alreadySaved = true;} 

			// Reset Buffers
			mileBuf = 0.0;
			ppgBuf 	= 0.0;
			galBuf 	= 0.0;
			day = 1; month = 1; year = 1900;
			
			int i{0};						// Clear all of the notesBuffer
			for( char& c : notesBuf) { notesBuf[i] = NULL; ++i; }
					
			wasSaved = true;

			ImGui::NewLine(); ImGui::Text("Gas Stop added to %s", + veh->getName().c_str()); ImGui::NewLine();
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); alreadySaved = false; inputsAccepted = false;}

		}
		else{
			ImGuiMods::CenterText("One or more of your inputs were invalid. \nPlease try again");
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup();  alreadySaved = false; }
		}
		ImGui::EndPopup();
	}
	

	// Assign the buffers to the vehicle
	ImGui::NewLine();
	ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - 75 );
	if(ImGui::Button("Save", ImVec2(150, 30))){
		// Moved checking of inputs here to remove the repeated checking in the Popup Modal loop
		if(CheckStringSize(notesBuf, Vehicle::maxVehicleNameSize)) 	{ badNotes = false; } else { badNotes = true; }
		if(DateTime::CheckDate(day, month, year)) 							{ badDate = false;  } else { badDate = true; }

		if(mileBuf > 0 && ppgBuf >= 0.0 && galBuf > 0.0 && !badNotes && !badDate) { inputsAccepted = true; } else { inputsAccepted = false; }

		ImGui::OpenPopup("New Gas Stop"); 
	}

	return success;
}
bool 		AddRepair(Vehicle* veh, bool& wasSaved){
	bool success{false};

	// Flags for if input was not accepted, these set to TRUE
	static bool showMileageOutOfRangeMsg { false };
	static bool showCostOutOfRangeMsg	 { false };
	static bool badNotes	{false};
	static bool badDate		{false};

	// Input field buffers
	static uint32_t 	mileBuf			{ 0 };
	static double 		costBuf			{ 0.0 };
	static bool 		thirdPartyBuf;
	static char 		notesBuf		[Vehicle::maxVehicleNameSize];
	static uint16_t day{1}, month{1}, year{1900};

	// Check inputs as they are typed
	if(mileBuf >= 500000 || mileBuf <= 0) { mileBuf = 0; showMileageOutOfRangeMsg = true;}	else {showMileageOutOfRangeMsg = false; }
	if(costBuf >= 500000 || costBuf < 0) { costBuf = 0; showCostOutOfRangeMsg = true;} 		else {showCostOutOfRangeMsg = false; }

	ImGuiMods::CenterText("Enter in the required information to add a new Repair to the Vehicle");
	ImGui::NewLine();

	ImGui::Text("Mileage         "); ImGui::SameLine();
 	ImGui::InputScalar("##MileBuf", ImGuiDataType_U32, &mileBuf, NULL, NULL, "%u");
	if(showMileageOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Mileage out of range. Mileage must be real.");}


	ImGui::Text("Type	        "); ImGui::SameLine();
	const char* repairTypes[] = { "Oil Change", "Transmission Fluid Exchange", "Lightbulb Replacement", "Power Steering Fluid Exchange",
		 "Wiper Blade Replacement", "Tire Rotation", "Tire Replacement", "Bodywork", "Mechanical Work", "Battery Replacement", "Other" }; 	// Options for the combo box
	
	static int repairTypeIndex = 10; 																										// Here we store our selected member data as an index of the array
	const char* combo_preview_value = repairTypes[repairTypeIndex];  																		// Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo("##RepairType", combo_preview_value))
	{
		for (int n = 0; n < IM_ARRAYSIZE(repairTypes); n++)
		{
			const bool is_selected = (repairTypeIndex == n);
			if (ImGui::Selectable(repairTypes[n], is_selected))
				repairTypeIndex = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected) { ImGui::SetItemDefaultFocus(); }
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Cost	        "); ImGui::SameLine();
 	ImGui::InputScalar("##cost", ImGuiDataType_Double, &costBuf, NULL, NULL, "%lf");
	if(showCostOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Cost out of range. Cost must be real."); }


	ImGui::Text("Notes           "); ImGui::SameLine();
	ImGui::InputText("##Notes", notesBuf, veh->maxNotesSize);
	if(badNotes) {ImGui::SameLine(); ImGui::Text("Notes out of range. Max size allowed is %d characters", Vehicle::maxVehicleNameSize); }

	ImGui::Checkbox("Third Party", &thirdPartyBuf);

	ImGui::NewLine();
	if(badDate) {ImGui::Text("The date entered was invalid. Try again!"); }
	ImGui::Text("Day    "); 	ImGui::SameLine(); ImGui::InputScalar("##Day", 		ImGuiDataType_U16, &day, NULL, NULL, "%d");
	ImGui::Text("Month  "); 	ImGui::SameLine(); ImGui::InputScalar("##Month", 	ImGuiDataType_U16, &month, NULL, NULL, "%d");
	ImGui::Text("Year   "); 	ImGui::SameLine(); ImGui::InputScalar("##Year", 	ImGuiDataType_U16, &year, NULL, NULL, "%d");
	

	static bool inputsAccepted { false };
	static bool alreadySaved{ false };
	if(ImGuiMods::BeginPopupModal("New Repair")){
	
		if(inputsAccepted){
			// Inputs were accepted
			if(!alreadySaved) { success = veh->NewRepair(mileBuf, static_cast<RepairType>(repairTypeIndex + 1), costBuf, std::string{notesBuf}, thirdPartyBuf, Date{day, month, year}); alreadySaved = true;} 

			// Reset Buffers
			mileBuf = 0.0;
			costBuf = 0.0;
			repairTypeIndex = 10;
			
			int i{0};						// Clear all of the notesBuffer
			for( char& c : notesBuf) { notesBuf[i] = NULL; ++i; }
			
			thirdPartyBuf = false;
			day = 1; month = 1; year = 1900;
			
			wasSaved = true;
			
			ImGui::NewLine(); ImGui::Text("Repair added to %s", + veh->getName().c_str()); ImGui::NewLine();
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); alreadySaved = false; inputsAccepted = false;}
		}
		else{
			ImGuiMods::CenterText("One or more of your inputs were invalid. \nPlease try again");
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup();  alreadySaved = false; }
		}

		ImGui::EndPopup();
	}



	// Assign the buffers to the vehicle
	ImGui::NewLine();
	ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - 75 );
	if(ImGui::Button("Save", ImVec2(150, 30)))	{ 
		// Moved checking of inputs here to remove the repeated checking in the Popup Modal loop
		if(CheckStringSize(notesBuf, Vehicle::maxVehicleNameSize)) 	{ badNotes = false; } else { badNotes = true; }
		if(DateTime::CheckDate(day, month, year)) 					{ badDate = false;  } else { badDate = true; }

		if(mileBuf > 0 && costBuf >= 0.0 && !badNotes && !badDate) { inputsAccepted = true; } else { inputsAccepted = false; }

		ImGui::OpenPopup("New Repair"); 
	}
	return success;
}

