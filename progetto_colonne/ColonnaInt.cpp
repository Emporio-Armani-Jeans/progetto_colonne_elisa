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

string ColonnaInt::getNomeColonna() const {
    return _nome_colonna;
}

void ColonnaInt::impostaValore(const string &valore_da_impostare) {
    int value_to_be_added = std::stoi(valore_da_impostare);
    _elementi_interi.push_back(value_to_be_added);
}
