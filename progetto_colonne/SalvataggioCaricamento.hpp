//
// Created by andry on 14/06/2020.
//

#ifndef PROGETTO_COLONNE_SALVATAGGIOCARICAMENTO_HPP
#define PROGETTO_COLONNE_SALVATAGGIOCARICAMENTO_HPP
#include "Tabella.h"
#include <fstream>
#include "Colonna.h"
#include "FileError.h"
#include "Comandi.hpp"



vector<Tabella*> Caricamento(const string& nome_file){
    ifstream database;
    database.open(nome_file);
    if(!database.is_open()){
        throw FileError();
    }else{
        typedef struct infoKEsterna{
            vector<string> colonne_madri, tabelle_madri;  //indicizzazione riferita alla colonna figlia
        }infoKEsterna;
        infoKEsterna tmp;
        vector<infoKEsterna> infos;
        int num_tabs=0, num_cols=0, num_recs=0;
        string word, tipo, auto_increment, not_null, primary_key, tabella_madre, colonna_madre, colonna_figlia;
        Colonna *new_col= nullptr;
        Tabella *temp = nullptr;
        vector<Tabella*> tabs;
        vector<string> campi, valori;
        database >> num_tabs;
        for(int i=0; i<num_tabs; i++){
            //salvataggio della i-esima tabella
            database >> word;
            tabs.emplace_back(new Tabella(word));
            database >> num_cols;
            for(int i_col=0; i_col<num_cols; i_col++){
                getline(database, word, '<');
                if(i_col==0) word.erase(0,1);      // cancello il '\n'
                getline(database, tabella_madre, '<');
                getline(database, colonna_madre, ':');
                tmp.colonne_madri.push_back(colonna_madre);
                tmp.tabelle_madri.push_back(tabella_madre);
                getline(database, tipo, ',');
                getline(database, auto_increment,',');
                getline(database, not_null,',');
                getline(database, primary_key,'#');
                if(tipo=="int"){
                    if(auto_increment=="true") {
                        new_col= new ColonnaInt(word, false, true);
                    }
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
                if(not_null=="true") {
                    if (toUpper(tipo) != "INT" || (toUpper(tipo) == "INT" && !new_col->isAutoIncrement())) {
                        new_col->setNotNull();
                    }
                }
                campi.push_back(new_col->getNomeColonna());
                tabs[i]->aggiungiColonna(new_col);
                if(primary_key=="true") tabs[i]->setChiavePrimaria(new_col->getNomeColonna());
                new_col= nullptr;
            }
            infos.push_back(tmp);
            tmp.tabelle_madri.clear();
            tmp.colonne_madri.clear();
            //add records
            database >> num_recs;
            for(int j=0;j<num_recs; j++){
                for(int t=0; t<campi.size(); t++){
                    getline(database, word, '#');
                    if(j==0 && t==0) word.erase(0,1);
                    valori.push_back(word);
                }
                tabs[i]->addRecordMemory(campi, valori);  //metodo alternativo per non tener conto di auto_increment
                valori.clear();
            }
            campi.clear();
        }
        //ho in infos in ogni elemento informazioni su link esterni per ogni colonna della tabella
        for(int i=0; i< num_tabs; i++){
            for(int j=0; j< tabs[i]->numCampi(); j++){
                if(infos[i].tabelle_madri[j]!="#" && infos[i].colonne_madri[j]!="#") {
                    //mi serve un puntatore alla tabella che matcha col nome presente nelle infos
                    for (int a = 0; a < num_tabs; a++) {
                        if (infos[i].tabelle_madri[j] == tabs[a]->getNome()) {
                            temp = tabs[a];
                            break;
                        }
                    }
                    tabs[i]->setChiaveEsterna(temp, tabs[i]->getCol(j)->getNomeColonna(), infos[i].colonne_madri[j]);
                }
            }
        }
        database.close();
        return tabs;
    }

}

void Salvataggio(const string& nome_file, const vector<Tabella*> &tabelle){
    ofstream database;
    database.open(nome_file);
    if(!database.is_open()){
        throw FileError();
    }else{
        Tabella *tab;
        database << tabelle.size() << endl;
        for(auto & t : tabelle) {
            database << endl;
            tab = t;
            database << tab->getNome() << " " << tab->numCampi() << endl;
            for (int i = 0; i < tab->numCampi(); i++) {
                database << tab->getCol(i)->getNomeColonna() << '<';
                if(!tab->getCol(i)->getTabMadre().empty())
                    database << tab->getCol(i)->getTabMadre() << '<';
                else database << "#<";
                if(!tab->getCol(i)->getForeignKey().empty())
                    database << tab->getCol(i)->getForeignKey() << ':';
                else database << "#:";
                database << tab->getCol(i)->getTipo() << ",";
                if (tab->getCol(i)->getTipo() == "int" && tab->getCol(i)->isAutoIncrement()) database << "true,";
                else database << "false,";
                if (tab->getCol(i)->isNotNull() && !tab->getCol(i)->isAutoIncrement()) {
                    database << "true,";
                }
                else database << "false,";
                if (tab->getCol(i)->isKey()) database << "true#";
                else database << "false#";
            }
            database << endl;
            database << tab->numRecs() << endl;
            for(int i=0; i<tab->numRecs(); i++){
                for(int j=0; j<tab->numCampi(); j++){
                    database << tab->getCol(j)->getElement(i) << "#";
                }
            }
            //memorizzo prima tabella in coda nella variabile, poi la cancello
            database << endl;
            //delete tab;
        }
        database.close();
    }
}


#endif //PROGETTO_COLONNE_SALVATAGGIOCARICAMENTO_HPP
