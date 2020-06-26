//
// Created by andry on 21/06/2020.
//

#ifndef PROGETTO_COLONNE_COMANDI_HPP
#define PROGETTO_COLONNE_COMANDI_HPP
#include <vector>
#include "Tabella.h"
#include "InexistentTable.h"
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

    stream_comando >> nome_colonna >> tipo;
    while (toUpper(tipo) != "KEY") {
        getline(stream_comando, riga_comando, ',');
        riga_temp << riga_comando;
        if (toUpper(tipo) == "INT") {
            //se è auto increment mi servono delle stringhe in più
            riga_temp >> word2;
            if (toUpper(word2) == "NOT") { //not null
                riga_temp >> word3;
                word = word2 + " " + word3;
                if (toUpper(word) == "NOT NULL") {
                    not_null = true;
                    getline(riga_temp, word, ',');
                    word.erase(0, 1);
                    if (toUpper(word) == "AUTO_INCREMENT") {
                        auto_increm = true;
                        not_null = false;
                    }
                }
            } else {//auto_increment
                auto_increm = true;
                not_null = false;
                if (!riga_temp.eof()) {
                    riga_temp >> scarto >> scarto; //scarto not null;
                }
            }
        } else { //se non è int controllo solo not null
            riga_temp >> word2;
            riga_temp >> word3;
            word2 += " " + word3;
            if (toUpper(word2) == "NOT NULL")
                not_null = true;
        }
                //aggiungo la colonna, casistica tipo per tipo
                if (toUpper(tipo) == "INT") {
                    if (auto_increm) {
                        tabelle[tabelle.size() - 1]->aggiungiColonna(
                                new ColonnaInt(nome_colonna, false, true, contatore));
                    } else {
                        if (not_null) {
                            tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaInt(nome_colonna, true));
                        } else
                            tabelle[tabelle.size() - 1]->aggiungiColonna(
                                    new ColonnaInt(nome_colonna));
                    }
                } else if (toUpper(tipo) == "TEXT") {
                    if (!not_null)
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaText(nome_colonna));
                    else
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaText(nome_colonna, true));
                } else if (toUpper(tipo) == "CHAR") {
                    if (!not_null)
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaChar(nome_colonna));
                    else
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaChar(nome_colonna, true));
                } else if (toUpper(tipo) == "DATE") {
                    if (!not_null)
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaDate(nome_colonna));
                    else
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaDate(nome_colonna, true));
                } else if (toUpper(tipo) == "FLOAT") {
                    if (!not_null)
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaFloat(nome_colonna));
                    else
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaFloat(nome_colonna, true));
                } else if (toUpper(tipo) == "TIME") {
                    if (!not_null)
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaTime(nome_colonna));
                    else
                        tabelle[tabelle.size() - 1]->aggiungiColonna(new ColonnaTime(nome_colonna, true));
                }

                riga_temp.clear();
                stream_comando >> nome_colonna >> tipo;
    }


    getline(stream_comando, testo_chiavi, ';');
    testo_chiavi.pop_back();  //cancella )
    testo_chiavi.pop_back();  //cancella spazio
    stringstream stream_chiavi(testo_chiavi);
    riga_comando.clear();
    getline(stream_chiavi, riga_comando,')');
    while (!riga_comando.empty()){ //gestione chiavi primaria ed esterna
        if (toUpper(nome_colonna) == "PRIMARY"){ //primary key
            riga_comando.erase(0,2); //tolgo lo spazio all'inizio e la (
            tabelle[tabelle.size() - 1]->setChiavePrimaria(riga_comando);
        }
        else { //foreign key
            riga_comando.erase(0,2); //tolgo lo spazio e la ( all'inizio
            //in riga_comando adesso ho il campo della mia tabella
            stream_chiavi >> scarto >> nome_tabella; //in nome tabella ho la tabella esterna
            int pos;
            bool tabella_trovata;
            for (int i = 0; i < tabelle.size(); i++) {
                if (tabelle[i]->getNome() == nome_tabella){
                    tabella_trovata= true;
                    pos = i;
                }
            }
            if(tabella_trovata){
                getline(stream_chiavi, scarto, '(');
                getline(stream_chiavi, word, ')');
                tabelle[tabelle.size() - 1]->setChiaveEsterna(tabelle[pos], riga_comando, word);
            }
            else {
                throw InexistentTable();
            }
        }
        riga_comando.clear();
        stream_chiavi >> nome_colonna >> tipo;
        getline(stream_chiavi, riga_comando,')');
    }

    (*message)="Operazione completata: tabella creata e aggiunta al database.";
}

void Drop(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
    string word;
    bool trovata;
    vector<Tabella*>::iterator it = tabelle.begin();
    stream_comando >> word;      //butto via "TABLE"
    stream_comando >> word;
    word.pop_back();            //butto via ';'
    //ricerco match nome tabella
    for (trovata = false; it != tabelle.end(); it++) {
        if ((*it)->getNome() == word) {
            trovata = true;
            break;
        }
    }
    if (!trovata) {
        throw InexistentTable();
    } else {
        tabelle.erase(it);
        (*message) = "Operazione completata: tabella eliminata dal database.";
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

void Select(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
    string word, scarto, word2, word3, nome_colonna, condizione1, condizione2, ordine;
    int a, b;
    bool trovata, ordinamento;
    vector<string> campi, operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    stream_comando >> word;  //leggo seconda parola
    if (word == "*") {   //seleziono tutte le colonne
        stream_comando >> scarto;  //scarto FROM
        stream_comando >> word;   //nome tab
        if (word[word.size() - 1] == ';') {
            word.pop_back();
            for (a = 0, trovata = false; a < tabelle.size(); a++) {
                if (toUpper(tabelle[a]->getNome()) == word) {
                    trovata = true;
                    break;
                }
            }
            if (trovata) {
                //stampo tutta la tabella
                for (const string &elem : tabelle[a]->returnData()) {
                    if (elem.empty()) cout << "Riga vuota" << endl;
                    else cout << elem << endl;
                }
                cout << endl;
            }else {
                (*message) = "Tabella non trovata";
            }
        }else{
            for (a = 0, trovata = false; a < tabelle.size(); a++) {
                if (toUpper(tabelle[a]->getNome()) == word) {
                    trovata = true;
                    break;
                }
            }
            stream_comando >> scarto;  //se il comando non è concluso ci sarà WHERE
            stream_comando >> word; //campo condizione
            stream_comando >> word2; //operatore
            stream_comando >> word3; //condizione1
            if(word3[word3.size()-1]==';'){     //non c'è ordinamento
                word3.pop_back();
            }
        }
    } else {
        while (toUpper(word) !=
               "FROM") {   //memorizzo campi finchè non incontro from, suppongo spazio dopo la virgola
            word.pop_back(); //rimuovo virgola
            campi.push_back(word);
            stream_comando >> word;
        }

        stream_comando >> word;
        //cerco match tabella in questione
        for (a = 0, trovata = false; a < tabelle.size(); a++) {
            if (toUpper(tabelle[a]->getNome()) == word) {
                trovata = true;
                break;
            }
        }
        if (!trovata) {
            (*message)="Tabella non trovata";
        } else {
            if (word[word.size() - 1] == ';') {   //non c'è WHERE
                word.pop_back();
                //stampo solo campi specificati della tabella
                for (const auto &elem : tabelle[a]->returnData(campi)) {
                    if(elem.empty()) cout << "Riga vuota" << endl;
                    else cout << elem << endl;
                }
                cout << endl;
            } else {
                stream_comando >> scarto; //butto via "WHERE"
                stream_comando >> word;  //in word c'è il campo condizione
                stream_comando >> word2;  //in word2 c'è l'operatore
                if (!belong_to(word2, operatori)) {
                    cout << "Operatore non valido" << endl;
                } else {
                    //casistica operatori
                    if(toUpper(word2)=="BETWEEN"){
                        stream_comando >> condizione1;
                        if (condizione1[0] == '"') {          //se ci sono virgolette le tolgo
                            condizione1.erase(0, 1);
                            condizione1.pop_back();
                        }
                        stream_comando >> scarto; //scarto and
                        stream_comando >> condizione2;
                        if (condizione2[condizione2.size()] != ';') {
                            if (condizione2[0] == '"') {          //se ci sono virgolette le tolgo
                                condizione2.erase(0, 1);
                                condizione2.pop_back();
                            }
                            stream_comando >> scarto;  //scarto order
                            stream_comando >> scarto; //scarto by
                            stream_comando >> nome_colonna;   //campo ordinamento
                            stream_comando >> ordine; //DESC o ASC
                            ordine.pop_back();
                            if(toUpper(ordine)=="DESC") {
                                for (int z = 0;
                                     z < tabelle[b]->returnData(campi, word2, condizione1, condizione2,
                                                                nome_colonna, 1).size(); z++) {
                                    cout << tabelle[b]->returnData(campi, word2, condizione1, condizione2,
                                                                   nome_colonna,
                                                                   1)[z] << endl;
                                    if(tabelle[b]->returnData(campi, word2, condizione1, condizione2, nome_colonna, 1).empty())
                                        cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                }
                            }
                            else if(toUpper(ordine)=="ASC"){
                                for (int z = 0;
                                     z < tabelle[b]->returnData(campi, word2, condizione1, condizione2,
                                                                nome_colonna, 2).size(); z++) {
                                    cout << tabelle[b]->returnData(campi, word2, condizione1, condizione2,
                                                                   nome_colonna,
                                                                   2)[z] << endl;
                                    if(tabelle[b]->returnData(campi, word2, condizione1, condizione2, nome_colonna, 2).empty())
                                        cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                }
                            }
                        } else {
                            condizione2.pop_back();
                            //stampo campi che rispettando condizione senza ordinamento
                            for (const auto &elem : tabelle[a]->returnData(campi, word, condizione1,
                                                                           condizione2)) {
                                cout << elem << " ";
                            }
                            if(tabelle[a]->returnData(campi, word, condizione1, condizione2).empty())
                                cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                            cout << endl;
                        }
                    }
                    else{
                        stream_comando >> word3; //in word3 c'è la condizione
                        if (word3[word3.size() - 1] != ';') {   //se non c'è ';' ci sarà ordinamento
                            ordinamento=true;
                            if (word3[0] == '"') {
                                word3.erase(0, 1);
                                word3.pop_back();
                            }
                            stream_comando >> scarto;  //scarto order
                            stream_comando >> scarto; //scarto by
                            stream_comando >> nome_colonna;   //campo ordinamento
                            stream_comando >> ordine; //DESC o ASC
                            ordine.pop_back();
                        } else {
                            ordinamento=false;
                            word3.pop_back();
                            //stampo campi che rispettando condizione senza ordinamento
                        }
                        if (word2 == "=") {
                            if(ordinamento) {
                                if(toUpper(ordine)=="DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 0, nome_colonna,
                                                                1).size(); z++) {
                                         cout << tabelle[b]->returnData(campi, word, word3, 0, nome_colonna, 1)[z]
                                             << endl;
                                         if(tabelle[b]->returnData(campi, word, word3, 0, nome_colonna, 1).empty())
                                             cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }
                                /*else if(toUpper(ordine)=="ASC"){
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 0, nome_colonna,
                                                                2).size(); z++) {
                                         cout << tabelle[b]->returnData(campi, word, word3, 0, nome_colonna, 2)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 0, nome_colonna, 2).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }*/
                            }else{
                                for (const auto &elem : tabelle[a]->returnData(campi, word, word3)) {
                                    cout << elem << " ";
                                }
                                if(tabelle[a]->returnData(campi, word, word3).empty())
                                    cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                cout << endl;
                            }
                        } else if (word2 == "<") {
                            if(ordinamento) {
                                if(toUpper(ordine)=="DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 1, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 1, nome_colonna, 1)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word2, word3, 1, nome_colonna, 1).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }
                                /*else if(toUpper(ordine)=="ASC"){
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 1, nome_colonna,
                                                                2).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 1, nome_colonna, 2)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 1, nome_colonna, 2).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }*/
                            }else{
                                for (const auto &elem : tabelle[a]->returnData(campi, word, word3)) {
                                    cout << elem << " ";
                                }
                                if(tabelle[a]->returnData(campi, word, word3).empty())
                                    cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                cout << endl;
                            }
                        } else if (word2 == "<=") {
                            if(ordinamento) {
                                if(toUpper(ordine)=="DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 2, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 2, nome_colonna, 1)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 2, nome_colonna, 1).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }
                                /*else if(toUpper(ordine)=="ASC"){
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 2, nome_colonna,
                                                                2).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 2, nome_colonna, 2)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 2, nome_colonna, 2).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }*/
                            }else{
                                for (const auto &elem : tabelle[a]->returnData(campi, word, word3)) {
                                    cout << elem << " ";
                                }
                                if(tabelle[a]->returnData(campi, word, word3).empty())
                                    cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                cout << endl;
                            }
                        } else if (word2 == ">") {
                            if(ordinamento) {
                                if(toUpper(ordine)=="DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 3, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 3, nome_colonna, 1)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 3, nome_colonna, 1).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }
                                /*else if(toUpper(ordine)=="ASC"){
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 3, nome_colonna,
                                                                2).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 3, nome_colonna, 2)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 3, nome_colonna, 2).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }*/
                            }else{
                                for (const auto &elem : tabelle[a]->returnData(campi, word, word3)) {
                                    cout << elem << " ";
                                }
                                if(tabelle[a]->returnData(campi, word, word3).empty())
                                    cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                cout << endl;
                            }
                        } else if (word2 == ">=") {
                            if(ordinamento) {
                                if(toUpper(ordine)=="DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 4, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 4, nome_colonna, 1)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 4, nome_colonna, 1).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }
                                /*else if(toUpper(ordine)=="ASC"){
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 4, nome_colonna,
                                                                2).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 4, nome_colonna, 2)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 4, nome_colonna, 2).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }*/
                            }else{
                                for (const auto &elem : tabelle[a]->returnData(campi, word, word3)) {
                                    cout << elem << " ";
                                }
                                if(tabelle[a]->returnData(campi, word, word3).empty())
                                    cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                cout << endl;
                            }
                        } else if (word2 == "<>") {
                            if(ordinamento) {
                                if(toUpper(ordine)=="DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 5, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 5, nome_colonna, 1)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 5, nome_colonna, 1).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }
                                /*else if(toUpper(ordine)=="ASC"){
                                    for (int z = 0;
                                         z <
                                         tabelle[b]->returnData(campi, word, word3, 5, nome_colonna,
                                                                2).size(); z++) {
                                        cout << tabelle[b]->returnData(campi, word, word3, 5, nome_colonna, 2)[z]
                                             << endl;
                                        if(tabelle[b]->returnData(campi, word, word3, 5, nome_colonna, 2).empty())
                                            cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                    }
                                }*/
                            }else{
                                for (const auto &elem : tabelle[a]->returnData(campi, word, word3)) {
                                    cout << elem << " ";
                                }
                                if(tabelle[a]->returnData(campi, word, word3).empty())
                                    cout << "Non ci sono valori che rispettano i criteri di ricerca" << endl;
                                cout << endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Insert(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message) {
    //INSERIRE NUOVO RECORD
    string scarto, word, nome_tabella;
    vector<string> campi, valori;
    int a;
    bool trovata;
    /*INSERT INTO CUSTOMERS (AGE, ADDRESS, NAME)
    VALUES (20, “via Roma 10, Torino”, “Francesco Rossi”);*/
    stream_comando >> scarto;
    stream_comando >> nome_tabella;
    for (a = 0, trovata = false; a < tabelle.size(); a++) {
        if (toUpper(tabelle[a]->getNome()) == nome_tabella) {
            trovata = true;
            break;
        }
    }
    if(!trovata){
        (*message)="Tabella non trovata nel database";
    }else {
        getline(stream_comando, scarto, '(');
        //salvo n-1 campi
        stream_comando >> word;
        while (word[word.size()-1] != ')') {
            word.pop_back();
            campi.push_back(word);
            stream_comando >> word;
        }
        //leggo salvo l'ultimo campo
        word.pop_back();
        campi.push_back(word);

        stream_comando >> scarto;
        getline(stream_comando, scarto, '(');

        //salvo n-1 valori
        stream_comando >> word;
        while (word[word.size()-1] != ')') {
            word.pop_back();
            if (word[0] == '"') {
                word.pop_back();
                word.erase(0,1);
            }
            valori.push_back(word);
            stream_comando >> word;
        }
        word.pop_back();
        if (word[0] == '"') {
            word.pop_back();
            word.erase(0,1);
        }
        valori.push_back(word);
        tabelle[a]->addRecord(campi, valori);

        (*message)="Record aggiunto correttamente alla tabella";
    }
}


#endif //PROGETTO_COLONNE_COMANDI_HPP
