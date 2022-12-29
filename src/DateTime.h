#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <RST/RST.h>

#include <sstream>
#include <string>
#include <iomanip>

struct Date{
	Date(unsigned short setDay=0, unsigned short setMonth=0, int setYear=0)
	: 	day{setDay},
		month{setMonth},
		year{setYear}
	{ }
	~Date() { };

	std::string 	string() const;								// Creates a string of the Date type. Formatted as 12 DEC 2022

	friend std::ostream& operator<<(std::ostream& os, const Date& date);

	// Member variables
	unsigned short day, month;
	int year;
};

namespace DateTime{
	bool CheckDate (int day, int month, int year);				// Ensures the parameters are a valid Date (such as not 31 days in April)
	void MakeDate(std::stringstream& text, Date& target);		// Overwrise parameter with a Date type from a string of text, If invalid, set to 1 JAN 1900
}


#endif