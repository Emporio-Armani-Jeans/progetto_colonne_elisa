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
            (*elem).addVal(valore);
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
        if(nome_col==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs.size(); j++){
            if(_colonne[i]->getElement(j)==condizione) {
                it+=j;
                _recs.erase(it);
                for(auto & elem : _colonne){
                    elem->deleteVal(j);
                }
            }
        }
    }else{
        //creare eccezione campo non esistente
    }
}

void Tabella::updateRecord(const string& condizione, const string& nome_col, vector<string> campi, vector<string> valori){
    bool trovata=false;
    int i=0, j;
    while(i<_colonne.size() && !trovata){
        if(nome_col==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs.size(); j++){
            if(_colonne[i]->getElement(j)==condizione) {
                for(int y=0; y<campi.size(); y++){
                    for(int g=0; g<_colonne.size(); g++){
                        if(campi[y]==_colonne[g]->getNomeColonna()){
                            _colonne[g]->updateVal(valori[y],j);
                        }
                    }
                }
            }
        }
    }else{
        //creare eccezione campo non esistente
    }
}

vector<string> Tabella::returnData() {
    vector<string> righe_testo;
    string riga;
    for(int i=0; i<_recs.size(); i++){
        riga.clear();
        for(int j=0; j<_colonne.size(); j++){
            riga+=_colonne[j]->getElement(i);
            riga+=" ";
        }
        righe_testo.push_back(riga);
    }
    return righe_testo;
}
