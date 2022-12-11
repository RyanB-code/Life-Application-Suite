#ifndef VEHICLE_H
#define VEHICLE_H

#include "../FileSystem.h"
#include "../DateTime.h"

#include "Module.h"

#include <string>
#include <vector>
#include <sstream>
#include <chrono>

#include <DearImGUI/imgui.h>
#include <RST/RST.h>




// Classification for the kind of Repair
enum class RepairType : int{

	// If adding additonal RepairTypes, be sure to change the operator<< override AND the VehicleManager's drop down menu to show the option

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

	OTHER // Other will always last
};
std::ostream& operator<<(std::ostream& os, const RepairType& type); 		// Used to output the RepairType in string format

// Tracks useful information when a Vehicle is repaired. Should be assigned to a Vehicle after initialization
class Repair {
public:
	// Initialize a Repair
	// \param setMileage Mileage the repair was done at \param setType The RepairType enum of the specific repair
	// \param setNotes Any notes to add to the repair \param setThirdParty Was the repair done by another person (3rd Party) or done by the owner (1st Person)
	// \param setDate The date the repair was done
	Repair(uint32_t setMileage=0, RepairType setType=RepairType::OTHER, double setCost=0.00, std::string setNotes="", bool setThirdParty=false, Date setDate={1, 1, 1900})
		: m_mileageDone{ setMileage },
		m_type{ setType },
		m_cost{ setCost },
		m_notes{ setNotes },
		m_isThirdPartyRepair{ setThirdParty },
		m_date{setDate}
	{ }
	~Repair() { }

	// Overwrites the parameters with the info of the repair
	void 		getRepairInfo(int& mileage, std::string& typeStr, double& costDbl, std::string& notesVar, bool& wasThirdParty, std::string& dateVar) const;
	uint32_t 	getMileage() 	const	{ return m_mileageDone; }

	 // Puts a Repair into a readable format
	friend std::ostream& operator<<(std::ostream& os, const Repair& repair);

private:
	uint32_t 	m_mileageDone;
	RepairType	m_type;
	std::string m_notes;
	double 		m_cost;
	bool 		m_isThirdPartyRepair;
	Date 		m_date;

};

// Tracks useful information when a Vehicle gets gas. Should be assigned to a Vehicle after initialization
class GasStop {
public:
	// Create a Gas Stop
	// \param setMileage The mileage the gas stop was done \param setGallons Amount of gallons filled up with 
	// \param setNotes Notes to add about the Gas Stop \param setDate Date of the Gas Stop
	GasStop(uint32_t setMileage=0, double setGallons=0.0, double setPricePerGallon=0.0, std::string setNotes="", Date setDate={1, 1, 1900})
		: m_mileageDone{ setMileage },
		m_gallons{setGallons},
		m_pricePerGallon{setPricePerGallon},
		m_notes{setNotes},
		m_date{setDate}
	{ }

	~GasStop() { }

	//Overwrites the parameters with the info of the gas stop
	void 		getGasStopInfo(int& mileage, double& gal, double& ppg, std::string& notesVar, std::string& dateVar) const;
	uint32_t 	getMileage() const { return m_mileageDone; }

	 // Puts a Gas Stop into a readable format
	friend std::ostream& operator<<(std::ostream& os, const GasStop& gasStop);

private:
	uint32_t	m_mileageDone;
	double		m_gallons;
	double		m_pricePerGallon;
	std::string m_notes;
	Date 		m_date;
};

// Track useful information about a Vehicle. Stores lists of Repairs and Gas Stops as well as mileage of the Vehicle
class Vehicle {
public:
	// Create a Vehicle
	Vehicle(const std::string setName = "Vehicle", uint32_t mileage = 0);
	~Vehicle() { }

	static constexpr int maxVehicleNameSize { 15 };
	static constexpr int maxNotesSize		{ 45 };

	uint32_t 				getMileage() 		const 	{ return m_mileage; }
	std::string 			getName() 			const 	{ return m_name; }
	Date					getLastUpdated()	const 	{ return m_lastUpdated; }

	bool 					setName						(const std::string setName);
	bool 					setMileage					(const uint32_t setMileage);
	bool					setLasUpdated				(const Date date);

	std::vector<Repair>& 	getRepairList()	 			{ return repairList; } 
	std::vector<GasStop>& 	getGasStopList() 			{ return gasList; }
	

	//Given individual types, it will error check ranges and data before adding to the vehicle's list of Repairs
	bool NewRepair	(uint32_t setMiles, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, Date date);
	//Given individual types, it will error check ranges and data before adding to the vehicle's list of Gas Stops
	bool NewGasStop	(uint32_t setMiles, double setGal, double setPPG, std::string setNotes, Date date);

	// Compare if Vehicle names are the same
	friend bool operator==(const Vehicle& lhs, const Vehicle& rhs);	

private:
	std::string m_name;
	uint32_t	m_mileage;
	Date 		m_lastUpdated{0,0,0};

	std::vector<Repair>		repairList{};
	std::vector<GasStop>	gasList{};

};

class VehicleManager : public Module{
public:
    VehicleManager(Application* app);
    ~VehicleManager() override;

    void    Display() override;
    bool    Setup()   override;

	// Functions for interacting with s_vehicleList
	void 					addToVehicleList(Vehicle& veh) 		{ s_vehicleList.push_back(veh); 	RST::Log( "Added [" + veh.getName() +"] to vehicle list", LogCode::LOG_HIGH); } 	// Used to add A vehicle to the master list
	void					delFromVehList	(Vehicle& veh) 		{ std::erase(s_vehicleList, veh); 	RST::Log( "Removed [" + veh.getName() +"] to from vehicle list", LogCode::LOG_HIGH);}	// Compares the Vehicle's m_name and deletes all Vehicles with the same m_name
	static bool				vehListIsEmpty()					{ return s_vehicleList.empty(); }		// Check if the master list is empty
	std::vector<Vehicle>& 	getVehicleList() 	const 			{ return s_vehicleList; }

private:
	static std::vector<Vehicle> s_vehicleList;

	Vehicle* 	SelectableVehicleList();						// Using ImGui, shows a list table of Vehicle's that are selectable
	void 		ShowFullVehicleInformation	(Vehicle* veh);		// Shows all the parameter Vehicle's Repairs and GasStops in table format


	bool SaveAllVehicles	();					// Saves vehicles by writing to file. The path is specified in the Application class 
	bool SaveVehicle		(Vehicle* veh);		// Writes Vehicle information to a file with the Vehicle's name
	bool DeleteVehicle		(Vehicle& veh);								// Remove vehicle from the list, and file directory

};


// Parsing raw text functions that format the text ----------------------------------

std::string MakeVehicleName		(std::string& text);					// Deletes read characters from parameter and returns with string of the text read
uint32_t 	MakeVehicleMiles	(std::string& text);					// Deletes read character from the parameter and returns the int read
void 		MakeRepair			(std::string& text, Vehicle& veh);		// Make a Repair from a text stream. Adds it to veh parameter
void 		MakeGasStop			(std::string& text, Vehicle& veh);		// Make a GasStop from a text stream. Adds it to the veh parameter
bool 		CheckStringSize		(const std::string text, int maxAllowed);
// -----------------------------------------------------------------------------------


// ImGui Functions that just have to do with a vehicle

bool	AddGasStop(Vehicle* veh, bool& wasSaved);
bool	AddRepair(Vehicle* veh, bool& wasSaved);


// ------------------------------------------------------------------------------------
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


#endif