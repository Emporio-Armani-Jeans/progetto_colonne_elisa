//
// Created by andry on 01/06/2020.
//

#include "ColonnaDate.h"

ColonnaDate::ColonnaDate(const string &nomecolonna, bool notnull){
    _nome_colonna=nomecolonna;
    _not_null = notnull;
}

void ColonnaDate::setValore(const string &value_to_set){
    int day, month, year;
    day=std::stoi(value_to_set.substr(0, 2));
    month=std::stoi(value_to_set.substr(3, 2));
    year=std::stoi(value_to_set.substr(6, 4));
    Date data(day,month,year);
    _elementi_date.push_back(data);
}

string ColonnaDate::getElemento(int index) const {
    return _elementi_date[index].str();
}

void ColonnaDate::deleteValore(int index) {
    auto it=_elementi_date.begin();
    it+=index;
    _elementi_date.erase(it);
}