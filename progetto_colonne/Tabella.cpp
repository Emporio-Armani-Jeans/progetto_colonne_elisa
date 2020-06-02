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
        i = nullptr;
    }
}

void Tabella::impostaValore(const string &nomecolonna, const string &value_to_set) {
    int index;
    for (auto elem : _colonne){
        if ((*elem).getNomeColonna() == nomecolonna) {
            (*elem).setValore(value_to_set);
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

void Tabella::deleteRecord(const string& nome_col, const string &condizione) {
    bool trovata=false;
    int i=0, j;
    vector<bool>::iterator it=_recs.begin();
    while(i<_colonne.size() && !trovata){
        if(nome_col==_colonne[i]->getNomeColonna())
            trovata = true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs.size(); j++){
            if(_colonne[i]->getElemento(j) == condizione) {
                it+=j;
                _recs.erase(it);
                for(auto & elem : _colonne){
                    elem->deleteValore(j);
                }
            }
        }
    } else {
        //creare eccezione campo non esistente
    }
}
