//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaText.h"

ColonnaText::ColonnaText(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaText::setValore(const string &value_to_set) {
    _elementi_di_testo.push_back(value_to_set);
}

string ColonnaText::getElemento(const int &index) const {
    return _elementi_di_testo[index];
}

void ColonnaText::deleteValore(const int &index) {
    auto it=_elementi_di_testo.begin();
    it+=index;
    _elementi_di_testo.erase(it);
}