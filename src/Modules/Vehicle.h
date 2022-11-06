#pragma once

#include "../FileSystem.h"
#include "../Log.h"
#include "../Application.h"
#include "../DateTime.h"


#include <string>
#include <vector>
#include <sstream>

#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_mods.h>

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

	//Specifically adds the vehicle to the master vehicleList
	void 					addToVehicleList() 			{ s_vehicleList.push_back(*this); } 
	std::vector<Vehicle> 	getVehicleList() 	const 	{ return s_vehicleList; }
	void					delFromVehList() 			{ std::erase(s_vehicleList, *this); }
	static bool				vehListIsEmpty()			{ return s_vehicleList.empty(); }

	// Saves vehicles by writing to file. The path is specified in the Application class \return True if could save to file. False if could not.
	friend bool saveVehicles(const Application& app);
	// Display selectable list of vehicles
	friend Vehicle* SelectableVehicleList();

	uint32_t 	getMileage() 	const { return m_mileage; }
	std::string getName() 		const { return m_name; };

	//Given individual types, it will error check ranges and data before adding to the vehicle's list
	bool NewRepair(uint32_t setMiles, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, Date setDate);
	//Given individual types, it will error check ranges and data before adding to the vehicle's list
	bool NewGasStop(uint32_t setMiles, double setGal, double setPPG, std::string setNotes, Date setDate);

	std::vector<Repair>& 	getRepairList()	 { return repairList; }
	std::vector<GasStop>& 	getGasStopList() { return gasList; }

	friend bool operator==(const Vehicle& lhs, const Vehicle& rhs);		// Compare if Vehicle names are the same

private:
	std::string m_name;
	uint32_t	m_mileage;

	std::vector<Repair>		repairList{};
	std::vector<GasStop>	gasList{};

	static std::vector<Vehicle> s_vehicleList;

};


// File Read/Write------------------------------

// Reads name\param Text string from file, deletes read characters
std::string makeVehicleName(std::string& text);
// Reads mileage \param Text string from file, deletes read characters
uint32_t makeVehicleMiles(std::string& text);
// Make a Repair from a text stream. \param repairList: Adds found repairs to this vehicle
void makeRepair(std::string& text, Vehicle& veh);
// Make a GasStop from a text stream. \param gasList : Adds found gas stops to this vehicle
void makeGasStop(std::string& text, Vehicle& veh);
// Writes Vehicle information to a file with the Vehicle's name
bool writeToFile(const Application& app, Vehicle& veh);


void NewVehicle(Application* app, Vehicle& veh);							// Add vehicle to the list of known vehicles
bool DeleteVehicle(Application* app, Vehicle& veh);							// Remove vehicle from the list, and file directory
//---------------------------------------------------------


// ImGui Implementation below
void VehicleManager(Application* app, bool &shown);
void SetupVehicleManager(Application* app);
//Shows just one vehicle
void ShowFullVehicleInformation(Vehicle* veh);
void EditVehicle(Vehicle* veh);


/* 	bool AddVehicle(Application* app) {
		bool returnValue{false};
		
		std::string nameBuf;
		uint32_t mileBuf;

		std::cout << "Name\n>";
		std::cin.ignore(10000, '\n');
		std::getline(std::cin, nameBuf);

		bool exit{ false };
		do {
			std::cout << "Mileage >";
			std::cin >> mileBuf;
			clearLineAfterInput();

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				exit = true;
			}

		} while (!exit);

		std::cout << "Name:\t\t" << nameBuf << std::endl;
		std::cout << "Mileage:\t" << mileBuf << std::endl;

		std::cout << "\nSave Vehicle?\n";
		if (getBoolInput()) {
			Vehicle newVehicle{ nameBuf, mileBuf };
			app->NewVehicle(newVehicle);

			returnValue = true;
		}
		else {
			returnValue = false;
		}

		return returnValue;
	}
*/
/*	bool AddRepair(Vehicle* veh) {
		bool success{false};

		uint32_t mileBuf{ 0 };
		std::string typeBuf;
		double costBuf{ 0.0 };
		std::string notesBuf;
		bool thirdPartyBuf;

		bool exit0{ false };
		do {
			bool exit1{ false };
			do {
				std::cout << "What mileage was the repair done at?\n>";
				std::cin >> mileBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit1 = true;
				}

			} while (!exit1);

			std::cout << "The Type of repair. [Max characters " << veh->maxRepairTypeSize << "]\nExamples: 'Oil Change', 'Power Steering', 'Body Work', etc...\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, typeBuf);


			bool exit2{ false };
			do {
				std::cout << "How much did it cost in total?\n>";
				std::cin >> costBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit2 = true;
				}

			} while (!exit2);

			std::cout << "Enter any notes here. [Max characters " << veh->maxNotesSize << "]\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, notesBuf);

		
			std::cout << "Did a third party perform the repair?" << std::endl;
			thirdPartyBuf = getBoolInput();

			std::cout << "Miles:\t\t" << mileBuf << std::endl;
			std::cout << "Type:\t\t" << typeBuf << std::endl;
			std::cout << "Cost:\t\t" << costBuf << std::endl;
			std::cout << "Notes:\t\t" << notesBuf << std::endl;
			std::cout << "3rd Party:\t" << std::boolalpha << thirdPartyBuf << std::endl;

			std::cout << "\nSave Repair?\n";
			if (getBoolInput()) {
				if (!veh->NewRepair(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf)) {
					std::cout << "\nCould not add repair to vehicle. \nTry Again?\n";
					if (!getBoolInput()) {
						exit0 = true;
					}
				}
				else {
					success = true;
					exit0 = true;
				}
			}
			else {
				exit0 = true;
			}
		} while (!exit0);

		return success;

	}
*/
/*  bool AddGasStop(Vehicle* veh) {
		bool success{false};

		uint32_t mileBuf{ 0 };
		double galBuf{ 0.0 };
		double ppgBuf{ 0.0 };
		std::string notesBuf;

		bool exit0{ false };
		do {
			bool exit1{ false };
			do {
				std::cout << "What mileage did you fill up at?\n>";
				std::cin >> mileBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit1 = true;
				}

			} while (!exit1);

			bool exit2{ false };
			do {
				std::cout << "How many gallons did you fill up?\n>";
				std::cin >> galBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit2 = true;
				}

			} while (!exit2);

			bool exit3{ false };
			do {
				std::cout << "What was the price per gallon?\n>";
				std::cin >> ppgBuf;
				clearLineAfterInput();
				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					exit3 = true;
				}

			} while (!exit3);

			std::cout << "Enter any notes here. [Max characters " << veh->maxNotesSize << "]\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, notesBuf);

			std::cout << std::setw(17) << std::right << "Miles:\t" << mileBuf << std::endl;
			std::cout << std::setw(17) << std::right << "Gallons:\t" << galBuf << std::endl;
			std::cout << std::setw(17) << std::right << "Price Per Gallon:\t" << ppgBuf << std::endl;
			std::cout << std::setw(17) << std::right << "Notes:\t" << notesBuf << std::endl;

			std::cout << "\nSave Gas Stop?\n";
			if (getBoolInput()) {
				if (!veh->NewGasStop(mileBuf, galBuf, ppgBuf, notesBuf)) {
					std::cout << "\nCould not add gas stop to vehicle. \nTry Again?\n";
					if (!getBoolInput()) {
						exit0 = true;
					}
				}
				else {
					success = true;
					exit0 = true;
				}
			}
			else {
				exit0 = true;
			}
		} while (!exit0);

		return success;
	}
*/
