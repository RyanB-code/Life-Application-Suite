#ifndef DATE_TIME_H
#define DATE_TIME_H

#include <sstream>
#include <string>
#include <iomanip>

struct Date{
	Date(unsigned short setDay=0, unsigned short setMonth=0, int setYear=0)
	: 	day{setDay},
		month{setMonth},
		year{setYear}
	{

	}
	Date(std::stringstream& text) { makeDate(text); }

	unsigned short day, month;
	int year;

	std::string string() const;

	friend std::ostream& operator<<(std::ostream& os, const Date& date);

	// Writes a Date type from a string of text
	void makeDate(std::stringstream& text);
};




#endif