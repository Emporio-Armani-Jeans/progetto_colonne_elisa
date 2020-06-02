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

void ColonnaInt::setVal(const string &valore_da_impostare) {
    int value_to_be_added = std::stoi(valore_da_impostare);
    _elementi_interi.push_back(value_to_be_added);
}

string ColonnaInt::getElement(int index) {
    return to_string(_elementi_interi[index]);
}
