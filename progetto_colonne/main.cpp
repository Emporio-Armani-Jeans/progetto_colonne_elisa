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


void deleteOggettoTabella(Tabella **ptr){
    if (*ptr != nullptr){
        delete *ptr;
        *ptr = nullptr;
    }
}

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
    bool auto_increm, not_null, key, trovata=false;
    string syntax_err = "Comando non valido, errore di sintassi";
    string first_word, word, word2, word3, scarto, nome_tabella, nome_colonna, condizione1, condizione2,
    nome, tipo, auto_increment;
    vector<string> operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    vector<string> first_word_comandi {"CREATE", "DROP", "INSERT", "DELETE", "UPDATE", "SELECT","QUIT"};
    vector<string> campi, valori, words;

    cout << "Inserire nome file database" << endl;
    cin >> nome_file;
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
                      //cout << "sono arrivato fin qui" << endl;
                    Create(tabelle, comando_intero, &contatore);
                  } else
                      cout << message_error << endl;
                break;
            /*case DROP :
                comando_intero >> word;      //butto via "TABLE"
                comando_intero >> word;
                word.pop_back();            //butto via ';'
                //ricerco match nome tabella
                for (a = 0, trovata = false; a < tabelle.size(); a++) {
                    if (toUpper(tabelle[a]->getNome()) == word) {
                        trovata = true;
                        break;
                    }
                }
                if (!trovata) {
                    cout << "Tabella non esistente" << endl;
                } else {
                    deleteOggettoTabella(&tabelle[a]);
                }
                break;
            case INSERT :
                break;
            case DELETE :
                comando_intero >> word;    //butto via "FROM"
                comando_intero >> word;
                //cerco match nome tabella
                for (a = 0, trovata = false; a < tabelle.size(); a++) {
                    if (toUpper(tabelle[a]->getNome()) == word) {
                        trovata = true;
                        break;
                    }
                }
                if (!trovata) {
                    cout << "Tabella non esistente" << endl;
                    break;
                } else {                //a contiene indice della tabella scelta
                    comando_intero >> word; //butto via "WHERE"
                    comando_intero >> word;
                    //cerco match con campo condizione
                    for (b = 0, trovata = false; b < tabelle.size(); b++) {
                        if (toUpper(tabelle[a]->getCol(b)->getNomeColonna()) == word) {
                            trovata = true;
                            break;
                        }
                    }
                    if (!trovata) {
                        cout << "Campo non esistente nella tabella" << endl;
                        break;
                    } else {
                        //leggo operatore; assumere che tra operatore e parole adiacenti ci sia almeno uno spazio
                        comando_intero >> word;
                        if (!(belong_to(word, operatori))) {
                            cout << "Operatore non valido" << endl;
                            break;
                        } else {
                            if (word == "=") {
                                comando_intero >> word;
                                word.pop_back();
                                word.pop_back();
                                word.erase(0, 1);    //rimuovo virgolette e punto e virgola finale
                                tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word);
                            } else if (word == "<") {
                                comando_intero >> word;
                                word.pop_back();
                                word.pop_back();
                                word.erase(0, 1);    //rimuovo virgolette e punto e virgola finale
                                tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 1);
                            } else if (word == "<=") {
                                comando_intero >> word;
                                word.pop_back();
                                word.pop_back();
                                word.erase(0, 1);    //rimuovo virgolette e punto e virgola finale
                                tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 2);
                            } else if (word == ">") {
                                comando_intero >> word;
                                word.pop_back();
                                word.pop_back();
                                word.erase(0, 1);    //rimuovo virgolette e punto e virgola finale
                                tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 3);
                            } else if (word == ">=") {
                                comando_intero >> word;
                                word.pop_back();
                                word.pop_back();
                                word.erase(0, 1);    //rimuovo virgolette e punto e virgola finale
                                tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 4);
                            } else if (word == "<>") {
                                comando_intero >> word;
                                word.pop_back();
                                word.pop_back();
                                word.erase(0, 1);    //rimuovo virgolette e punto e virgola finale
                                tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, 5);
                            } else if (toUpper(word) == "BETWEEN") {
                                comando_intero >> word; //prima condizione
                                comando_intero >> word2; //butto via "AND"
                                comando_intero >> word2; //seconda condizione
                                word.erase(0, 1);
                                word.pop_back();           //rimuovo le virgolette
                                word2.erase(0, 1);
                                word2.pop_back();
                                tabelle[a]->deleteRecord(tabelle[a]->getCol(b)->getNomeColonna(), word, word2);
                            }
                        }
                    }
                }
                break;
            case TRUNCATE :
                if (controllore.controlloTruncate(comando_intero, &message_error)) {
                    if (!tabelle.empty()) {
                        comando_intero >> word;      //butto via "TABLE"
                        comando_intero >> word;       //in word1 ho nome_tab
                        for (auto &s : tabelle) {
                            if (word == s->getNome()) {
                                s->deleteRecord();
                                break;
                            }
                        }
                    } else
                        cout << "Tabella non esistente" << endl;     //se non ho trovato tab con tale nome nel database
                } else {
                    cout << message_error << endl;
                }
                break;
            case UPDATE :
                campi.clear();
                valori.clear();
                comando_intero >> word;
                for (a = 0, trovata = false; a < tabelle.size(); a++) {
                    if (toUpper(tabelle[a]->getNome()) == word) {
                        trovata = true;
                        break;
                    }
                }
                comando_intero >> scarto; //scarto SET
                word.clear();
                word2.clear();
                while (toUpper(word) != "WHERE") {
                    comando_intero >> word;
                    campi.push_back(word);
                    comando_intero >> scarto;   //operatore è sempre '='
                    comando_intero >> word2;   //valore
                    word2.pop_back();   //rimuovo virgola
                    if (word2[0] == '"') {
                        word2.pop_back();
                        word2.erase(0, 1);
                    }
                    valori.push_back(word2);
                }    //ho opportunamente riempito campi e valori
                comando_intero >> word; //in word c'è campo condizione
                comando_intero >> word2; //operatore
                if (word2 == "=") {
                    comando_intero >> word3; //condizione
                    word3.pop_back();  //rimuovo ';'
                    if (word3[0] == '"') {
                        word3.erase(0, 1);
                        word3.pop_back();
                    }
                    tabelle[a]->updateRecord(word, word3, campi, valori);
                } else if (word2 == "<") {
                    comando_intero >> word3; //condizione
                    word3.pop_back();  //rimuovo ';'
                    if (word3[0] == '"') {
                        word3.erase(0, 1);
                        word3.pop_back();
                    }
                    tabelle[a]->updateRecord(word, word3, campi, valori, 1);
                } else if (word2 == "<=") {
                    comando_intero >> word3; //condizione
                    word3.pop_back();  //rimuovo ';'
                    if (word3[0] == '"') {
                        word3.erase(0, 1);
                        word3.pop_back();
                    }
                    tabelle[a]->updateRecord(word, word3, campi, valori, 2);
                } else if (word2 == ">") {
                    comando_intero >> word3; //condizione
                    word3.pop_back();  //rimuovo ';'
                    if (word3[0] == '"') {
                        word3.erase(0, 1);
                        word3.pop_back();
                    }
                    tabelle[a]->updateRecord(word, word3, campi, valori, 3);
                } else if (word2 == ">=") {
                    comando_intero >> word3; //condizione
                    word3.pop_back();  //rimuovo ';'
                    if (word3[0] == '"') {
                        word3.erase(0, 1);
                        word3.pop_back();
                    }
                    tabelle[a]->updateRecord(word, word3, campi, valori, 4);
                } else if (word2 == "<>") {
                    comando_intero >> word3; //condizione
                    word3.pop_back();  //rimuovo ';'
                    if (word3[0] == '"') {
                        word3.erase(0, 1);
                        word3.pop_back();
                    }
                    tabelle[a]->updateRecord(word, word3, campi, valori, 5);
                } else if (toUpper(word2) == "BETWEEN") {
                    comando_intero >> condizione1;
                    if (condizione1[0] == '"') {
                        condizione1.erase(0, 1);
                        condizione1.pop_back();
                    }
                    comando_intero >> scarto; //scarto AND
                    comando_intero >> condizione2;
                    condizione2.pop_back();  //rimuovo ';'
                    if (condizione1[0] == '"') {
                        condizione1.erase(0, 1);
                        condizione1.pop_back();
                    }
                    tabelle[a]->updateRecord(word, condizione1, condizione2, campi, valori);
                }
                break;
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
    /*cout << "Arresto in corso..." << endl;

    Arresto(nome_file, tabelle);

    cout << "Arresto eseguito correttamente" << endl;*/

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


bool belong_to(const string& elemento, const vector<string>& insieme){
    for(const auto & elem : insieme){
        if(elemento==elem) return true;
    }
    return false;
}