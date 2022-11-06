#include "DateTime.h"

std::string Date::string() const{
	std::ostringstream os;
	if(day == 0 && month == 0 && year == 0){
		os << "NA";
		return os.str();
	}

	os << day << ' ';

	switch (month) {
	case 1:
		os << "JAN";
		break;
	case 2:
		os << "FEB";
		break;
	case 3:
		os << "MAR";
		break;
	case 4:
		os << "APR";
		break;
	case 5:
		os << "MAY";
		break;
	case 6:
		os << "JUN";
		break;
	case 7:
		os << "JUL";
		break;
	case 8:
		os << "AUG";
		break;
	case 9:
		os << "SEP";
		break;
	case 10:
		os << "OCT";
		break;
	case 11:
		os << "NOV";
		break;
	case 12:
		os << "DEC";
		break;
	default:
		os << "MMM";
	}

	os << ' ' << year;

	return os.str();
}

std::ostream& operator<<(std::ostream& os, const Date& date){
	os << date.day << ' ' << date.month << ' ' << date.year;

	return os;
}

void Date::makeDate(std::stringstream& text){
	if(text.str() == ""){
		day = 0;
		month = 0;
		year = 0;
	}

	text >> day;
	text >> month;
	text >> year;

	return;
}