#include "data.h"
Date::Date() {
	std::time_t t = std::time(0);   // get time now
	std::tm* now = std::localtime(&t);
	_day = now->tm_mday;
	_month = now->tm_mon + 1;
	_year = now->tm_year + 1900;
}

Date::Date(const Date &to_copy) {
	*this = to_copy;
}

Date::Date(const int &d, const int &m, const int &y) {
    if (d >= 0 && d < 32 && m >= 0 && m < 13 && y >= 0){
        _day = d;
        _month = m;
        _year = y;
    } else {
        throw InvalidDate();
    }
}

int Date::day() const {
	return _day;
}

int Date::month() const {
	return _month;
}

int Date::year() const {
	return _year;
}


std::string Date::str() const {
	std::ostringstream stream;

	stream 	<< std::setfill('0') << std::setw(2) << _day << "/"
			<< std::setfill('0') << std::setw(2) << _month << "/"
			<< std::setfill('0') << std::setw(4) << _year;
	return stream.str();
}

Date& Date::operator=(const Date &to_assign) {
	_day = to_assign._day;
	_month = to_assign._month;
	_year = to_assign._year;
	return (*this);
}

bool Date::operator==(const Date &to_compare) const {
	return ((_day == to_compare._day) &&
			(_month == to_compare._month) &&
			(_year == to_compare._year));
}

bool Date::operator!=(const Date &to_compare) const {
	return !(*this == to_compare);
}

Date Date::operator+(const Date &to_add) const {
	Date ret(to_add);
	ret._day += _day;
	ret._month += _month;
	ret._year += _year;
	return ret;
}
std::ostream& operator<< (std::ostream& stream, const Date& d) {
	stream << d.str();
	return  stream;
}

std::istream &operator>>(std::istream &stream, Date d) {
    char c;
    stream >> d._day >> c >> d._month >> c >> d._year;
    return stream;
}

bool Date::operator<(const Date &to_compare) const {
    if(_year==to_compare._year){
        if(_month==to_compare._month){
            return _day<to_compare._day;
        }
        else return _month<to_compare._month;
    }
    else return _year < to_compare._year;
}

bool Date::operator<=(const Date &to_compare) const {
    if(_year==to_compare._year){
        if(_month==to_compare._month){
            return _day<=to_compare._day;
        }
        else return _month<=to_compare._month;
    }
    else return _year <= to_compare._year;
}

bool Date::operator>(const Date &to_compare) const {
    if(_year==to_compare._year){
        if(_month==to_compare._month){
            return _day>to_compare._day;
        }
        else return _month>to_compare._month;
    }
    else return _year > to_compare._year;
}

bool Date::operator>=(const Date &to_compare) const {
    if(_year==to_compare._year){
        if(_month==to_compare._month){
            return _day>=to_compare._day;
        }
        else return _month>=to_compare._month;
    }
    else return _year >= to_compare._year;
}
