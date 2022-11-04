#include "Vehicle.h"

std::string Date::string() const{
	std::ostringstream os;
	if(day == 0 && month == 0 && year == 0){
		os << "NA";
		return os.str();
	}

	os << day << ' ';

	switch (month) {
	case 1:
		os << "JAN";
		break;
	case 2:
		os << "FEB";
		break;
	case 3:
		os << "MAR";
		break;
	case 4:
		os << "APR";
		break;
	case 5:
		os << "MAY";
		break;
	case 6:
		os << "JUN";
		break;
	case 7:
		os << "JUL";
		break;
	case 8:
		os << "AUG";
		break;
	case 9:
		os << "SEP";
		break;
	case 10:
		os << "OCT";
		break;
	case 11:
		os << "NOV";
		break;
	case 12:
		os << "DEC";
		break;
	default:
		os << "MMM";
	}

	os << ' ' << year;

	return os.str();
}

void Repair::getRepairInfo(int& mileage, std::string& typeStr, double& costDbl, std::string& notesVar, bool& wasThirdParty, std::string& dateVar) const {
	mileage = m_mileageDone;

	std::ostringstream typeVarBuf; typeVarBuf << m_type;
	typeStr = typeVarBuf.str();
	
	costDbl = m_cost;
	notesVar = m_notes;
	wasThirdParty = m_isThirdPartyRepair;

	dateVar = m_date.string();
	return;
}
void GasStop::getGasStopInfo(int& mileage, double& gal, double& ppg, std::string& notesVar, std::string& dateVar) const{
	mileage = m_mileageDone;
	gal = m_gallons;
	ppg = m_pricePerGallon;
	notesVar = m_notes;
	
	dateVar = m_date.string();
	return;
}

Vehicle::Vehicle(const std::string setName, uint32_t setMileage)
{
	if (setName.length() <= 0 || setName.length() > maxVehicleNameSize) {
		std::ostringstream logText;
		logText << "Vehicle name out of range. Max allowed is: " << maxVehicleNameSize << ". Current: " << setName.length();
		Log(LogCode::WARNING, logText.str());
		m_name = "Vehicle";
	}
	else {
		m_name = setName;
	}

	if (setMileage < 0) {
		Log(LogCode::WARNING, "Vehicle mileage out of range for " + m_name);
		m_mileage = 0;
	}
	else {
		m_mileage = setMileage;
	}
}
bool Vehicle::NewRepair(uint32_t setMiles, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, Date setDate) {
	bool milesAccepted{ false }, costAccepted{ false }, notesAccepted{ false };
	if (setMiles > 0) {
		milesAccepted = true;
	}

	if (setCost > 0) {
		costAccepted = true;
	}

	if (setNotes.length() <= 0 || setNotes.length() > maxNotesSize) {
		std::ostringstream logText;
		logText << "Repair notes out of range. Max size allowed is " << maxNotesSize << ". Current: " << setNotes.length();
		Log(LogCode::WARNING, logText.str());
		notesAccepted = false;
	}
	else {
		notesAccepted = true;
	}

	if (milesAccepted && costAccepted && notesAccepted) {
		Repair newRepair{ setMiles, setType, setCost, setNotes, setThirdParty, setDate };
		repairList.push_back(newRepair);

		//If the New Repair's mileage is higher than the last element in the vector, no need to sort.
		if (repairList.size() == 1 || newRepair.getMileage() > repairList[repairList.size() - 2].getMileage())
		{
			// Do nothing
		}
		else { //sort by mileage
			for (int i{ 0 }; i < repairList.size() - 1; ++i) {
				for (int x{ i + 1 }; x < repairList.size(); ++x) {
					if (repairList[x].getMileage() < repairList[i].getMileage()) {
						std::swap(repairList[i], repairList[x]);
					}
				}
			}
		}

		//If the mileage is higher than the vehicle, set the vehicle's mileage
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
	bool milesAccepted{ false }, galAccepted{ false }, ppgAccepted{ false }, notesAccepted{ false };
	if (setMiles > 0) {
		milesAccepted = true;
	}

	if (setGal > 0) {
		galAccepted = true;
	}

	if (setPPG > 0) {
		ppgAccepted = true;
	}

	if (setNotes.length() <= 0 || setNotes.length() > maxNotesSize) {
		std::ostringstream logText;
		logText << "Gas Stop notes out of range. Max size allowed is " << maxNotesSize << ". Current: " << setNotes.length();
		Log(LogCode::WARNING, logText.str());
	}
	else {
		notesAccepted = true;
	}

	if (milesAccepted && galAccepted && ppgAccepted && notesAccepted) {
		GasStop newGasStop{ setMiles, setGal, setPPG, setNotes, setDate };
		gasList.push_back(newGasStop);

		//If the New GasStop's mileage is higher than the last element in the vector, no need to sort.
		if (gasList.size() == 1 || newGasStop.getMileage() > gasList[gasList.size() - 2].getMileage())
		{
			//Do nothing
		}
		else { //sort by mileage
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

std::ostream& operator<<(std::ostream& os, const Repair& repair) {
	os << repair.m_mileageDone << '|' << repair.m_type << '|' << repair.m_cost << '|' << repair.m_isThirdPartyRepair << '|' << repair.m_notes << '|' << repair.m_date;

	return os;
}
std::ostream& operator<<(std::ostream& os, const GasStop& gasStop) {
	os << gasStop.m_mileageDone << '|' << gasStop.m_pricePerGallon << '|' << gasStop.m_gallons << '|' << gasStop.m_notes << '|' << gasStop.m_date;

	return os;
}
std::ostream& operator<<(std::ostream& os, const RepairType& type){
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
std::ostream& operator<<(std::ostream& os, const Date& date){
	os << date.day << ' ' << date.month << ' ' << date.year;

	return os;
}
bool operator==(const Vehicle& lhs, const Vehicle& rhs){
	if(lhs.getName() == rhs.getName()){
		return true;
	}
	else{
		return false;
	}
}