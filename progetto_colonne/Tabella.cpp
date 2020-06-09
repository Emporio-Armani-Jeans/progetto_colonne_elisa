//
// Created by Elisa Messina on 01/06/2020.
//

#include "Tabella.h"

Tabella::Tabella(const string &nometabella) {
    _nome_tabella = nometabella;
    _chiave_primaria = nullptr;
    _recs=0;
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

int Tabella::numCampi() const {
    return _colonne.size();
}

int Tabella::numRecs() const {
    return _recs;
}

void Tabella::addRecord(const vector<string>& campi, const vector<string>& valori) {
    _recs++;
    for(auto & i : _colonne){
        i->addDefault();
    }
    for(int i=0; i<campi.size(); i++){
        for(auto & j : _colonne){
            if(campi[i]==j->getNomeColonna())
                j->updateVal(valori[i], int(_recs-1));
        }
    }
}

void Tabella::deleteRecord(const string& nome_col, const string &condizione, int operatore) {
    bool trovata=false;
    int i=0, j=0;
    while(i<_colonne.size() && !trovata){
        if(nome_col==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        while(j<_recs){
            if (_colonne[i]->compareElements(condizione, operatore, j)){
                _recs--;
                for (auto &elem : _colonne) {
                    elem->deleteVal(j);
                }
            }else j++;
        }
    }else{
        //creare eccezione campo non esistente
    }
}

void Tabella::deleteRecord(const string& nome_col, const string& condizione1, const string& condizione2){
    bool trovata=false;
    int i=0, j=0;
    while(i<_colonne.size() && !trovata){
        if(nome_col==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        while(j<_recs){
            if (_colonne[i]->compareElements(condizione1, 4, j) && _colonne[i]->compareElements(condizione2, 2, j)) {
                _recs--;
                for (auto &elem : _colonne) {
                    elem->deleteVal(j);
                }
            }else j++;
        }
    }else{
        //creare eccezione campo non esistente
    }
}

void Tabella::updateRecord(const string& condizione, const string& nome_col, const vector<string>& campi, const vector<string>& valori){
    bool trovata=false;
    int i=0, j;
    while(i<_colonne.size() && !trovata){
        if(nome_col==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs; j++){
            if(_colonne[i]->getElement(j)==condizione) {
                for(int y=0; y<campi.size(); y++){
                    for(auto & g : _colonne){
                        if(campi[y]==g->getNomeColonna()){
                            g->updateVal(valori[y],j);
                        }
                    }
                }
            }
        }
    }else{
        //creare eccezione campo non esistente
    }
}

vector<string> Tabella::returnData()const{
    vector<string> righe_testo;
    if(_recs!=0) {
        string riga;
        for (int i = 0; i < _recs; i++) {
            riga.clear();
            for (auto j : _colonne) {
                if (j->getElement(i) == j->getElement(-1)) {
                    riga += "___ ";
                } else {
                    riga += j->getElement(i);
                    riga += " ";
                }
            }
            righe_testo.push_back(riga);
        }
    }else{
        righe_testo.emplace_back("Tabella vuota");
    }
    return righe_testo;
}

vector<string> Tabella::returnData(const vector<string>& campi) const {
    vector<string> righe_testo;
    string riga;
    for(int i=0; i<_recs; i++){
        riga.clear();
        for(auto j : _colonne){
            for(const auto & s : campi){
                if(j->getNomeColonna()==s) {
                    if(j->getElement(i)==j->getElement(-1)){
                        riga+="___ ";
                    }else {
                        riga += j->getElement(i);
                        riga += " ";
                    }
                }
            }
        }
        righe_testo.push_back(riga);
    }
    return righe_testo;
}

vector<string> Tabella::returnData(const vector<string> &campi, const string& campo_condizione, const string &condizione) {
    vector<string> righe_testo;
    string riga;
    bool trovata=false;
    int i=0;
    while(i<_colonne.size() && !trovata){
        if(campo_condizione==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(int j=0; j<_recs; j++){
            if(_colonne[i]->getElement(j)==condizione) {
                riga.clear();
                for(auto elem : _colonne){
                    for(const auto & s : campi){
                        if(elem->getNomeColonna()==s) {
                            if(elem->getElement(i)==elem->getElement(-1)){
                                riga+="___ ";
                            }else {
                                riga += elem->getElement(i);
                                riga += " ";
                            }
                        }
                    }
                }
                righe_testo.push_back(riga);
            }
        }
    }else{
        //eccezione campo non trovato
    }
    return righe_testo;
}
