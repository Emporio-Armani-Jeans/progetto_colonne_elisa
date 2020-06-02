//
// Created by Elisa Messina on 02/06/2020.
//

#include "ColonnaChar.h"

ColonnaChar::ColonnaChar(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaChar::setVal(const string &valore_da_impostare) {
    char value_to_be_added = valore_da_impostare[0];
    _elementi_char.push_back(value_to_be_added);
}

string ColonnaChar::getElement(int index) {
    string str_to_return;
    str_to_return.push_back(_elementi_char[index]);
    return str_to_return;
}