#include "DateTime.h"


void DateTime::MakeDate(std::stringstream& text, Date& target){

	// If the string is empty, set to 1 JAN 1900
	if(text.str() == ""){
		target.day = 1;
		target.month = 1;
		target.year = 1900;
		RST::Log("Date cannot be blank. Set date to 1 JAN 1900", LogCode::WARNING);
	}
	else{
		int dayBuf, monthBuf, yearBuf;		// Input buffers

		// Read the text and write to buffers
		text >> dayBuf;
		text >> monthBuf;
		text >> yearBuf;

		// Ensure the buffers are valid before assigning member variables
		if(!CheckDate(dayBuf, monthBuf, yearBuf)){
			std::ostringstream txt; txt << dayBuf << " " << monthBuf << " " << yearBuf;
			RST::Log("Date [" + txt.str() + "] not accepted. Set date to 1 JAN 1900", LogCode::ERROR);
			target.day = 1;
			target.month = 1;
			target.year = 1900;
		}
		else{
			// If the input buffers are valid, set member variables accordingly 
			target.day = dayBuf;
			target.month = monthBuf;
			target.year = yearBuf;

			std::ostringstream txt; txt << target;
			RST::Log("Date was set to [" + txt.str() + "]", LogCode::RUNTIME_LOW);
		}
	}

	return;
}

std::string Date::string() const{
	std::ostringstream os;			// Output stream buffer

	if(day == 0 && month == 0 && year == 0){	// If variables are zero, make string say 'NA'
		os << "NA";
		return os.str();
	}

	os << std::setw(2) << std::setfill('0') << day << ' ';

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

	// Output as numbers
	os << date.day << ' ' << date.month << ' ' << date.year;

	return os;
}


bool 	DateTime::CheckDate			(int day, int month, int year){
	bool success {false};

	// Check valid year
	if(year < 1900){
		RST::Log("Year was below 1900", LogCode::RUNTIME_MED);
		return false; 
	}

	// Check valid month
	if(month <= 0 || month > 12){
		RST::Log("Month was not between 1 and 12", LogCode::RUNTIME_MED);
		return false; 
	}

	// Check valid day
	if(day <= 0 ){
		RST::Log("Day was at or below 0", LogCode::RUNTIME_MED);
		return false; 
	}
	else{
		// Check if the days allowed in each month is accurate
		if(month == 1 && day > 31){
			RST::Log("Day was out of range for January. Needs to be at or below 31", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 2 && day > 29){
			RST::Log("Day was out of range for February. Needs to be at or below 29", LogCode::RUNTIME_MED);
		}
		else if(month == 3 && day > 31){
			RST::Log("Day was out of range for March. Needs to be at or below 31", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 4 && day > 30){
			RST::Log("Day was out of range for April. Needs to be at or below 30", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 5 && day > 31){
			RST::Log("Day was out of range for May. Needs to be at or below 31", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 6 && day > 30){
			RST::Log("Day was out of range for June. Needs to be at or below 30", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 7 && day > 31){
			RST::Log("Day was out of range for July. Needs to be at or below 31", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 8 && day > 31){
			RST::Log("Day was out of range for August. Needs to be at or below 31", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 9 && day > 30){
			RST::Log("Day was out of range for September. Needs to be at or below 30", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 10 && day > 31){
			RST::Log("Day was out of range for October. Needs to be at or below 31", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 11 && day > 30){
			RST::Log("Day was out of range for November. Needs to be at or below 30", LogCode::RUNTIME_MED);
			success = false;
		}
		else if(month == 12 && day > 31){
			RST::Log("Day was out of range for December. Needs to be at or below 31", LogCode::RUNTIME_MED);
			success = false;
		}
		else{
			RST::Log("Date was accepted", LogCode::RUNTIME_LOW);
			success = true;
		}
	}

	return success;
}