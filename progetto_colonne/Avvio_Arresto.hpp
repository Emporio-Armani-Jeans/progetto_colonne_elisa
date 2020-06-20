//
// Created by andry on 14/06/2020.
//

#ifndef PROGETTO_COLONNE_AVVIO_ARRESTO_HPP
#define PROGETTO_COLONNE_AVVIO_ARRESTO_HPP
#include "Tabella.h"
#include <fstream>
#include "Colonna.h"
#include "FileError.h"

vector<Tabella*> Avvio(const string& nome_file){
    ifstream database;
    database.open(nome_file);
    if(!database.is_open()){
        throw FileError();
    }else{
        int num_tabs, num_cols, num_recs, increment_value=0;
        string word, tipo, auto_increment, not_null, primary_key;
        Colonna *new_col= nullptr;
        Tabella *tabella= nullptr;
        vector<Tabella*> tabs;
        vector<string> campi, valori;
        char c;
        database >> num_tabs;
        for(int i=0; i<num_tabs; i++){
            //salvataggio della i-esima tabella
            database >> word;
            tabella = new Tabella(word);
            database >> num_cols;
            for(int i_col=0; i_col<num_cols; i_col++){
                getline(database, word, ':');
                if(i_col==0) word.erase(0,1);      // cancello il '\n'
              //  database >> c;
                getline(database, tipo, ',');
             //   database >> c;
                getline(database, auto_increment,',');
               // database >> c;
                getline(database, not_null,',');
              //  database >> c;
                getline(database, primary_key,'#');
              //  database >> c;
                if(tipo=="int"){
                    if(auto_increment=="true") new_col= new ColonnaInt(word, false, true, &increment_value);
                    else new_col= new ColonnaInt(word);
                }
                else if(tipo=="char"){
                    new_col= new ColonnaChar(word);
                }
                else if(tipo=="text"){
                    new_col= new ColonnaText(word);
                }
                else if(tipo=="float"){
                    new_col= new ColonnaFloat(word);
                }
                else if(tipo=="data"){
                    new_col= new ColonnaDate(word);
                }
                else if(tipo=="time"){
                    new_col= new ColonnaTime(word);
                }
                campi.push_back(new_col->getNomeColonna());
                tabella->aggiungiColonna(new_col);
                if(not_null=="true") new_col->setNotNull();
                if(primary_key=="true") tabella->setChiavePrimaria(new_col->getNomeColonna());
                new_col= nullptr;
            }
            //add records
            database >> num_recs;
            for(int j=0;j<num_recs; j++){
                for(int t=0; t<campi.size(); t++){
                    getline(database, word, '#');
                    if(t==0) word.erase(0,1);
                   // database >> c;
                    valori.push_back(word);
                }
                (*tabella).addRecord(campi, valori);
                valori.clear();
            }
            campi.clear();
            tabs.push_back(tabella);
            tabella = nullptr;
        }
        database.close();
        return tabs;
    }
}

void Arresto(const string& nome_file, const vector<Tabella*>& tabelle){
    ofstream database;
    database.open(nome_file);
    if(!database.is_open()){
        throw FileError();
    }else{
        Tabella* tab;
        database << tabelle.size() << endl;
        for(auto & t : tabelle) {
            tab = t;
            database << tab->getNome() << " " << tab->numCampi() << endl;
            for (int i = 0; i < tab->numCampi(); i++) {
                database << tab->getCol(i)->getNomeColonna() << ':' << tab->getCol(i)->getTipo();
                if (tab->getCol(i)->getTipo() == "int" && tab->getCol(i)->isAutoIncrement()) database << "true,";
                else database << "false,";
                if (tab->getCol(i)->isNotNull()) database << "true,";
                else database << "false,";
                if (tab->getCol(i)->isKey()) database << "true#";
                else database << "false#";
            }
            for(int i=0; i<tab->numRecs(); i++){
                for(int j=0; j<tab->numCampi(); j++){
                    database << tab->getCol(j)->getElement(i) << "#";
                }
                database << endl;
            }
            //memorizzo prima tabella in coda nella variabile, poi la cancello
            database << endl;
            delete tab;
            tab = nullptr;
        }
        database.close();
    }
}
#endif //PROGETTO_COLONNE_AVVIO_ARRESTO_HPP
