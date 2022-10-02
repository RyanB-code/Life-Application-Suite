#include "Display.h"

#pragma warning(disable : 4996)

namespace Display {

	void clear()
	{
		system("cls");
	}
	void clearLineAfterInput() {
		printf("\033[A"); //goes up a line
		printf("\33[2K"); //clears current line
	}

	int getInput(short low, short high) {
		int input{ 0 };

		bool exit{ false };
		while (!exit) {
			std::cout << ">";
			std::cin >> input;

			clearLineAfterInput();

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				if (input < low || input > high) {

				}
				else {
					exit = true;
				}
			}
		}

		return input;
	}
	bool getBoolInput() {
		bool success{false};

		bool exit{ false };
		while (!exit) {
			std::cout << "(y/n) >";
			char input;
			std::cin >> input;
			clearLineAfterInput();

			switch (input) {
			case 'y': case 'Y':
				std::cin.ignore(10000, '\n');
				success = true;
				exit = true;
				break;
			case 'n': case 'N':
				std::cin.ignore(10000, '\n');
				success = false;
				exit = true;
				break;
			default:
				std::cin.ignore(10000, '\n');
				exit = false;
				break;
			}

			if (std::cin.fail()) {
				std::cin.clear();
				std::cin.ignore(10000, '\n');
			}
			else {
				//Do Nothing
			}
		}

		return success;
	}

	//===========Menus/Screens=====================
	void Home(Application* app)
	{
		bool exit{ false };
		do {
			Log(LogCode::LOG, "Home Screen called");

			DisplayBanner("Life Application Suite", "A place for all of life's needs");
			std::cout << "\nOptions: \n";
			std::cout << "1. Vehicle Manager\n";
			std::cout << "2. Settings\n";
			std::cout << "3. Exit\n";


			switch (getInput(1, 3)) {
			case 1:
				VehicleHome(app);
				break;
			case 2:
				Settings(app);
				break;
			case 3:
				exit = true;
				break;
			}

		} while (!exit);
	}
	void VehicleHome(Application* app) {
		Log(LogCode::LOG, "Vehicle Home Screen called");

		Vehicle* selectedVehicle{ nullptr };

		bool exit{ false };
		while (!exit) {
			DisplayBanner("Vehicles -> Home", "Remember to save hit save before exiting...\n");

			std::cout << "Vehicles:\n";
			std::cout << ListVehicles(app->getVehicleList()).str();

			if (app->getVehicleList().empty()) {
				std::cout << "\nOptions:\n";
				std::cout << "1. Add new Vehicle\n";
				std::cout << "2. Go back\n";
				switch (getInput(1, 2)) {
				case 1:
					AddVehicle(app);
					selectedVehicle = nullptr;
					break;
				case 2:
					exit = true;
					break;
				default:
					std::cout << "\nInvalid number. Restarting...\n";
					Sleep(2000);
					break;
				}
			}

			else {
				if (!selectedVehicle) {
					std::cout << "\nOptions: \n";
					std::cout << "1. Select Vehicle\n";
					std::cout << "2. View All Vehicle Info\n";
					std::cout << "3. Add new Vehicle\n";
					std::cout << "\n4. Save all Vehicles\n\n";
					std::cout << "5. Go back\n";

					switch (getInput(1, 5)) {
					case 1:
						selectedVehicle = SelectVehicle(app->getVehicleList());
						break;
					case 2:
						ShowFullVehicleInformation(app->getVehicleList());
						break;
					case 3:
						AddVehicle(app);
						selectedVehicle = nullptr;
						break;
					case 4:
						if (app->saveVehicles()) {
							std::cout << "\nSaved all vehicle information.\n";
						}
						else {
							std::cout << "\nCould not save all vehicle information.\n";
						}
						Sleep(2000);
						break;
					case 5:
						exit = true;
						break;
					default:
						std::cout << "\nInvalid number. Restarting...\n";
						Sleep(2000);
						break;
					}
				}
				else {
					std::cout << "\nSelected Vehicle > " << selectedVehicle->getName() << "\n";
					std::cout << "\nOptions: \n";
					std::cout << "1. Deselect Vehicle\n";
					std::cout << "2. Add new Vehicle\n";
					std::cout << "\n";
					std::cout << "3. View Selected Vehicle Info\n";
					std::cout << "4. Add a new repair\n";
					std::cout << "5. Add a new gas stop\n";
					std::cout << "6. Edit name\n";
					std::cout << "\n";
					std::cout << "7. Save all vehicles\n";
					std::cout << "\n";
					std::cout << "8. Go back\n";

					switch (getInput(1, 8)) {
					case 1:
						selectedVehicle = nullptr;
						break;
					case 2:
						AddVehicle(app);
						selectedVehicle = nullptr;
						break;
					case 3:
						DisplayBanner("Vehicle -> View");
						std::cout << ShowFullVehicleInformation(selectedVehicle).str();
						system("pause");
						break;
					case 4:
						AddRepair(selectedVehicle);
						break;
					case 5:
						AddGasStop(selectedVehicle);
						break;
					case 6:
						std::cout << "\nIn Edit name\n";
						Sleep(2000);
						break;
					case 7:
						if (app->saveVehicles()) {
							std::cout << "\nSaved all vehicle information.\n";
						}
						else {
							std::cout << "\nCould not save all vehicle information.\n";
						}
						Sleep(2000);
						break;
					case 8:
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
	
	bool AddVehicle(Application* app) {
		bool returnValue{false};
		
		std::string nameBuf;
		uint32_t mileBuf;
		DisplayBanner("Create Vehicle", "To create a new vehicle, we need some information.\n");

		std::cout << "Name\n>";
		std::cin.ignore(10000, '\n');
		std::getline(std::cin, nameBuf);

		bool exit{ false };
		do {
			DisplayBanner("Create Vehicle", "To create a new vehicle, we need some information.\n");
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

		DisplayBanner("Create Vehicle", "Review before submitting...\n");
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
	bool AddRepair(Vehicle* veh) {
		bool success{false};

		uint32_t mileBuf{ 0 };
		std::string typeBuf;
		double costBuf{ 0.0 };
		std::string notesBuf;
		bool thirdPartyBuf;

		bool exit0{ false };
		do {
			DisplayBanner("Edit -> " + veh->getName(), "To add a new repair we need some information.\n\n");

			bool exit1{ false };
			do {
				std::cout << "Mileage done >";
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

			DisplayBanner("Edit -> " + veh->getName(), "To add a new repair we need some information.\n\n");
			std::cout << "The Type of repair. [Max characters " << veh->maxRepairTypeSize << "]\nExamples: 'Oil Change', 'Power Steering', 'Body Work', etc...\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, typeBuf);


			bool exit2{ false };
			do {
				DisplayBanner("Edit -> " + veh->getName(), "To add a new repair we need some information.\n\n");
				std::cout << "Cost >";
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

			DisplayBanner("Edit -> " + veh->getName(), "To add a new repair we need some information.\n\n");
			std::cout << "Notes. [Max characters " << veh->maxNotesSize << "]\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, notesBuf);

		
			DisplayBanner("Edit -> " + veh->getName(), "To add a new repair we need some information.\n\n");
			std::cout << "Did a third party perform the repair?" << std::endl;
			thirdPartyBuf = getBoolInput();

			DisplayBanner("Edit -> " + veh->getName(), "Review before saving.");
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
	bool AddGasStop(Vehicle* veh) {
		bool success{false};

		uint32_t mileBuf{ 0 };
		short galBuf;
		double ppgBuf{ 0.0 };
		std::string notesBuf;

		bool exit0{ false };
		do {
			bool exit1{ false };
			do {
				DisplayBanner("Edit -> " + veh->getName(), "To add a new gas stop we need some information.\n\n");
				std::cout << "Mileage done >";
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

			DisplayBanner("Edit -> " + veh->getName(), "To add a new gas stop we need some information.\n\n");
			bool exit2{ false };
			do {
				std::cout << "Gallons >";
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
				DisplayBanner("Edit -> " + veh->getName(), "To add a new gas stop we need some information.\n\n");
				std::cout << "Price Per Gallon >";
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

			DisplayBanner("Edit -> " + veh->getName(), "To add a new repair we need some information.\n\n");
			std::cout << "Notes. [Max characters " << veh->maxNotesSize << "]\n>";
			std::cin.ignore(10000, '\n');
			std::getline(std::cin, notesBuf);

			DisplayBanner("Edit -> " + veh->getName(), "Review before saving.");
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
			Log(LogCode::ROUTINE, "Selected vehicle " + selVeh->getName());
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
	void DisplayBanner(const std::string title, const std::string subheading) {
		clear();
		unsigned short screenWidth{ 50 };
		std::ostringstream date{ readableDayMonthYear() };

		std::cout << std::setw(screenWidth/2) << std::left << title << std::setw(screenWidth/2) << std::right << date.str();
		for (int i{ 1 }; i < screenWidth; ++i) {
			std::cout << "=";
		}
		std::cout << "\n";
		if (subheading != "") {
			std::cout << subheading << "\n";
		}
	}
}