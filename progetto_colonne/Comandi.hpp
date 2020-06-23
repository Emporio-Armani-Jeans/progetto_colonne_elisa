//
// Created by andry on 21/06/2020.
//

#ifndef PROGETTO_COLONNE_COMANDI_HPP
#define PROGETTO_COLONNE_COMANDI_HPP
#include <vector>
#include "Tabella.h"
#include <string>
using namespace std;

bool belong_to(const string& elemento, const vector<string>& insieme){
    for(const auto & elem : insieme){
        if(elemento==elem) return true;
    }
    return false;
}

string toUpper(string word){
    for (auto & c : word)
        c = toupper(c);
    return word;
}

void Create(vector<Tabella*> &tabelle, stringstream &stream_comando, int *contatore, string *message) {
    string scarto, word, word2, word3, nome_tabella, nome_colonna, tipo, riga_comando, testo_chiavi;
    bool not_null = false, auto_increm = false;
    stringstream riga_temp;
    bool c;

    stream_comando >> scarto >> nome_tabella;
    if (nome_tabella[nome_tabella.size() - 1] == '(') {
        nome_tabella.pop_back();
    } else {
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
                word.erase(0, 1);
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
    (*message)="Tabella creata e aggiunta al database";
}


void Drop(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
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
        (*message)="Tabella eliminata dal database";
    }
}


void Truncate(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
    string word;
    if (!tabelle.empty()) {
        stream_comando >> word;      //butto via "TABLE"
        stream_comando >> word;       //in word ho nome_tab
        word.pop_back();   //rimuovo ';'
        for (Tabella* &s : tabelle) {
            if (toUpper(word) == toUpper(s->getNome())) {
                s->deleteRecord();
                (*message)="Tabella " + s->getNome() + "svuotata correttamente";
                break;
            }
        }
    } else
        cout << "Tabella non esistente" << endl;     //se non ho trovato tab con tale nome nel database
}


void Delete(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
    vector<string> operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    string word, word2, operatore;
    int a, b;
    bool trovata;
    stream_comando >> word;    //butto via "FROM"
    stream_comando >> word;
    //cerco match nome tabella
    for (a = 0, trovata = false; a < tabelle.size(); a++) {
        if (toUpper(tabelle[a]->getNome()) == word) {
            trovata = true;
            break;
        }
    }
    if (!trovata) {
        (*message)="Tabella non trovata";
    } else {   //a contiene indice della tabella scelta
        stream_comando >> word; //butto via "WHERE"
        stream_comando >> word;
        //cerco match con campo condizione
        for (b = 0, trovata = false; b < tabelle.size(); b++) {
            if (toUpper(tabelle[a]->getCol(b)->getNomeColonna()) == word) {
                trovata = true;
                break;
            }
        }
        if (!trovata) {
            (*message)="Campo condizione non esistente nella tabella scelta";
        } else {
            //leggo operatore; assumere che tra operatore e parole adiacenti ci sia almeno uno spazio
            stream_comando >> operatore;
            if (!(belong_to(operatore, operatori))) {
                (*message)="Operatore nella condizione non valido";
            } else {
                if (toUpper(operatore) == "BETWEEN") {
                    stream_comando >> word; //prima condizione
                    stream_comando >> word2; //butto via "AND"
                    stream_comando >> word2; //seconda condizione
                    if(word[0]==34 || word[0]==39) {
                        word.erase(0, 1);
                        word.pop_back();           //rimuovo le virgolette
                    }
                    if(word2[0]==34 || word2[0]==39) {
                        word2.erase(0, 1);
                        word2.pop_back();           //rimuovo le virgolette
                    }
                    word2.pop_back();
                    tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, word2);
                }else {
                    stream_comando >> word;
                    if (word[0] == 34 || word[0] == 39) {
                        word.erase(0, 1);
                        word.pop_back();
                    }
                    word.pop_back();
                    //rimuovo virgolette e punto e virgola finale
                    if (operatore == "=") {
                        tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word);
                    } else if (operatore == "<") {
                        tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 1);
                    } else if (operatore == "<=") {
                        tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 2);
                    } else if (operatore == ">") {
                        tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 3);
                    } else if (operatore == ">=") {
                        tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 4);
                    } else if (operatore == "<>") {
                        tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 5);
                    }
                }
                (*message)="Record eliminati";
            }
        }
    }
}

void Update(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
    string scarto, word, word2, word3, condizione1, condizione2;
    vector<string> campi, valori, operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    int a;
    bool trovata;
    stream_comando >> word;
    for (a = 0, trovata = false; a < tabelle.size(); a++) {
        if (toUpper(tabelle[a]->getNome()) == word) {
            trovata = true;
            break;
        }
    }
    if(!trovata){
        (*message)="Tabella non trovata nel database";
    }else {
        stream_comando >> scarto; //scarto SET
        stream_comando >> word;
        while (toUpper(word) != "WHERE") {
            campi.push_back(word);
            stream_comando >> scarto;   //operatore è sempre '='
            stream_comando >> word2;   //valore
            if(word2[word2.size()-1]==',') {
                word2.pop_back();   //rimuovo virgola
            }
            if (word2[0] == 34 || word2[0]==39) {
                word2.pop_back();
                word2.erase(0, 1);
            }
            valori.push_back(word2);
            stream_comando >> word;
        }    //ho opportunamente riempito campi e valori
        stream_comando >> word; //in word c'è campo condizione
        stream_comando >> word2; //operatore
        if(!belong_to(word2, operatori)){
            (*message)="Operatore non valido";
        }else {
            if (toUpper(word2) == "BETWEEN") {
                stream_comando >> condizione1;
                if (condizione1[0] == 34 || condizione1[0] == 39) {
                    condizione1.erase(0, 1);
                    condizione1.pop_back();
                }
                stream_comando >> scarto; //scarto AND
                stream_comando >> condizione2;
                condizione2.pop_back();  //rimuovo ';'
                if (condizione2[0] == 34 || condizione2[0] == 39) {
                    condizione2.erase(0, 1);
                    condizione2.pop_back();
                }
                tabelle[a]->updateRecord(word, condizione1, condizione2, campi, valori);
            } else {
                stream_comando >> word3; //condizione
                word3.pop_back();  //rimuovo ';'
                if (word3[0] == 34 || word3[0] == 39) {
                    word3.erase(0, 1);
                    word3.pop_back();
                }
                if (word2 == "=") {
                    tabelle[a]->updateRecord(word, word3, campi, valori);
                } else if (word2 == "<") {
                    tabelle[a]->updateRecord(word, word3, campi, valori, 1);
                } else if (word2 == "<=") {
                    tabelle[a]->updateRecord(word, word3, campi, valori, 2);
                } else if (word2 == ">") {
                    tabelle[a]->updateRecord(word, word3, campi, valori, 3);
                } else if (word2 == ">=") {
                    tabelle[a]->updateRecord(word, word3, campi, valori, 4);
                } else if (word2 == "<>") {
                    tabelle[a]->updateRecord(word, word3, campi, valori, 5);
                }
            }
        }
    }
}

#endif //PROGETTO_COLONNE_COMANDI_HPP
