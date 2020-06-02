//
// Created by Elisa Messina on 02/06/2020.
//

#include "ColonnaTime.h"
#include "Time.h"

ColonnaTime::ColonnaTime(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaTime::setValore(const string &value_to_set) {
    int ora, minuti, secondi;
    ora=std::stoi(value_to_set.substr(0, 2));
    minuti=std::stoi(value_to_set.substr(3, 2));
    secondi=std::stoi(value_to_set.substr(6, 4));
    Time time(ora,minuti,secondi);
    _elementi_time.push_back(time);
}

string ColonnaTime::getElemento(const int &index) const{
    return _elementi_time[index].str();
}

void ColonnaTime::deleteValore(const int &index) {
    auto it=_elementi_time.begin();
    it+=index;
    _elementi_time.erase(it);
}