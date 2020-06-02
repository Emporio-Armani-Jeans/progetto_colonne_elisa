//
// Created by Elisa Messina on 01/06/2020.
//

#include "Tabella.h"

Tabella::Tabella(const string &nometabella) {
    _nome_tabella = nometabella;
    _chiave_primaria = nullptr;
}

void Tabella::aggiungiColonna(Colonna *to_be_added) {
    _colonne.push_back(to_be_added);
}

void Tabella::setChiavePrimaria(Colonna *to_be_primary_key) {
    _chiave_primaria = to_be_primary_key;
}

Tabella::~Tabella() {
    for(auto & i : _colonne){
        delete i;
        i= nullptr;
    }
}

void Tabella::impostaVal(const string &nomecolonna, const string &valore) {
    int index;
    for (auto elem : _colonne){
        if ((*elem).getNomeColonna() == nomecolonna) {
            (*elem).setVal(valore);
        }
    }
}

Colonna *Tabella::getCol(int index) const {
    return _colonne[index];
}

bool Tabella::recExists(int index) const {
    return _recs[index];
}

int Tabella::numCampi() const {
    return _colonne.size();
}

int Tabella::numRecs() const {
    return _recs.size();
}

void Tabella::addRecord() {
    _recs.push_back(true);
}
