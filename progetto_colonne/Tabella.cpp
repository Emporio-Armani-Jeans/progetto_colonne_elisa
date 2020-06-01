//
// Created by Elisa Messina on 01/06/2020.
//

#include "Tabella.h"

Tabella::Tabella(const string &nometabella) {
    _nome_tabella = nometabella;
    _numero_colonne = 0;
}

void Tabella::aggiungiColonna(Colonna *to_be_added) {
    _numero_colonne++;
    _colonne.push_back(to_be_added);
}

void Tabella::setChiavePrimaria(Colonna *to_be_primary_key) {
    _chiave_primaria = to_be_primary_key;
}

Tabella::~Tabella() {
    _colonne.clear();
}

void Tabella::impostaValore(const string &nomecolonna, const string &valore) {
    for (auto elem : _colonne){
        if ((*elem).getNomeColonna() == nomecolonna) {
            (*elem).impostaValore(valore);
        }
    }
}
