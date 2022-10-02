#include "Vehicle.h"

const void Repair::getRepairInfo(int& mileage, std::string& typeStr, double& costDbl, std::string& notesVar, bool& wasThirdParty) {
	mileage = mileageDone;
	typeStr = type;
	costDbl = cost;
	notesVar = notes;
	wasThirdParty = isThirdPartyRepair;
	return;
}
const void GasStop::getGasStopInfo(int& mileage, short& gal, double& ppg, std::string& notesVar) {
	mileage = mileageDone;
	gal = gallons;
	ppg = pricePerGallon;
	notesVar = notes;
	return;
}


bool Vehicle::NewRepair(uint32_t setMiles, std::string setType, double setCost, std::string setNotes, bool setThirdParty) {
	bool milesAccepted{ false }, typeAccepted{ false }, costAccepted{ false }, notesAccepted{ false };
	if (setMiles > 0) {
		milesAccepted = true;
	}

	if (setType.length() <= 0 || setType.length() > maxRepairTypeSize) {
		typeAccepted = false;
		std::ostringstream logText;
		logText << "Repair type out of range. Max size allowed is " << maxRepairTypeSize << ". Current: " << setType.length();
		Log(LogCode::WARNING, logText.str());
	}
	else {
		typeAccepted = true;
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

	if (milesAccepted && typeAccepted && costAccepted && notesAccepted) {
		Repair newRepair{ setMiles, setType, setCost, setNotes, setThirdParty };
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
bool Vehicle::NewGasStop(uint32_t setMiles, short setGal, double setPPG, std::string setNotes) {
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
		GasStop newGasStop{ setMiles, setGal, setPPG, setNotes };
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
	os << repair.mileageDone << '|' << repair.type << '|' << repair.cost << '|' << repair.isThirdPartyRepair << '|' << repair.notes;

	return os;
}
std::ostream& operator<<(std::ostream & os, const GasStop & gasStop) {
	os << gasStop.mileageDone << '|' << gasStop.pricePerGallon << '|' << gasStop.gallons << '|' << gasStop.notes;

	return os;
}

