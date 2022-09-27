#include "Display.h"

#pragma warning(disable : 4996)

namespace Display {

	void clear()
	{ 
		system("cls"); 
	}

	//===========Menus/Screens=====================
	void Home(Application* app)
	{
		Log(LogCode::LOG, "Home Screen called");
		bool exit{ false };
		while (!exit) {
			DisplayBanner("Life Application Suite");
			std::cout << "\nOptions: \n";
			std::cout << "1. Vehicle Manager\n";
			std::cout << "2. Settings\n";
			std::cout << "3. Exit\n";

			std::cout << ">";

			unsigned short input;
			std::cin >> input;

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				switch (input) {
				case 1:
					VehicleHome(app);
					break;
				case 2:
					Settings(app);
					break;
				case 3:
					std::cout << "\nExiting...\n";
					exit = true;
					break;
				default:
					std::cout << "\nInvalid number. Restarting...\n";
					Sleep(2000);
					break;
				} //end switch
			}

		} // end while loop
		
	}
	void VehicleHome(Application* app) {
		Log(LogCode::LOG, "Vehicle Home Screen called");

		Vehicle* selectedVehicle{ nullptr };

		bool exit{ false };
		while (!exit) {
			DisplayBanner("Vehicles -> Home");

			std::cout << "Vehicles:\n";
			std::cout << ListVehicles(app->getVehicleList()).str();

			if (!selectedVehicle) {
				std::cout << "\nOptions: \n";
				std::cout << "1. Select Vehicle\n";
				std::cout << "2. View All Vehicle Info\n";
				std::cout << "3. Go back\n";
				std::cout << ">";

				unsigned short input;
				std::cin >> input;

				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					switch (input) {
					case 1:
						selectedVehicle = SelectVehicle(app->getVehicleList());
						break;
					case 2:
						ShowFullVehicleInformation(app->getVehicleList());
						break;
					case 3:
						exit = true;
						break;
					default:
						std::cout << "\nInvalid number. Restarting...\n";
						Sleep(2000);
						break;
					} //end switch
				}
			}
			else {
				std::cout << "\nSelected Vehicle > " << selectedVehicle->getName() << "\n";
				std::cout << "\nOptions: \n";
				std::cout << "1. Select Another Vehicle\n";
				std::cout << "2. Deselect Vehicle\n";
				std::cout << "3. View Selected Vehicle Info\n";
				std::cout << "4. Edit\n";
				std::cout << "5. Go back\n";
				std::cout << ">";

				unsigned short input;
				std::cin >> input;

				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					switch (input) {
					case 1:
						selectedVehicle = SelectVehicle(app->getVehicleList());
						break;
					case 2:
						selectedVehicle = nullptr;
						break;
					case 3:
						DisplayBanner("Vehicle -> View");
						std::cout << ShowFullVehicleInformation(selectedVehicle).str();
						system("pause");
						break;
					case 4:
						EditVehicle(selectedVehicle);
						break;
					case 5:
						exit = true;
						break;
					default:
						std::cout << "\nInvalid number. Restarting...\n";
						Sleep(2000);
						break;
					} //end switch

				}
			}
		} //end while loop
	}
	void Settings(Application* app) {
		DisplayBanner("LAS -> Settings");
		std::cout << "Main Directory:\t\t" << app->showMainDirectory() << "\n";
		std::cout << "Debug Directory:\t" << app->showDebugDirectory() << "\n";
		std::cout << "Debug File:\t\t" << app->showLogFilePath() << "\n";
		std::cout << "Vehicle Directory:\t" << app->showVehicleDirectory() << "\n";
		std::cout << "\n";
		system("pause");
	}

	void ShowFullVehicleInformation(std::vector<Vehicle>& vehList) {
		DisplayBanner("Vehicles -> Detailed View");
		std::cout << ListVehicles(vehList, true).str();
		system("pause");
		return;
	}

	std::ostringstream ShowFullVehicleInformation(Vehicle* veh) {
		std::ostringstream os;

		if (!veh) {
			Log(LogCode::WARNING, "Could not show full vehicle information. Null pointer.");
		}
		else {
			short mileageWidth{ 8 };
			short typeWidth{ 15 };
			short costWidth{ 8 };
			short thirdPartyWidth{ 8 };
			short notesWidth{ 30 };
			int tableLineRepairs{ mileageWidth + typeWidth + costWidth + thirdPartyWidth + notesWidth + 12 }; //last number is number of separators

			short gallonsWidth{ 8 };
			int tableLineGas{ mileageWidth + gallonsWidth + costWidth + notesWidth + 9 };

			os << std::setw(veh->maxVehicleNameSize + 1) << std::left << veh->getName() << std::setw(6) << std::right << veh->getMileage() << " miles\n";

			os << std::setw((tableLineRepairs / 2) + 7) << std::right << "===REPAIRS===" << std::left << '\n'; //make repairs be in the middle
			os << "   " << std::setw(mileageWidth) << "Miles"; os << " | ";
			os << std::setw(typeWidth) << "Type"; os << " | ";
			os << std::setw(costWidth) << "Cost"; os << " | ";
			os << std::setw(thirdPartyWidth) << "By Me?"; os << " | ";
			os << std::setw(notesWidth) << "Notes";
			os << std::setw(tableLineRepairs) << std::setfill('=');
			os << "\n   " << std::setfill(' ');

			for (Repair& rep : veh->getRepairList()) {
				int				mileBuf;
				std::string		typeBuf;
				double			costBuf;
				std::string		notesBuf;
				bool			thirdPartyBuf;
				rep.getRepairInfo(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf);
				os << "\n   " << std::setw(mileageWidth) << std::right << mileBuf; os << " | ";
				os << std::setw(typeWidth) << typeBuf; os << " | ";
				os << std::setw(costWidth) << costBuf; os << " | ";
				os << std::setw(thirdPartyWidth) << std::boolalpha << thirdPartyBuf; os << " | ";
				os << std::setw(notesWidth) << std::left << notesBuf;
			}
			os << "\n\n";

			os << std::setw((tableLineGas / 2) + 8) << std::right << "===GAS STOPS===" << std::left << '\n'; //make text be in the middle
			os << "   " << std::setw(mileageWidth) << "Miles"; os << " | ";
			os << std::setw(gallonsWidth) << "Gallons"; os << " | ";
			os << std::setw(costWidth) << "PPG"; os << " | ";
			os << std::setw(notesWidth) << "Notes";
			os << std::setw(tableLineGas) << std::setfill('=');
			os << "\n   " << std::setfill(' ');


			for (GasStop& gas : veh->getGasStopList()) {
				int			mileBuf;
				short		galBuf;
				double		costBuf;
				std::string notesBuf;
				gas.getGasStopInfo(mileBuf, galBuf, costBuf, notesBuf);
				os << "\n   " << std::setw(mileageWidth) << std::right << mileBuf; os << " | ";
				os << std::setw(gallonsWidth) << galBuf; os << " | ";
				os << std::setw(costWidth) << costBuf; os << " | ";
				os << std::setw(notesWidth) << std::left << notesBuf;
			}

			os << "\n\n";
		}

		return os;
	}
	void EditVehicle(Vehicle* veh) {

		bool exit{ false };
		while (!exit) {
			DisplayBanner("Vehicle -> Edit");
			std::cout << ShowFullVehicleInformation(veh).str();

			std::cout << "\nOptions:\n";
			std::cout << "1. Add a new repair or gas stop\n";
			std::cout << "2. Edit an existing repair or gas stop\n";
			std::cout << "3. Go back\n";
			std::cout << ">";
			unsigned short input;
			std::cin >> input;

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				switch (input) {
				case 1:
					std::cout << "\nIn new repair/gas stop\n";
					Sleep(2000);
					break;
				case 2:
					std::cout << "\nIn edit repair/gas stop\n";
					Sleep(2000);
					break;
				case 3:
					exit = true;
					break;
				default:
					std::cout << "\nInvalid number. Restarting...\n";
					Sleep(2000);
					break;
				} //end switch
			}
		}

	}
	Vehicle* SelectVehicle(std::vector<Vehicle>& vehList) {
		if (vehList.empty()) {
			return nullptr;
		}
		else {
			Vehicle* selVeh{ nullptr };

			bool exit{ false };
			while (!exit) {
				clear();
				DisplayBanner("Vehicles -> Select Vehicle");
				std::cout << ListVehicles(vehList).str();

				std::cout << ">";

				unsigned short input;
				std::cin >> input;

				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(10000, '\n');
				}
				else {
					if (input - 1 < vehList.size()) {
						selVeh = &vehList[input - 1];
						exit = true;
					}
					else {

					}
				}
			}

			return selVeh;
		}

	}
	//============END MENU/SCREENS==================

	const std::ostringstream ListVehicles(std::vector<Vehicle>& vehList, bool detailed) {
		std::ostringstream os;
		if (vehList.empty()) {
			os << "No vehicles found.\n";
		}
		else
		{
			int i{ 1 };

			for (Vehicle& currentVehicle : vehList) {
				if (detailed) {		//If set to detailed view, display repairs and gas lists
					os << i << ". ";
					os << ShowFullVehicleInformation(&currentVehicle).str();
				}
				else {
					os << i << ". " << std::setw(currentVehicle.maxVehicleNameSize + 1) << std::left << currentVehicle.getName() << std::setw(6) << std::right << currentVehicle.getMileage() << " miles\n";
				}
				++i;
			}
		}

		return os;
	}
	std::ostringstream readableDayMonthYear() {
		time_t now{ time(0) };
		tm* ltm = localtime(&now);

		std::ostringstream date;

		switch (ltm->tm_wday) {
		case 0:
			date << "SUN";
			break;
		case 1:
			date << "MON";
			break;
		case 2:
			date << "TUE";
			break;
		case 3:
			date << "WED";
			break;
		case 4:
			date << "THU";
			break;
		case 5:
			date << "FRI";
			break;
		case 6:
			date << "SAT";
			break;
		default:
			date << "DDD";
		}
		date << ", ";

		switch (ltm->tm_mon) {
		case 0:
			date << "JAN";
			break;
		case 1:
			date << "FED";
			break;
		case 2:
			date << "MAR";
			break;
		case 3:
			date << "APR";
			break;
		case 4:
			date << "MAY";
			break;
		case 5:
			date << "JUN";
			break;
		case 6:
			date << "JUL";
			break;
		case 7:
			date << "AUG";
			break;
		case 8:
			date << "SEP";
			break;
		case 9:
			date << "OCT";
			break;
		case 10:
			date << "NOV";
			break;
		case 11:
			date << "DEC";
			break;
		default:
			date << "MMM";
		}


		int year{ ltm->tm_year + 1900 };

		date << ' ' << std::setw(2) << std::setfill('0') << ltm->tm_mday << ", " << year << '\n';

		/*
		
		THIS DISPLAYS THE TIME IN HH:MM:SS format

		std::cout << "   ["
			<< std::setw(2) << std::setfill('0') << ltm->tm_hour << ':'
			<< std::setw(2) << std::setfill('0') << ltm->tm_min << ':'
			<< std::setw(2) << std::setfill('0') << ltm->tm_sec << "]";

		std::cout << '\n';
		*/

		return date;
	}
	void DisplayBanner(const std::string title) {
		clear();
		unsigned short screenWidth{ 50 };
		std::ostringstream date{ readableDayMonthYear() };

		std::cout << std::setw(screenWidth/2) << std::left << title << std::setw(screenWidth/2) << std::right << date.str();
		for (int i{ 1 }; i < screenWidth; ++i) {
			std::cout << "=";
		}
		std::cout << "\n";
	}
}


