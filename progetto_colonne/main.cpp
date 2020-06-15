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
#define ERR_COMANDO -1

using namespace std;


void deleteOggettoTabella(Tabella **ptr){
    if (*ptr != nullptr){
        delete *ptr;
        *ptr = nullptr;
    }
}




enum comando {CREATE, DROP, INSERT, DELETE, UPDATE, SELECT,QUIT};
int compare_first_word_comandi(string &first_word);
string toUpper(string word);


int main() {
    list<Tabella*> tabelle;
    string comando_intero, nome_file;
    string stringa;
    char c;
    string first_word, word, nome_colonna, condizione1, condizione2, nome, tipo, not_null, auto_increment;
    vector<string> first_word_comandi {"CREATE", "DROP", "INSERT", "DELETE", "UPDATE", "SELECT","QUIT"};
    vector<string> campi;
    vector<string> valori;

    cout << "Inserire nome file database" << endl;
    cin >> nome_file;
    tabelle=Avvio(nome_file);


    cout << "Inserisci comando: " << endl;
    //leggi comando intero
    while (stringa[stringa.size() - 1] != ';') {
        if (stringa[stringa.size() - 1] != ';') {
            getline(cin, stringa);
            comando_intero.append(stringa);
            comando_intero.append(" ");
        }
    }
    cout << "il comando è: " << comando_intero;
    //scomposizione comando
    stringstream buffer(comando_intero);
    buffer >> first_word;
    cout << endl << first_word;

    while(compare_first_word_comandi(first_word)!=QUIT) {
        switch (compare_first_word_comandi(first_word)) {
            case CREATE :
               /*CREATE TABLE CUSTOMERS(
                ID INT NOT NULL AUTO_INCREMENT,
                NAME TEXT NOT NULL,
                AGE INT NOT NULL,
                ADDRESS TEXT ,
                COUNTRY_ID INT
                SALARY FLOAT,
                PRIMARY KEY (ID)
                FOREIGN KEY (COUNTRY_ID) REFERENCES COUNTRIES (ID)); */
                buffer >> word;
                word = toUpper(word);
                if (word == "TABLE") {
                    getline(buffer, word, '(');
                    buffer >> c;
                    if(c!='(') {
                        cout << "Comando non valido, errore di sintassi" << endl;
                        break;
                    }
                    tabelle.emplace_back(new Tabella(word));
                    buffer >> word >> tipo;
                    campi.push_back(word);
                } else cout << "Comando non valido" << endl;
                break;
            case DROP :
                break;
            case INSERT :
                break;
            case DELETE :
                break;
            case UPDATE :
                break;
            case SELECT :
                break;
            case QUIT :
                break;
            default:
                //eccezione comando non valido
                break;
        }

        cout << "Inserisci comando: " << endl;
        //leggi comando intero
        stringa.clear();
        comando_intero.clear();
        while (stringa[stringa.size() - 1] != ';') {
            if (stringa[stringa.size() - 1] != ';') {
                getline(cin, stringa);
                comando_intero.append(stringa);
                comando_intero.append(" ");
            }
        }
        cout << "il comando è: " << comando_intero;
        //scomposizione comando
        buffer << comando_intero;
        buffer >> first_word;
        first_word = toUpper(first_word);
        cout << endl << first_word;
    }


    Arresto(nome_file, tabelle);
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