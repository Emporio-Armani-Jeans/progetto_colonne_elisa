//
//
//

#ifndef DATA_DATA_H
#define DATA_DATA_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

class Date {

public:
	Date();
	Date(const int &d, const int &m, const int &y);
    // Copy Constructor
	Date(const Date & to_copy);
    // Getters
	int year() const;
	int month() const;
	int day() const;

	std::string str() const; //ritorna la data come una stringa di testo nel formato gg/mm/yyyy

	//overload degli operatori
	Date& operator= (const Date & to_assign);
	Date operator+(const Date & to_add) const;
	bool operator==(const Date & to_compare) const;
	bool operator!=(const Date & to_compare) const;
	bool operator<(const Date& to_compare)const;
	bool operator<=(const Date& to_compare)const;
    bool operator>(const Date& to_compare)const;
    bool operator>=(const Date& to_compare)const;

    friend std::ostream& operator<< (std::ostream& stream, const Date& d);
	friend std::istream& operator>> (std::istream& stream, Date d);

private:
	int _year; int _month;
	int _day;
};


#endif //DATA_DATA_H
