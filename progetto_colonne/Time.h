//
// Created by Elisa Messina on 02/06/2020.
//

#ifndef PROGETTO_COLONNE_TIME_H
#define PROGETTO_COLONNE_TIME_H

#include <iostream>
#include <string>


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

    Time& operator=(const Time & to_assign);
    bool operator==(const Time & to_compare) const;
    bool operator!=(const Time & to_compare) const;
    bool operator<(const Time& to_compare)const;
    bool operator<=(const Time& to_compare)const;
    bool operator>(const Time& to_compare)const;
    bool operator>=(const Time& to_compare)const;

    std::string str() const;

    friend std::ostream& operator<<(std::ostream& stream, const Time & d);
    friend std::istream& operator>>(std::istream& stream, Time d);

private:
    int _minuti, _secondi, _ora;
};


#endif //PROGETTO_COLONNE_TIME_H
