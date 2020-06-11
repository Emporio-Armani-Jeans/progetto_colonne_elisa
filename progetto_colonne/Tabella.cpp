//
// Created by Elisa Messina on 01/06/2020.
//

#include "Tabella.h"

Tabella::Tabella(const string &nometabella) {
    _nome_tabella = nometabella;
    _recs=0;
}

Tabella::~Tabella() {
    for(auto & i : _colonne){
        delete i;
        i = nullptr;
    }
}


void Tabella::aggiungiColonna(Colonna *to_be_added) {
    _colonne.push_back(to_be_added);
}

void Tabella::setChiavePrimaria(const string& nomecolonna) {
    bool flag_another_pk_found = false, flag_colonna_trovata = false;
    for(auto & i : _colonne) {   //verifica che non ci siano altre colonne marcate come primary key
        if(i->_primary_key)
            flag_another_pk_found = true;
    }
    if(!flag_another_pk_found){ //se effettivamente non è ancora stata aggiunta una colonna marcata come primary key, posso impostarla
        for(int j = 0; j < _colonne.size() && !flag_colonna_trovata; j++){
            if(_colonne[j]->getNomeColonna() == nomecolonna){
                _colonne[j]->_primary_key = true;
                _colonne[j]->_not_null = true;
                flag_colonna_trovata = true;
            }
            else {
                //eccezione colonna non esistente
            }
        }
    }
    else{
        //eccezione già c'è un'altra colonna primary key
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
    bool flag_campo_non_trovato = false, flag_colonna_trovata = false;
    for(int i = 0; i < _colonne.size() && !flag_campo_non_trovato; i++){
        flag_colonna_trovata=false;
        if(_colonne[i]->_not_null){ //se la colonna è marcata come not null, all'interno dei campi da modificare deve esserci il suo nome
            for(int j=0; j<campi.size() && !flag_colonna_trovata; j++){
                if(_colonne[i]->getNomeColonna() == campi[j])
                    flag_colonna_trovata = true;
            }
            if(!flag_colonna_trovata)
                flag_campo_non_trovato = true;
        }
    }
    if(!flag_campo_non_trovato) { //se tutte le colonne marcate come not null sono presenti nei campi, posso procedere con l'aggiunta del record
        _recs++;
        for (auto &i : _colonne) {
            i->addDefault();
        }
        for (int i = 0; i < campi.size(); i++) {
            for (auto &j : _colonne) {
                if (campi[i] == j->getNomeColonna())
                    j->updateVal(valori[i], int(_recs - 1));
            }
        }
    }else{
        //eccezione campo non trovato
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

void Tabella::updateRecord(const string& condizione, const string& nome_col, const vector<string>& campi, const vector<string>& valori, int operatore){
    bool trovata=false;
    int i=0, j;
    while(i<_colonne.size() && !trovata){
        if(nome_col==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs; j++){
            if(_colonne[i]->compareElements(condizione, operatore, j)) {
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

void Tabella::updateRecord(const string& condizione1, const string& condizione2, const string& nome_col, const vector<string>& campi, const vector<string>& valori){
    bool trovata=false;
    int i=0, j;
    while(i<_colonne.size() && !trovata){
        if(nome_col==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs; j++){
            if(_colonne[i]->compareElements(condizione1, 4, j) && _colonne[i]->compareElements(condizione2, 2, j)) {
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

vector<string> Tabella::returnData(const vector<string> &campi, const string& campo_condizione, const string &condizione, int operatore) {
    vector<string> righe_testo;
    righe_testo.clear();
    string riga;
    bool trovata=false;
    int i=0;
    while(i<_colonne.size() && !trovata){
        if(campo_condizione==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(int j=0; j<_recs; j++){
            if(_colonne[i]->compareElements(condizione, operatore, j)) {
                riga.clear();
                for(auto elem : _colonne){
                    for(const auto & s : campi){
                        if(elem->getNomeColonna()==s) {
                            if(elem->getElement(j)==elem->getElement(-1)){
                                riga+="___ ";
                            }else {
                                riga += elem->getElement(j);
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

vector<string> Tabella::returnData(const vector<string> &campi, const string& campo_condizione, const string &condizione1, const string& condizione2) {
    vector<string> righe_testo;
    righe_testo.clear();
    string riga;
    bool trovata=false;
    int i=0;
    while(i<_colonne.size() && !trovata){
        if(campo_condizione==_colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(int j=0; j<_recs; j++){
            if(_colonne[i]->compareElements(condizione1, 4, j) && _colonne[i]->compareElements(condizione2, 2, j)) {
                riga.clear();
                for(auto elem : _colonne){
                    for(const auto & s : campi){
                        if(elem->getNomeColonna()==s) {
                            if(elem->getElement(j)==elem->getElement(-1)){
                                riga+="___ ";
                            }else {
                                riga += elem->getElement(j);
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