#ifndef PROGETTO_COLONNE_TIME_H
#define PROGETTO_COLONNE_TIME_H

#include <iostream>
#include <string>
#include "InvalidTime.h"


class Time {
public:
    Time();
    Time(const int &h, const int &m, const int &s);
    // Copy Constructor
    Time(const Time & to_copy);

    // Getters
    int getHour() const;
    int getMinutes() const;
    int getSeconds() const;

    std::string str() const;//ritorna l'ora come una stringa di testo nel formato hh:mm:ss

    //overload degli operatori
    Time& operator=(const Time & to_assign);
    bool operator==(const Time & to_compare) const;
    bool operator!=(const Time & to_compare) const;
    bool operator<(const Time& to_compare)const;
    bool operator<=(const Time& to_compare)const;
    bool operator>(const Time& to_compare)const;
    bool operator>=(const Time& to_compare)const;

    friend std::ostream& operator<<(std::ostream& stream, const Time & d);
    friend std::istream& operator>>(std::istream& stream, Time d);

private:
    int _minuti, _secondi, _ora;
};


#endif //PROGETTO_COLONNE_TIME_H
