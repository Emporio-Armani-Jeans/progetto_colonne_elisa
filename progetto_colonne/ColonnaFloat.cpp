//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaFloat.h"

ColonnaFloat::ColonnaFloat(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaFloat::setVal(const string &valore_da_impostare) {
    float value_to_be_added = std::stof(valore_da_impostare);
    _elementi_float.push_back(value_to_be_added);
}

string ColonnaFloat::getElement(int i){
    return to_string(_elementi_float[i]);
}
