//
// Created by Elisa Messina on 02/06/2020.
//

#include "ColonnaChar.h"

ColonnaChar::ColonnaChar(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaChar::setValore(const string &value_to_set) {
    char value_to_be_added = value_to_set[0];
    _elementi_char.push_back(value_to_be_added);
}

string ColonnaChar::getElemento(int index) const{
    string str_to_return;
    str_to_return.push_back(_elementi_char[index]);
    return str_to_return;
}

void ColonnaChar::deleteValore(int index) {
    auto it=_elementi_char.begin();
    it+=index;
    _elementi_char.erase(it);
}