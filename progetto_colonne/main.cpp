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
    vector<Tabella*> tabelle;
    string nome_file, riga_comando;
    char c;
    int contatore=0, i=0;
    bool auto_increm, not_nul, key;
    string syntax_err = "Comando non valido, errore di sintassi";
    string first_word, word, nome_colonna, condizione1, condizione2, nome, tipo, not_null, auto_increment;
    vector<string> first_word_comandi {"CREATE", "DROP", "INSERT", "DELETE", "UPDATE", "SELECT","QUIT"};
    vector<string> campi, valori, words;
    ControlloSintassi oggetto;
    string message;

   // cout << "Inserire nome file database" << endl;
   // cin >> nome_file;
   // tabelle=Avvio(nome_file);


    stringstream comando_intero;
    cout << "Inserisci comando: " << endl;
    //leggi comando intero
    while (riga_comando[riga_comando.size() - 1] != ';') {
        if (riga_comando[riga_comando.size() - 1] != ';') {
            getline(cin, riga_comando);
            comando_intero << riga_comando << " ";
        }
    }
    cout << "il comando è: " << comando_intero.str();
    comando_intero >> first_word;
    cout << endl << first_word;

    while(compare_first_word_comandi(first_word)!=QUIT) {
        switch (compare_first_word_comandi(first_word)) {
            case CREATE :
               /*CREATE TABLE CUSTOMERS(
                ID INT NOT NULL AUTO_INCREMENT,
                NAME TEXT NOT NULL,
                AGE INT NOT NULL,
                ADDRESS TEXT,
                COUNTRY_ID INT,
                SALARY FLOAT,
                PRIMARY KEY (ID),
                FOREIGN KEY (COUNTRY_ID) REFERENCES COUNTRIES (ID)); */
               message = oggetto.controlloCreate(comando_intero);
               if (message == "valid"){
                   cout << "sono arrivato fin qui" << endl;
               }
               else
                   cout << message << endl;
               
                /*comando_intero >> word;
                word = toUpper(word);
                if (word == "TABLE") {
                    getline(comando_intero, word, '(');
                    comando_intero >> c;
                    if(c!='(') {
                        cout << syntax_err << endl;
                        break;
                    }
                    tabelle.emplace_back(new Tabella(word));
                    while(!comando_intero.eof()){
                        comando_intero >> words[i];
                        while(words[i][words[i].size()-1]!=',') {
                            i++;
                            words.emplace_back();
                            comando_intero >> words[i];
                        } //ho letto tutta la riga, in words[i] c'è la i-esima parola
                    }
                } else cout << "Comando non valido" << endl;*/
                break;
            case DROP :
                break;
            case INSERT :
                break;
            case DELETE :
                break;
            case TRUNCATE :
                comando_intero >> word;      //butto via "TABLE"
                comando_intero >> word;       //in word ho nome_tab
                for(auto & s : tabelle){
                    if(word==s->getNome()) {
                        s->deleteRecord();
                        break;
                    }
                }
                cout << "Tabella non esistente" << endl;       //se non ho trovato tab con tale nome nel database
                break;
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

        comando_intero.clear();
        riga_comando.clear();
        cout << "Inserisci comando: " << endl;
        //leggi comando intero
        while (riga_comando[riga_comando.size() - 1] != ';') {
            if (riga_comando[riga_comando.size() - 1] != ';') {
                getline(cin, riga_comando);
                comando_intero << riga_comando << " ";
            }
        }
        cout << "il comando è: " << comando_intero.str();
        comando_intero >> first_word;
        cout << endl << first_word;
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
    else if (first_word == "QUIT")
        return QUIT;
    else
        return ERR_COMANDO;
}


string toUpper(string word){
    for (auto & c : word)
        c = toupper(c);
    return word;
}

