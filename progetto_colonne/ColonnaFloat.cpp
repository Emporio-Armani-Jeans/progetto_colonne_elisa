//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaFloat.h"

ColonnaFloat::ColonnaFloat(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaFloat::setValore(const string &value_to_set) {
    float value_to_be_added = std::stof(value_to_set);
    _elementi_float.push_back(value_to_be_added);
}

string ColonnaFloat::getElemento(int i) const {
    return to_string(_elementi_float[i]);
}

void ColonnaFloat::deleteValore(int index) {
    auto it=_elementi_float.begin();
    it+=index;
    _elementi_float.erase(it);
}