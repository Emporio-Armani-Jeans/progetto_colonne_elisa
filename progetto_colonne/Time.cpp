//
// Created by Elisa Messina on 02/06/2020.
//

#include "Time.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

Time::Time() {
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    _ora = now->tm_hour + 1;
    _minuti = now->tm_min + 1;
    _secondi = now->tm_sec + 1;
}

Time::Time(const int &h, const int &m, const int &s) {
    if (h>=0 && h<25 && m>=0 && m<60 && s>=0 && s<60){
        _ora = h;
        _minuti = m;
        _secondi = s;
    } else
        throw InvalidTime();
}

Time::Time(const Time &to_copy) {
    *this = to_copy;
}

int Time::getHour() const {
    return _ora;
}

int Time::getMinutes() const {
    return _minuti;
}

int Time::getSeconds() const {
    return _secondi;
}

Time &Time::operator=(const Time &to_assign) {
    _ora = to_assign._ora;
    _minuti = to_assign._minuti;
    _secondi = to_assign._secondi;
    return (*this);
}

bool Time::operator==(const Time &to_compare) const {
    return (_ora == to_compare._ora &&
            _minuti == to_compare._minuti &&
            _secondi == to_compare._secondi);
}

bool Time::operator!=(const Time &to_compare) const {
    return !(*this == to_compare);
}

std::string Time::str() const {
    std::ostringstream stream;
    stream 	<< std::setfill('0') << std::setw(2) << _ora << ":"
              << std::setfill('0') << std::setw(2) << _minuti << ":"
              << std::setfill('0') << std::setw(2) << _secondi;
    return stream.str();
}

std::ostream &operator<<(std::ostream &stream, const Time &d) {
    stream << d.str();
    return  stream;
}

std::istream &operator>>(std::istream &stream, Time d) {
    char c;
    stream >> d._ora >> c >> d._minuti >> c >> d._secondi;
    return stream;
}

bool Time::operator<(const Time &to_compare) const {
    if(_ora==to_compare._ora){
        if(_minuti==to_compare._minuti){
            return _secondi<to_compare._secondi;
        }
        else return _minuti<to_compare._minuti;
    }
    else return _ora < to_compare._ora;
}

bool Time::operator<=(const Time &to_compare) const {
    if(_ora==to_compare._ora){
        if(_minuti==to_compare._minuti){
            return _secondi<=to_compare._secondi;
        }
        else return _minuti<=to_compare._minuti;
    }
    else return _ora <= to_compare._ora;
}

bool Time::operator>(const Time &to_compare) const {
    if(_ora==to_compare._ora){
        if(_minuti==to_compare._minuti){
            return _secondi>to_compare._secondi;
        }
        else return _minuti>to_compare._minuti;
    }
    else return _ora > to_compare._ora;
}

bool Time::operator>=(const Time &to_compare) const {
    if(_ora==to_compare._ora){
        if(_minuti==to_compare._minuti){
            return _secondi>=to_compare._secondi;
        }
        else return _minuti>=to_compare._minuti;
    }
    else return _ora >= to_compare._ora;
}




