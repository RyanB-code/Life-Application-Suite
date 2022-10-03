#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "Log.h"

// \param Mileage, Type Of Repair, Cost, Notes, Did Third Party Perform it?
class Repair {
public:
	Repair(uint32_t setMileage, std::string setType, double setCost, std::string setNotes, bool setThirdParty)
		: mileageDone{ setMileage },
		type{ setType },
		cost{ setCost },
		notes{ setNotes },
		isThirdPartyRepair{ setThirdParty }
	{

	};
	~Repair() {

	}

	//Overwrites the parameters with the info of the repair
	const void getRepairInfo(int& mileage, std::string& typeStr, double& costDbl, std::string& notesVar, bool& wasThirdParty);
	const uint32_t getMileage() { return mileageDone; }
	friend std::ostream& operator<<(std::ostream& os, const Repair& repair);

private:
	uint32_t mileageDone;
	std::string type;
	std::string notes;
	double cost;
	bool isThirdPartyRepair;
};

// \param Mileage, Gallons Filled, Price Per Gallon, Notes,
class GasStop {
public:
	GasStop(uint32_t setMileage, double setGallons, double setPricePerGallon, std::string setNotes)
		: mileageDone{ setMileage },
		gallons{setGallons},
		pricePerGallon{setPricePerGallon},
		notes{setNotes}
	{

	}
	~GasStop() {

	}
	//Overwrites the parameters with the info of the gas stop
	const void getGasStopInfo(int& mileage, double& gal, double& ppg, std::string& notesVar);
	const uint32_t getMileage() { return mileageDone; }
	friend std::ostream& operator<<(std::ostream& os, const GasStop& gasStop);

private:
	uint32_t	mileageDone;
	double		gallons;
	double		pricePerGallon;
	std::string notes;
};

// \param Name, Mileage
class Vehicle {
public:
	Vehicle(const std::string setName = "Vehicle", uint32_t setMileage = 0)
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
	~Vehicle() 
	{

	}

	static constexpr int maxVehicleNameSize { 15 };

	static constexpr int maxRepairTypeSize	{ 15 };
	static constexpr int maxNotesSize		{ 30 };

	const uint32_t getMileage() { return m_mileage; }
	const std::string getName() { return m_name; };

	//Given individual types, it will error check ranges and data before adding to the vehicle's list
	bool NewRepair(uint32_t setMiles, std::string setType, double setCost, std::string setNotes, bool setThirdParty);
	//Given individual types, it will error check ranges and data before adding to the vehicle's list
	bool NewGasStop(uint32_t setMiles, double setGal, double setPPG, std::string setNotes);

	std::vector<Repair>& getRepairList()	{ return repairList; }
	std::vector<GasStop>& getGasStopList()	{ return gasList; }

private:
	std::string m_name;
	uint32_t	m_mileage;

	std::vector<Repair>		repairList{};
	std::vector<GasStop>	gasList{};
};

