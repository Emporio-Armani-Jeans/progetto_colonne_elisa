//
// Created by Elisa Messina on 02/06/2020.
//

#include "ColonnaTime.h"
#include "Time.h"

ColonnaTime::ColonnaTime(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    Time s(0,0,0);
    _default_value = s;
    _primary_key = false;
}

string ColonnaTime::getElement(int index) {
    if(index == -1)
        return _default_value.str();
    else
        return _elementi_time[index].str();
}

void ColonnaTime::deleteVal(int index) {
    auto it = _elementi_time.begin();
    it += index;
    _elementi_time.erase(it);
}

void ColonnaTime::updateVal(const string& val, int index){
    int ora, minuti, secondi;
    ora = std::stoi(val.substr(0,2));
    minuti = std::stoi(val.substr(3,2));
    secondi = std::stoi(val.substr(6,2));
    Time time(ora,minuti,secondi);
    _elementi_time[index] = time;
}

void ColonnaTime::addDefault() {
    _elementi_time.push_back(_default_value);
}

bool ColonnaTime::compareElements(const string& condizione, int operatore, int index)const{
    int ora, minuti, secondi;
    ora = std::stoi(condizione.substr(0,2));
    minuti = std::stoi(condizione.substr(3,2));
    secondi = std::stoi(condizione.substr(6,2));
    Time to_compare(ora,minuti,secondi);
    switch (operatore){
        case 0:
            return (_elementi_time[index] == to_compare);
        case 1:
            return (_elementi_time[index] < to_compare);
        case 2:
            return (_elementi_time[index] <= to_compare);
        case 3:
            return (_elementi_time[index] > to_compare);
        case 4:
            return (_elementi_time[index] >= to_compare);
        default:
            //creare eccezione e toglere false
            return false;
    }
}