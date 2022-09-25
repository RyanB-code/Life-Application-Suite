#pragma once
#include <string>
#include <vector>
#include <sstream>

// \param Mileage, Type Of Repair, Cost, Notes, bool Did Third Party Perform it?
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
	GasStop(uint32_t setMileage, short setGallons, double setPricePerGallon, std::string setNotes)
		: mileageDone{ setMileage },
		gallons{setGallons},
		pricePerGallon{setPricePerGallon},
		notes{setNotes}
	{

	}

	const uint32_t getMileage() { return mileageDone; }
	friend std::ostream& operator<<(std::ostream& os, const GasStop& gasStop);

private:
	uint32_t mileageDone;
	short gallons;
	double pricePerGallon;
	std::string notes;
};

// \param Name, Mileage
class Vehicle {
public:
	Vehicle(const std::string setName = "Vehicle", uint32_t setMileage = 1) : m_name{ setName }, m_mileage{ setMileage }
	{

	}
	~Vehicle() 
	{

	}

	static constexpr int maxVehicleNameSize {15};

	const uint32_t getMileage() { return m_mileage; }
	const std::string getName() { return m_name; };

	void NewRepair(Repair& newRepair);
	void NewGasStop(GasStop& newGasStop);

	std::vector<Repair>& getRepairList() { return repairList; }
	std::vector<GasStop>& getGasStopList() { return gasList; }

private:
	std::string m_name;
	uint32_t m_mileage;

	std::vector<Repair> repairList{};
	std::vector<GasStop> gasList{};
};

