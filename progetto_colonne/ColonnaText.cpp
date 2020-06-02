//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaText.h"

ColonnaText::ColonnaText(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
}

void ColonnaText::impostaValore(const string &valore_da_impostare) {
    _elementi_di_testo.push_back(valore_da_impostare);
}

string ColonnaText::getElement(int index){
    return _elementi_di_testo[index];
}