#include <iostream>
#include "ColonnaText.h"
#include "ColonnaFloat.h"
#include "ColonnaInt.h"
#include "Tabella.h"
#include "FileError.h"
#include <list>
#include <vector>
#include <fstream>

using namespace std;


void deleteOggettoTabella(Tabella **ptr){
    if (*ptr != nullptr){
        delete *ptr;
        *ptr = nullptr;
    }
}
list<Tabella*> Avvio(const string& nome_file);                     ////
void Arresto(const string& nome_file, list<Tabella*> tabelle);                   ////

int main() {
    list<Tabella*> tabelle;
    int increment=0;

    return 0;
}


list<Tabella*> Avvio(const string& nome_file){                ////
    ifstream database;
    database.open(nome_file);
    if(!database){
        throw FileError();
    }else{
        int num_tabs, num_cols, numero;
        string parola, tipo, auto_increment, not_null, primary_key;
        Colonna* colonna, *new_col;
        Tabella* tabella;
        list<Tabella*> tabs;
        database >> num_tabs >> num_cols;
        for(int i=0; i<num_tabs; i++){
            //salvataggio della i-esima tabella
            database >> parola;
            tabella= new Tabella("parola");
            for(int i_col=0; i_col<num_cols; i_col++){        //da terminare
                getline(database, parola, ':');
                getline(database, tipo, ',');
                getline(database, auto_increment,',');
                getline(database, not_null,',');
                getline(database, primary_key,'#');
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
                tabella->aggiungiColonna(new_col);
                if(not_null=="true") new_col->setNotNull();
                if(primary_key=="true") tabella->setChiavePrimaria(new_col->getNomeColonna());
                new_col= nullptr;
            }
            //add record
        }
        database.close();
        return tabs;
    }
}
