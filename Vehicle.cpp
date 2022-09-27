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

void Vehicle::NewRepair(Repair& newRepair) {
	repairList.push_back(newRepair);	

	//If the New Repair's mileage is higher than the last element in the vector, no need to sort.
	if (repairList.size() == 1 || newRepair.getMileage() > repairList[repairList.size() - 2].getMileage())
	{
		// Do nothing
	}
	else { //sort by mileage
		for (int i{ 0 }; i < repairList.size()-1; ++i) {
			for (int x{ i+1 }; x < repairList.size(); ++x) {
				if (repairList[x].getMileage() < repairList[i].getMileage()) {
					std::swap(repairList[i], repairList[x]);
				}
			}
		}
	}

	//If the mileage is higher than the vehicle, set the vehicle's mileage
	if (repairList.back().getMileage() > m_mileage)
		m_mileage = repairList.back().getMileage();
}
void Vehicle::NewGasStop(GasStop& newGasStop) {
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
	if (gasList.back().getMileage() > m_mileage)
		m_mileage = gasList.back().getMileage();
}


std::ostream& operator<<(std::ostream& os, const Repair& repair) {
	os << repair.mileageDone << '|' << repair.type << '|' << repair.cost << '|' << repair.isThirdPartyRepair << '|' << repair.notes;

	return os;
}
std::ostream& operator<<(std::ostream & os, const GasStop & gasStop) {
	os << gasStop.mileageDone << '|' << gasStop.pricePerGallon << '|' << gasStop.gallons << '|' << gasStop.notes;

	return os;
}

