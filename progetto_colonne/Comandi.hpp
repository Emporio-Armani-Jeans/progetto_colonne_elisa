//
// Created by andry on 21/06/2020.
//

#ifndef PROGETTO_COLONNE_COMANDI_HPP
#define PROGETTO_COLONNE_COMANDI_HPP
#include <vector>
#include "Tabella.h"
#include <string>
using namespace std;

string toUpper(string word){
    for (auto & c : word)
        c = toupper(c);
    return word;
}

void Create(vector<Tabella*> &tabelle, stringstream &stream_comando, int *contatore){
    string scarto, word, word2, word3, nome_tabella, nome_colonna, tipo, riga_comando, testo_chiavi;
    bool not_null=false, auto_increm=false;
    stringstream riga_temp;
    bool c;

    stream_comando >> scarto >> nome_tabella;
    if(nome_tabella[nome_tabella.size()-1]=='('){
        nome_tabella.pop_back();
    }else{
        stream_comando >> scarto;
    }
    tabelle.emplace_back(new Tabella(nome_tabella));
    getline(stream_comando, riga_comando, ',');
    riga_temp << riga_comando;
    riga_temp >> nome_colonna >> tipo;
        while (tipo != "KEY") {
            if (toUpper(tipo) == "INT") {
                //se è auto increment mi servono delle stringhe in più
                riga_temp >> word2;
                riga_temp >> word3;
                word = word2 + " " + word3;        //voglio mantenere word2, quindi non uso +=
                if (word == "NOT NULL") {
                    not_null = true;
                    getline(riga_temp, word, ',');
                    word.erase(0,1);
                    if (word == "AUTO_INCREMENT") {
                        auto_increm = true;
                        not_null = false;
                    }
                } else if (word2 == "AUTO_INCREMENT") {
                    auto_increm = true;
                    not_null = false;
                }
            } else {
                //se non è int controllo solo not null
                riga_temp >> word2;
                riga_temp >> word3;
                word2 += " " + word3;
                if (word2 == "NOT NULL") not_null = true;
            }
            //aggiungo la colonna, casistica tipo per tipo
            if (tipo == "INT") {
                if (auto_increm) {
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaInt(nome_colonna, false, true, contatore));
                } else {
                    if (not_null) {
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaInt(nome_colonna, true));
                    } else
                        tabelle[tabelle.size() - 1]->aggiungiColonna(
                                new ColonnaInt(nome_colonna));
                }
            } else if (tipo == "TEXT") {
                if (!not_null)
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaText(nome_colonna));
                else
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaInt(nome_colonna, true));
            } else if (tipo == "CHAR") {
                if (!not_null)
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaChar(nome_colonna));
                else
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaChar(nome_colonna, true));
            } else if (tipo == "DATE") {
                if (!not_null)
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaDate(nome_colonna));
                else
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaDate(nome_colonna, true));
            } else if (tipo == "FLOAT") {
                if (!not_null)
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaFloat(nome_colonna));
                else
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaFloat(nome_colonna, true));
            } else if (tipo == "TIME") {
                if (!not_null)
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaTime(nome_colonna));
                else
                    tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaTime(nome_colonna, true));
            }
            getline(stream_comando, riga_comando, ',');
            riga_temp.clear();
            riga_temp << riga_comando;
            riga_temp >> nome_colonna >> tipo;
        }
      //  testo_chiavi += nome_colonna + " " + tipo;
        getline(stream_comando, riga_comando, ';');
        testo_chiavi += " " + riga_comando;
        testo_chiavi.pop_back();  //cancella ;
        testo_chiavi.pop_back();  //cancella )
        testo_chiavi.push_back(',');
        stream_comando.clear();
        stream_comando << testo_chiavi;
        getline(stream_comando, riga_comando, ',');
        riga_temp.clear();
        riga_temp << riga_comando;
        while (!stream_comando.eof()) {
            if (nome_colonna == "PRIMARY") {   //PRIMARY KEY (ID) ,
                getline(riga_temp, scarto, '(');
                getline(riga_temp, word, ')');
                tabelle[tabelle.size() - 1]->setChiavePrimaria(word);
            } else if (nome_colonna == "FOREIGN") {
                int i;
                //FOREIGN KEY (COUNTRY_ID) REFERENCES COUNTRIES (ID) ,
                getline(riga_temp, scarto, '(');
                getline(riga_temp, word, ')');
                riga_temp << scarto;
                riga_temp << nome_tabella;
                for (i = 0; i < tabelle.size(); i++) {
                    if (tabelle[i]->getNome() == nome_tabella)
                        break;
                }
                getline(riga_temp, scarto, '(');
                getline(riga_temp, word2, ')');
                tabelle[tabelle.size() - 1]->setChiaveEsterna(tabelle[i], word2, word);
            }
            getline(stream_comando, riga_comando, ',');
            riga_temp.clear();
            riga_temp << riga_comando;
            riga_temp >> nome_colonna >> tipo;
        }
}

void deleteOggettoTabella(Tabella **ptr){
    if (*ptr != nullptr){
        delete *ptr;
        *ptr = nullptr;
    }
}

void Drop(vector<Tabella*> &tabelle, stringstream &stream_comando){
    string word;
    bool trovata;
    vector<Tabella*>::iterator it=tabelle.begin();
    stream_comando >> word;      //butto via "TABLE"
    stream_comando >> word;
    word.pop_back();            //butto via ';'
    //ricerco match nome tabella
    for (trovata = false; it != tabelle.end(); it++) {
        if (toUpper((*it)->getNome()) == word) {
            trovata = true;
            break;
        }
    }
    if (!trovata) {
        cout << "Tabella non esistente" << endl;
    } else {
        tabelle.erase(it);
    }
}


void Truncate(vector<Tabella*> &tabelle, stringstream &stream_comando){
    string word;
    if (!tabelle.empty()) {
        stream_comando >> word;      //butto via "TABLE"
        stream_comando >> word;       //in word ho nome_tab
        word.pop_back();   //rimuovo ';'
        for (Tabella* &s : tabelle) {
            if (toUpper(word) == toUpper(s->getNome())) {
                s->deleteRecord();
                break;
            }
        }
    } else
        cout << "Tabella non esistente" << endl;     //se non ho trovato tab con tale nome nel database
}

#endif //PROGETTO_COLONNE_COMANDI_HPP
