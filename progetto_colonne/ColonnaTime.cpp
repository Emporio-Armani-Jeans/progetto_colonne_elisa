//
// Created by Elisa Messina on 02/06/2020.
//

#include "ColonnaTime.h"
#include "Time.h"

ColonnaTime::ColonnaTime(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaTime::setVal(const string &valore_da_impostare) {
    int ora, minuti, secondi;
    ora=std::stoi(valore_da_impostare.substr(0,2));
    minuti=std::stoi(valore_da_impostare.substr(3,2));
    secondi=std::stoi(valore_da_impostare.substr(6,4));
    Time time(ora,minuti,secondi);
    _elementi_time.push_back(time);
}

string ColonnaTime::getElement(int index) {
    return _elementi_time[index].str();
}

void ColonnaTime::deleteVal(int index) {
    auto it=_elementi_time.begin();
    it+=index;
    _elementi_time.erase(it);
}