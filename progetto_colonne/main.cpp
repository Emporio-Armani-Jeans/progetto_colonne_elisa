#include <iostream>
#include "ColonnaText.h"
#include "ColonnaFloat.h"
#include "ColonnaInt.h"
#include "Tabella.h"
#include "FileError.h"
#include <list>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Avvio_Arresto.hpp"
#include "ControlloSintassi.h"
#define ERR_COMANDO -1

using namespace std;


void deleteOggettoTabella(Tabella **ptr){
    if (*ptr != nullptr){
        delete *ptr;
        *ptr = nullptr;
    }
}

enum comando {CREATE, DROP, INSERT, DELETE, TRUNCATE, UPDATE, SELECT,QUIT};


int compare_first_word_comandi(string &first_word);
string toUpper(string word);
bool belong_to(const string& elemento, const vector<string>& insieme);


int main() {
    ControlloSintassi controllore;
    vector<Tabella*> tabelle;
    string nome_file, riga_comando;
    string message_error;
    char c;
    int contatore=0, i=0;
    bool auto_increm, not_null, key;
    string syntax_err = "Comando non valido, errore di sintassi";
    string first_word, word, nome_tabella, nome_colonna, condizione1, condizione2, nome, tipo, auto_increment;
    vector<string> first_word_comandi {"CREATE", "DROP", "INSERT", "DELETE", "UPDATE", "SELECT","QUIT"};
    vector<string> campi, valori, words;

   // cout << "Inserire nome file database" << endl;
   // cin >> nome_file;
   // tabelle=Avvio(nome_file);

    stringstream comando_intero;
    stringstream riga_temp;


    //IL COMMENTO QUI DI SEGUITO E' DA COMPLETARE NON TOCCARE !!!!
  /*  string comando;
    bool found_text = false, found_first = false, found_last = false;
    vector <int> pos_first, pos_last; //possono esserci più campi testo
    cout << "Inserisci comando: " << endl;
    //leggi comando intero
    cin >> comando;
    for (int j = 0; j < comando.size(); ++j) {
        if (!found_first){
            if (comando[j] == '"' && ((int)comando[j-1] != 39 && (int)comando[j+1] != 39)) { //se trovo un " e non è all'interno di un campo char
                found_text = true;
                found_first = true;
                pos_first.push_back(j);
            }
        }
        else {
            if (!found_last){
                if (comando[j] == '"' && ((int)comando[j-1] != 39 && (int)comando[j+1] != 39) ){
                    found_last = true;
                    pos_last.push_back(j);
                }
            }
            else {

            }
        }
    }
    for (int k = 0; k < comando.size(); ++k) {
        if (!found_text){
            if (comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39) ){
                comando.erase(k-1);
            }
        }
        else{
            if ( k < pos_first && k > pos_last ){

            }
            else {

            }
        }
    }*/



    while (riga_comando[riga_comando.size() - 1] != ';') {
        if (riga_comando[riga_comando.size() - 1] != ';') {
            getline(cin, riga_comando);
            comando_intero << riga_comando << " ";
        }
    }
    comando_intero >> first_word;


    while(compare_first_word_comandi(first_word)!=QUIT) {
        switch (compare_first_word_comandi(first_word)) {
            case CREATE :
               if (controllore.controlloCreate(comando_intero,&message_error)){
                   comando_intero >> word >> nome_tabella;
                   tabelle.emplace_back(new Tabella(nome_tabella));
                   comando_intero >> word;
                   getline(comando_intero, riga_comando, ',');
                   riga_temp << riga_comando;
                   riga_temp >> nome_colonna >> tipo;
                   while (tipo != "key") {
                       if (toUpper(tipo) == "INT") {
                           //se è auto increment mi servono delle stringhe in più
                           string word2, word3;
                           riga_temp >> word2;
                           riga_temp >> word3;
                           word+= " " + word2;
                           if (word == "NOT NULL") not_null = true;
                           getline(riga_temp, word, ',');
                           if (word == "AUTO_INCREMENT") auto_increm= true;
                       } else {
                           //se non è int controllo solo not null
                           getline(riga_temp, word, ',');
                           if (word == "NOT NULL") not_null = true;
                       }
                       //aggiungo la colonna
                       if(tipo == "INT") {
                           if (!not_null) {
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaInt(nome_colonna));
                           } else {
                               if (!auto_increm) {
                                   tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaInt(nome_colonna, false));
                               } else
                                   tabelle[tabelle.size()-1]->aggiungiColonna(
                                           new ColonnaInt(nome_colonna, false, true, &contatore));
                           }
                       } else if(tipo == "TEXT") {
                           if (!not_null)
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaText(nome_colonna));
                           else
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaInt(nome_colonna, false));
                       }
                       else if(tipo == "CHAR") {
                           if (!not_null)
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaChar(nome_colonna));
                           else
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaChar(nome_colonna, false));
                       }
                       else if(tipo == "DATE") {
                           if (!not_null)
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaDate(nome_colonna));
                           else
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaDate(nome_colonna, false));
                       }
                       else if(tipo == "FLOAT") {
                           if (!not_null)
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaFloat(nome_colonna));
                           else
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaFloat(nome_colonna, false));
                       }
                       else if(tipo == "TIME") {
                           if (!not_null)
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaTime(nome_colonna));
                           else
                               tabelle[tabelle.size()-1]->aggiungiColonna(new ColonnaTime(nome_colonna, false));
                       }
                       getline(comando_intero, riga_comando, ',');
                       riga_temp << riga_comando;
                       riga_temp >> nome_colonna >> tipo;
                   }
                   while (!comando_intero.eof()) {
                       if (nome_colonna == "PRIMARY") {
                           getline(riga_temp, word, '(');
                           getline(riga_temp, word, ')');
                           tabelle[tabelle.size()-1]->setChiavePrimaria(word);
                       } else if (nome_colonna == "FOREIGN") {
                              //FOREIGN KEY (COUNTRY_ID) REFERENCES COUNTRIES (ID)
                           getline(riga_temp, word, '(');
                           getline(riga_temp, nome_colonna, ')');
                           riga_temp << word;
                           riga_temp << nome_tabella;
                           for (i=0; i<tabelle.size(); i++) {
                               if (tabelle[i]->getNome() == nome_tabella)
                                   break;
                           }
                           getline(riga_temp, word, '(');
                           getline(riga_temp, word, ')');
                           tabelle[tabelle.size()-1]->setChiaveEsterna(tabelle[i], word, nome_colonna);
                       }
                       getline(comando_intero, riga_comando, ',');
                       riga_temp << riga_comando;
                       riga_temp >> nome_colonna >> tipo;
                   }

               }
               else
                   cout << message_error <<endl;
                break;
            case DROP :
                break;
            case INSERT :
                break;
            case DELETE :
                break;
            case TRUNCATE :
                if(controllore.controlloTruncate(comando_intero,&message_error)) {
                    if (tabelle.size() != 0){
                        comando_intero >> word;      //butto via "TABLE"
                        comando_intero >> word;       //in word1 ho nome_tab
                        for (auto &s : tabelle) {
                            if (word == s->getNome()) {
                                s->deleteRecord();
                                break;
                            }
                        }
                    }
                    else
                        cout << "Tabella non esistente" << endl;       //se non ho trovato tab con tale nome nel database
                    break;
                } else
                    cout << message_error << endl;
            case UPDATE :
                break;
            case SELECT :
                break;
            case QUIT :
                break;
            default:
                cerr << syntax_err << endl;
                break;
        }

        comando_intero.str("");
        riga_comando.clear();

        cout << "Inserisci comando: " << endl;
        //leggi comando intero
        while (riga_comando[riga_comando.size() - 1] != ';') {
            if (riga_comando[riga_comando.size() - 1] != ';') {
                getline(cin, riga_comando);
                comando_intero << riga_comando << " ";
            }
        }
        //cout << comando_intero.str() << endl;
        comando_intero >> first_word;
    }

   // Arresto(nome_file, tabelle);
    return 0;
}




int compare_first_word_comandi(string &first_word){
    first_word = toUpper(first_word);
    if(first_word == "CREATE")
        return CREATE;
    else if (first_word == "DROP")
        return DROP;
    else if (first_word == "INSERT")
        return INSERT;
    else if (first_word == "DELETE")
        return DELETE;
    else if (first_word == "TRUNCATE")
        return TRUNCATE;
    else if (first_word == "UPDATE")
        return UPDATE;
    else if (first_word == "SELECT")
        return SELECT;
    else if (first_word == "QUIT" || first_word == "QUIT;")
        return QUIT;
    else
        return ERR_COMANDO;
}


string toUpper(string word){
    for (auto & c : word)
        c = toupper(c);
    return word;
}

