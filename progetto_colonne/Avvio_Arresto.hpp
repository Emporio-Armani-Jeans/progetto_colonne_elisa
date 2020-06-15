//
// Created by andry on 14/06/2020.
//

#ifndef PROGETTO_COLONNE_AVVIO_ARRESTO_HPP
#define PROGETTO_COLONNE_AVVIO_ARRESTO_HPP
#include "Tabella.h"
#include <fstream>
#include "Colonna.h"
#include "FileError.h"

list<Tabella*> Avvio(const string& nome_file){
    ifstream database;
    database.open(nome_file);
    if(!database.is_open()){
        throw FileError();
    }else{
        int num_tabs, num_cols, num_recs;
        string parola, tipo, auto_increment, not_null, primary_key;
        Colonna* colonna, *new_col;
        Tabella* tabella;
        list<Tabella*> tabs;
        vector<string> campi, valori;
        char c;
        database >> num_tabs >> num_cols;
        for(int i=0; i<num_tabs; i++){
            //salvataggio della i-esima tabella
            database >> parola;
            tabella= new Tabella("parola");
            for(int i_col=0; i_col<num_cols; i_col++){
                getline(database, parola, ':');
                database >> c;
                getline(database, tipo, ',');
                database >> c;
                getline(database, auto_increment,',');
                database >> c;
                getline(database, not_null,',');
                database >> c;
                getline(database, primary_key,'#');
                database >> c;
                if(tipo=="int"){
                    if(auto_increment=="true") new_col= new ColonnaInt("parola", false, true);
                    else new_col= new ColonnaInt("parola");
                }
                else if(tipo=="char"){
                    new_col= new ColonnaChar("parola");
                }
                else if(tipo=="text"){
                    new_col= new ColonnaText("parola");
                }
                else if(tipo=="float"){
                    new_col= new ColonnaFloat("parola");
                }
                else if(tipo=="data"){
                    new_col= new ColonnaDate("parola");
                }
                else if(tipo=="time"){
                    new_col= new ColonnaTime("parola");
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
                    getline(database,parola,'#');
                    database >> c;
                    valori.push_back(parola);
                }
                tabella->addRecord(campi, valori);
                valori.clear();
            }
        }
        database.close();
        return tabs;
    }
}

void Arresto(const string& nome_file, list<Tabella*> tabelle){
    ofstream database;
    database.open(nome_file);
    if(!database.is_open()){
        throw FileError();
    }else{
        Tabella* tab;
        database << tabelle.size() << endl;
        for(int t=0; t<tabelle.size(); t++) {
            tab = tabelle.front();
            tabelle.pop_front();        //memorizzo prima tabella in coda nella variabile, poi la tolgo dalla lista
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
            database << endl;
            delete tab;
            tab = nullptr;
        }
        database.close();
    }
}
#endif //PROGETTO_COLONNE_AVVIO_ARRESTO_HPP
