#pragma once
#include <string>
#include <vector>
#include <sstream>

#include "Log.h"

enum class RepairType{
	OIL_CHANGE = 1,
	TRANSMISSION_FLUID_EXCHANGE,
	LIGHTBULB_REPLACEMENT,
	POWER_STEERING_FLUID_EXCHANGE,
	WIPER_REPLACEMENT,
	TIRE_ROTATION,
	TIRE_REPLACEMENT,
	BODYWORK,
	MECHANICAL,
	BATTERY_REPLACEMENT,

	OTHER //Other is always last
};
std::ostream& operator<<(std::ostream& os, const RepairType& type);

struct Date{
	Date(unsigned short setDay, unsigned short setMonth, int setYear)
	: 	day{setDay},
		month{setMonth},
		year{setYear}
	{

	}
	unsigned short day, month;
	int year;

	std::string string() const;
	friend std::ostream& operator<<(std::ostream& os, const Date& date);
};

class Repair {
public:
	// A Creation of a Repair
	// \param setMileage Mileage the repair was done at \param setType The RepairType enum of the specific repair
	// \param setNotes Any notes to add to the repair \param setThirdParty Was the repair done by another person (3rd Party) or done by the owner (1st Person)
	// \param setDate The date the repair was done
	Repair(uint32_t setMileage, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, Date setDate)
		: m_mileageDone{ setMileage },
		m_type{ setType },
		m_cost{ setCost },
		m_notes{ setNotes },
		m_isThirdPartyRepair{ setThirdParty },
		m_date{setDate}
	{

	};
	~Repair() {

	}

	//Overwrites the parameters with the info of the repair
	void getRepairInfo(int& mileage, std::string& typeStr, double& costDbl, std::string& notesVar, bool& wasThirdParty, std::string& dateVar) const;
	uint32_t getMileage() const{ return m_mileageDone; }
	friend std::ostream& operator<<(std::ostream& os, const Repair& repair);

private:
	uint32_t 	m_mileageDone;
	RepairType	m_type;
	std::string m_notes;
	double 		m_cost;
	bool 		m_isThirdPartyRepair;
	Date 		m_date;

};

class GasStop {
public:
	// Create a Gas Stop
	// \param setMileage The mileage the gas stop was done \param setGallons Amount of gallons filled up with 
	// \param setNotes Notes to add about the Gas Stop \param setDate Date of the Gas Stop
	GasStop(uint32_t setMileage, double setGallons, double setPricePerGallon, std::string setNotes, Date setDate)
		: m_mileageDone{ setMileage },
		m_gallons{setGallons},
		m_pricePerGallon{setPricePerGallon},
		m_notes{setNotes},
		m_date{setDate}
	{

	}
	~GasStop() {

	}
	//Overwrites the parameters with the info of the gas stop
	void 		getGasStopInfo(int& mileage, double& gal, double& ppg, std::string& notesVar, std::string& dateVar) const;
	uint32_t 	getMileage() const { return m_mileageDone; }
	friend std::ostream& operator<<(std::ostream& os, const GasStop& gasStop);

private:
	uint32_t	m_mileageDone;
	double		m_gallons;
	double		m_pricePerGallon;
	std::string m_notes;
	Date 		m_date;
};

class Vehicle {
public:
	// Create a Vehicle
	// \param setName Vehicle Name
	// \param setMileage Vehicle's initial mileage
	Vehicle(const std::string setName = "Vehicle", uint32_t setMileage = 0);
	~Vehicle() 
	{

	}

	static constexpr int maxVehicleNameSize { 15 };
	static constexpr int maxNotesSize		{ 45 };

	uint32_t getMileage() const { return m_mileage; }
	std::string getName() const { return m_name; };

	//Given individual types, it will error check ranges and data before adding to the vehicle's list
	bool NewRepair(uint32_t setMiles, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, Date setDate);
	//Given individual types, it will error check ranges and data before adding to the vehicle's list
	bool NewGasStop(uint32_t setMiles, double setGal, double setPPG, std::string setNotes, Date setDate);

	std::vector<Repair>& getRepairList()	{ return repairList; }
	std::vector<GasStop>& getGasStopList()  { return gasList; }

	friend bool operator==(const Vehicle& lhs, const Vehicle& rhs);

private:
	std::string m_name;
	uint32_t	m_mileage;

	std::vector<Repair>		repairList{};
	std::vector<GasStop>	gasList{};
};

