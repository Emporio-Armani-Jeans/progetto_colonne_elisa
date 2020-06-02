//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaInt.h"

ColonnaInt::ColonnaInt(const string &nomecolonna, bool notnull, bool autoincrement) {
    _nome_colonna = nomecolonna;
    if (notnull)
        _not_null = true;
    if (autoincrement)
        _auto_increment = true;
}

void ColonnaInt::setValore(const string &value_to_set) {
    int value_to_be_added = std::stoi(value_to_set);
    _elementi_interi.push_back(value_to_be_added);
}

string ColonnaInt::getElemento(const int &index) const {
    return to_string(_elementi_interi[index]);
}

void ColonnaInt::deleteValore(const int &index){
    auto it=_elementi_interi.begin();
    it+=index;
    _elementi_interi.erase(it);
}

