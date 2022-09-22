#include "Display.h"

#pragma warning(disable : 4996)

namespace Display {

	void clear()
	{ 
		system("cls"); 
	}

	void Home()
	{
		Log(LogCode::LOG, "Home Screen called");
		bool exit{ false };
		while (!exit) {
			clear();
			
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
					VehicleHome();
					break;
				case 2:
					std::cout << "\nIn settings. Does nothing now. Restarting...\n";
					Sleep(2000);
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
	void VehicleHome() {
		Log(LogCode::LOG, "Vehicle Home Screen called");
		bool exit{ false };
		while (!exit) {
			clear();
			DisplayBanner("Vehicles Home Page");

			std::cout << "\nOptions: \n";
			std::cout << "1. List Vehicles\n";
			std::cout << "2. Save Vehicles\n";
			std::cout << "3. Settings\n";
			std::cout << "4. Home Page\n";

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
					std::cout << "\nSelected Vehicle List\n";
					Sleep(2000);
					break;
				case 2:
					std::cout << "\nSelected Save Vehicle List\n";
					Sleep(2000);
					break;
				case 3:
					std::cout << "\nIn settings. Does nothing now. Restarting...\n";
					Sleep(2000);
					break;
				case 4:
					std::cout << "\nExiting...\n";
					Sleep(2000);
					exit = true;
					break;
				default:
					std::cout << "\nInvalid number. Restarting...\n";
					Sleep(2000);
					break;
				} //end switch
			}
		} //end while loop
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
		date << " ";

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
		unsigned short screenWidth{ 50 };
		std::ostringstream date{ readableDayMonthYear() };

		std::cout << std::setw(screenWidth/2) << std::left << title << std::setw(screenWidth/2) << std::right << date.str();
		for (int i{ 1 }; i < screenWidth; ++i) {
			std::cout << "=";
		}
		std::cout << "\n";
	}
}


