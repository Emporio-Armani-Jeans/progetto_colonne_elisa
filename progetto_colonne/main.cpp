#include <iostream>
#include "ColonnaText.h"
#include "ColonnaFloat.h"
#include "ColonnaInt.h"
#include "Tabella.h"
#include "FileError.h"
#include <vector>
#include <string>
#include <sstream>
#include "Avvio_Arresto.hpp"
#include "ControlloSintassi.h"
#include "Comandi.hpp"
#define ERR_COMANDO -1

using namespace std;

enum comando {CREATE, DROP, INSERT, DELETE, TRUNCATE, UPDATE, SELECT,QUIT};


int compare_first_word_comandi(string &first_word);
bool belong_to(const string& elemento, const vector<string>& insieme);


int main() {
    ControlloSintassi controllore;
    vector<Tabella*> tabelle;
    string nome_file, riga_comando;
    string message_error;
    char c;
    int contatore=0, i=0, a, b;
    bool auto_increm, not_null, key, trovata=false, ok=false;
    string syntax_err = "Comando non valido, errore di sintassi", message, first_word;
    vector<string> first_word_comandi {"CREATE", "DROP", "INSERT", "DELETE", "UPDATE", "SELECT","QUIT"};
    vector<string> campi, valori, words;

    while(!ok) {
        try {
            cout << "Inserire nome file database" << endl;
            cin >> nome_file;
            ok=true;
        }
        catch (exception &file) {
            ok=false;
            cout << "Eccezione: " << file.what() << endl;
        }
    }
    tabelle = Avvio(nome_file, &contatore);

    stringstream riga_temp;

    string comando;
    bool found_text = false;
    bool campo_testo_presente = false;
    bool inside_testo = false;
    vector <int> pos_first, pos_last; //possono esserci più campi testo
    getchar();
    cout << "Inserisci comando: " << endl;



    //leggi comando intero
    getline(cin,comando);
    for (int j = 0; j < comando.size(); ++j) {
        if (!found_text){
            if (comando[j] == '"' && ((int)comando[j-1] != 39 && (int)comando[j+1] != 39)) { //se trovo un " e non è all'interno di un campo char
                found_text = true;
                campo_testo_presente = true;
                pos_first.push_back(j);
            }
        }
        else {
            if ( (comando[j] == '"' && comando[j+1] != '"' ) && ((comando[j] == '"' && comando[j-1] != '"') || (comando[j]=='"' && comando[j-1] == '"' && comando[j-2] == '"')) ){
                found_text = false;
                pos_last.push_back(j);
            }
        }
    }

    for (int k = 0; k < comando.size(); ++k) {
        inside_testo = false;
        if (!campo_testo_presente){
            if ( k!= comando.size()-1 && comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39) ){
                comando.erase(k+1);
            }
        }
        else{
            if (pos_first.size() != pos_last.size())
                cout << "errore" <<endl; //modificare
            else{
                for (int j = 0; j < pos_first.size(); ++j) {
                    if (k > pos_first[j] && k < pos_last[j]){
                        inside_testo = true;
                    }
                }
                if (!inside_testo){
                    if ( k != comando.size()-1 && comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39)){
                        comando.erase(k+1);
                    }
                }
            }
        }
    }


    cout << comando << endl;

    stringstream comando_intero(comando);
    stringstream comando_per_controlli(comando);

    /*while (riga_comando[riga_comando.size() - 1] != ';') {
        if (riga_comando[riga_comando.size() - 1] != ';') {
            getline(cin, riga_comando);
            comando_intero << riga_comando << " ";
        }
    }*/
    comando_intero >> first_word;


    while(compare_first_word_comandi(first_word)!=QUIT) {
        switch (compare_first_word_comandi(first_word)) {
            case CREATE :
                  if (controllore.controlloCreate(comando_per_controlli, &message_error)) {
                    Create(tabelle, comando_intero, &contatore, &message);
                  } else
                      cout << message_error << endl;
                break;
            case DROP :
                if(controllore.controlloDrop(comando_per_controlli, &message_error)) {
                    Drop(tabelle, comando_intero, &message);
                } else
                    cout << message_error << endl;
                break;
            case INSERT :   //fare controllo
                break;
            case DELETE :   //fare controllo
                Delete(tabelle, comando_intero, &message);
                cout << message << endl;
                break;
            case TRUNCATE :
                if (controllore.controlloTruncate(comando_per_controlli, &message_error)) {
                    Truncate(tabelle, comando_intero, &message);
                    cout << message << endl;
                } else
                    cout << message_error << endl;
                break;
            case UPDATE :   //fare controllo
                Update(tabelle, comando_intero, &message);
                cout << message << endl;
                break;/*
            case SELECT :
                comando_intero >> word;  //leggo seconda parola
                campi.clear();
                if (word == "*") {
                    comando_intero >> scarto;  //scarto FROM
                    comando_intero >> word;
                    if (word[word.size() - 1] == ';') word.pop_back();
                    for (a = 0, trovata = false; a < tabelle.size(); a++) {
                        if (toUpper(tabelle[a]->getNome()) == word) {
                            trovata = true;
                            break;
                        }
                    }
                    //stampo tutta la tabella
                    for (const string &elem : tabelle[a]->returnData()) {
                        if(elem.empty()) cout << "Riga vuota" << endl;
                        else cout << elem << endl;
                    }
                    cout << endl;
                } else {
                    while (toUpper(word) ==
                           "FROM") {   //memorizzo campi finchè non incontro from, suppongo spazio dopo la virgola
                        word.pop_back(); //rimuovo virgola
                        campi.push_back(word);
                        comando_intero >> word;
                    }

                    comando_intero >> word;
                    //cerco match tabella in questione
                    for (a = 0, trovata = false; a < tabelle.size(); a++) {
                        if (toUpper(tabelle[a]->getNome()) == word) {
                            trovata = true;
                            break;
                        }
                    }
                    if (!trovata) {
                        cout << "Tabella non esistente" << endl;
                        break;
                    } else {
                        if (word[word.size() - 1] == ';') {
                            word.pop_back();
                            //stampo solo campi specificati della tabella
                            for (const auto &elem : tabelle[a]->returnData(campi)) {
                                cout << elem << " ";
                            }
                            cout << endl;
                        } else {
                            comando_intero >> word; //butto via "WHERE"
                            comando_intero >> word;  //in word c'è il campo condizione
                            comando_intero >> word2;  //in word2 c'è l'operatore
                            if (!belong_to(word2, operatori)) {
                                cout << "Operatore non valido" << endl;
                                break;
                            } else {
                                //casistica operatori
                                if (word2 == "=") {
                                    comando_intero >> word3; //in word3 c'è la condizione
                                    if (word3[word3.size() - 1] != ';') {   //se non c'è ';' ci sarà ordinamento
                                        if (word3[0] == '"') {
                                            word3.erase(0, 1);
                                            word3.pop_back();
                                        }
                                        comando_intero >> scarto;  //scarto order
                                        comando_intero >> scarto; //scarto by
                                        comando_intero >> nome_colonna;   //campo ordinamento
                                        comando_intero >> scarto; //scarto DESC
                                        for (int z = 0;
                                             z <
                                             tabelle[b]->returnData(campi, word2, word3, 0, nome_colonna,
                                                                    1).size(); z++) {
                                            cout << tabelle[b]->returnData(campi, word2, word3, 0, nome_colonna, 1)[z]
                                                 << endl;
                                        }
                                    } else {
                                        word3.pop_back();
                                        //stampo campi che rispettando condizione senza ordinamento
                                        for (const auto &elem : tabelle[a]->returnData(campi, word, word3)) {
                                            cout << elem << " ";
                                        }
                                        cout << endl;
                                    }
                                } else if (word2 == "<") {
                                    comando_intero >> word3; //in word3 c'è la condizione
                                    if (word3[word3.size()] != ';') {
                                        if (word3[0] == '"') {          //se ci sono virgolette le tolgo
                                            word3.erase(0, 1);
                                            word3.pop_back();
                                        }
                                        comando_intero >> scarto;  //scarto order
                                        comando_intero >> scarto; //scarto by
                                        comando_intero >> nome_colonna;   //campo ordinamento
                                        comando_intero >> scarto; //scarto DESC
                                        for (int z = 0;
                                             z <
                                             tabelle[b]->returnData(campi, word2, word3, 1, nome_colonna,
                                                                    1).size(); z++) {
                                            cout << tabelle[b]->returnData(campi, word2, word3, 1, nome_colonna, 1)[z]
                                                 << endl;
                                        }
                                    } else {
                                        word3.pop_back();     //tolgo virgoletta o ';'
                                        //stampo campi che rispettando condizione senza ordinamento
                                        for (const auto &elem : tabelle[a]->returnData(campi, word, word3, 1)) {
                                            cout << elem << " ";
                                        }
                                        cout << endl;
                                    }
                                } else if (word2 == "<=") {
                                    comando_intero >> word3; //in word3 c'è la condizione
                                    if (word3[word3.size()] != ';') {
                                        if (word3[0] == '"') {          //se ci sono virgolette le tolgo
                                            word3.erase(0, 1);
                                            word3.pop_back();
                                        }
                                        comando_intero >> scarto;  //scarto order
                                        comando_intero >> scarto; //scarto by
                                        comando_intero >> nome_colonna;   //campo ordinamento
                                        comando_intero >> scarto; //scarto DESC
                                        for (int z = 0;
                                             z <
                                             tabelle[b]->returnData(campi, word2, word3, 2, nome_colonna,
                                                                    1).size(); z++) {
                                            cout << tabelle[b]->returnData(campi, word2, word3, 2, nome_colonna, 1)[z]
                                                 << endl;
                                        }
                                    } else {
                                        word3.pop_back();
                                        //stampo campi che rispettando condizione senza ordinamento
                                        for (const auto &elem : tabelle[a]->returnData(campi, word, word3, 2)) {
                                            cout << elem << " ";
                                        }
                                        cout << endl;
                                    }
                                } else if (word2 == ">") {
                                    comando_intero >> word3; //in word3 c'è la condizione
                                    if (word3[word3.size()] != ';') {
                                        if (word3[0] == '"') {          //se ci sono virgolette le tolgo
                                            word3.erase(0, 1);
                                            word3.pop_back();
                                        }
                                        comando_intero >> scarto;  //scarto order
                                        comando_intero >> scarto; //scarto by
                                        comando_intero >> nome_colonna;   //campo ordinamento
                                        comando_intero >> scarto; //scarto DESC
                                        for (int z = 0;
                                             z <
                                             tabelle[b]->returnData(campi, word2, word3, 3, nome_colonna,
                                                                    1).size(); z++) {
                                            cout << tabelle[b]->returnData(campi, word2, word3, 3, nome_colonna, 1)[z]
                                                 << endl;
                                        }
                                    } else {
                                        word3.pop_back();
                                        //stampo campi che rispettando condizione senza ordinamento
                                        for (const auto &elem : tabelle[a]->returnData(campi, word, word3, 3)) {
                                            cout << elem << " ";
                                        }
                                        cout << endl;
                                    }
                                } else if (word2 == ">=") {
                                    comando_intero >> word3; //in word3 c'è la condizione
                                    if (word3[word3.size()] != ';') {
                                        if (word3[0] == '"') {          //se ci sono virgolette le tolgo
                                            word3.erase(0, 1);
                                            word3.pop_back();
                                        }
                                        comando_intero >> scarto;  //scarto order
                                        comando_intero >> scarto; //scarto by
                                        comando_intero >> nome_colonna;   //campo ordinamento
                                        comando_intero >> scarto; //scarto DESC
                                        for (int z = 0;
                                             z <
                                             tabelle[b]->returnData(campi, word2, word3, 4, nome_colonna,
                                                                    1).size(); z++) {
                                            cout << tabelle[b]->returnData(campi, word2, word3, 4, nome_colonna, 1)[z]
                                                 << endl;
                                        }
                                    } else {
                                        word3.pop_back();
                                        //stampo campi che rispettando condizione senza ordinamento
                                        for (const auto &elem : tabelle[a]->returnData(campi, word, word3, 4)) {
                                            cout << elem << " ";
                                        }
                                        cout << endl;
                                    }
                                } else if (word2 == "<>") {
                                    comando_intero >> word3; //in word3 c'è la condizione
                                    if (word3[word3.size()] != ';') {
                                        if (word3[0] == '"') {          //se ci sono virgolette le tolgo
                                            word3.erase(0, 1);
                                            word3.pop_back();
                                        }
                                        comando_intero >> scarto;  //scarto order
                                        comando_intero >> scarto; //scarto by
                                        comando_intero >> nome_colonna;   //campo ordinamento
                                        comando_intero >> scarto; //scarto DESC
                                        for (int z = 0;
                                             z <
                                             tabelle[b]->returnData(campi, word2, word3, 5, nome_colonna,
                                                                    1).size(); z++) {
                                            cout << tabelle[b]->returnData(campi, word2, word3, 5, nome_colonna, 1)[z]
                                                 << endl;
                                        }
                                    } else {
                                        word3.pop_back();
                                        //stampo campi che rispettando condizione senza ordinamento
                                        for (const auto &elem : tabelle[a]->returnData(campi, word, word3, 5)) {
                                            cout << elem << " ";
                                        }
                                        cout << endl;
                                    }
                                } else if (toUpper(word2) == "BETWEEN") {
                                    comando_intero >> condizione1;
                                    if (condizione1[0] == '"') {          //se ci sono virgolette le tolgo
                                        condizione1.erase(0, 1);
                                        condizione1.pop_back();
                                    }
                                    comando_intero >> scarto; //scarto and
                                    comando_intero >> condizione2;
                                    if (condizione2[condizione2.size()] != ';') {
                                        if (condizione2[0] == '"') {          //se ci sono virgolette le tolgo
                                            condizione2.erase(0, 1);
                                            condizione2.pop_back();
                                        }
                                        comando_intero >> scarto;  //scarto order
                                        comando_intero >> scarto; //scarto by
                                        comando_intero >> nome_colonna;   //campo ordinamento
                                        comando_intero >> scarto; //scarto DESC
                                        for (int z = 0;
                                             z < tabelle[b]->returnData(campi, word2, condizione1, condizione2,
                                                                        nome_colonna, 1).size(); z++) {
                                            cout << tabelle[b]->returnData(campi, word2, condizione1, condizione2,
                                                                           nome_colonna,
                                                                           1)[z] << endl;
                                        }
                                    } else {
                                        condizione2.pop_back();
                                        //stampo campi che rispettando condizione senza ordinamento
                                        for (const auto &elem : tabelle[a]->returnData(campi, word, condizione1,
                                                                                       condizione2)) {
                                            cout << elem << " ";
                                        }
                                        cout << endl;
                                    }
                                }
                            }
                        }
                    }
                }*/
                break;
            case QUIT :
                break;
            default:
                cerr << syntax_err << endl;
                break;
        }

        comando.clear();
        comando_intero.clear();
        comando_per_controlli.clear();
        comando_intero.str("");
        comando_per_controlli.str("");
        riga_comando.clear();

        cout << "Inserisci comando: " << endl;
        getline(cin,comando);
        int temp=comando.size();
        for (int j = 0; j < temp; ++j) {
            if (!found_text){
                if (comando[j] == '"' && ((int)comando[j-1] != 39 && (int)comando[j+1] != 39)) { //se trovo un " e non è all'interno di un campo char
                    found_text = true;
                    campo_testo_presente = true;
                    pos_first.push_back(j);
                }
            }
            else {
                if ( (comando[j] == '"' && comando[j+1] != '"' ) && ((comando[j] == '"' && comando[j-1] != '"') || (comando[j]=='"' && comando[j-1] == '"' && comando[j-2] == '"')) ){
                    found_text = false;
                    pos_last.push_back(j);
                }
            }
        }

        for (int k = 0; k < comando.size(); ++k) {
            inside_testo = false;
            if (!campo_testo_presente){
                if ( k!= comando.size()-1 && comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39) ){
                    comando.erase(k+1);
                }
            }
            else{
                if (pos_first.size() != pos_last.size())
                    cout << "errore" <<endl; //modificare
                else{
                    for (int j = 0; j < pos_first.size(); ++j) {
                        if (k > pos_first[j] && k < pos_last[j]){
                            inside_testo = true;
                        }
                    }
                    if (!inside_testo){
                        if ( k != comando.size()-1 && comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39)){
                            comando.erase(k+1);
                        }
                    }
                }
            }
        }
        cout << comando << endl;
        comando_intero << comando;
        comando_per_controlli << comando;
        comando_intero >> first_word;
    }
    cout << "Arresto in corso..." << endl;

    Arresto(nome_file, tabelle);

    cout << "Arresto eseguito correttamente" << endl;

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
