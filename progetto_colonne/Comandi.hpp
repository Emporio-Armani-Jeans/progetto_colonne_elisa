//
// Created by andry on 21/06/2020.
//

#ifndef PROGETTO_COLONNE_COMANDI_HPP
#define PROGETTO_COLONNE_COMANDI_HPP
#include <vector>
#include "Tabella.h"
#include "InexistentTable.h"
#include "TableAlreadyExisting.h"
#include <string>
using namespace std;

string campoTesto(stringstream *comando, string &word){
    int contatore_virgolette = 0;
    bool fine_testo = false;
    char carattere;
    int k;
    for (k = 1; k < word.size(); k++){
        if (word[k] == 34) {
            contatore_virgolette++;
        } else {
            if (contatore_virgolette % 2 != 0) {
                fine_testo = true;
                break;
            }
            contatore_virgolette = 0;
        }
    }
    if (!fine_testo) {
        while (!fine_testo) {
            (*comando).get(carattere);
            if (carattere == 34) {
                word.push_back(carattere);
                contatore_virgolette++;
            } else {
                word.push_back(carattere);
                if (contatore_virgolette % 2 != 0) {
                    fine_testo = true;
                }
                contatore_virgolette = 0;
            }
        }
    }
    else
        carattere = word[k];
    if (carattere == ';')
        word.pop_back();
    if (carattere == ')' && word[word.size()-1] == ')')
        word.push_back(')');
    word.pop_back();
    word.erase(0,1);
    return word;
}

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

vector<Tabella*> Create(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message) {
    vector <Tabella> tabelle_temporanee;
    for (auto & j : tabelle){
        tabelle_temporanee.push_back((*j));
    }
    string scarto, word, word2, word3, nome_tabella, nome_colonna, tipo, riga_comando, testo_chiavi;
    bool not_null = false, auto_increm = false;
    stringstream riga_temp;
    bool c, tabella_already_existing = false;

    stream_comando >> scarto >> nome_tabella;
    if (nome_tabella[nome_tabella.size() - 1] == '(') {
        nome_tabella.pop_back();
    } else {
        stream_comando >> scarto;
    }
    for (auto & j : tabelle){
        if (j->getNome() == nome_tabella)
            tabella_already_existing = true;
    }
    if (!tabella_already_existing)
        tabelle.emplace_back(new Tabella(nome_tabella));
    else
        throw TableAlreadyExisting();

    stream_comando >> nome_colonna >> tipo;
    while (toUpper(tipo) != "KEY") {
        if (tipo[tipo.size()-1] == ','){
            tipo.pop_back();
        }
        else
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
            } else if(toUpper(word2)=="AUTO_INCREMENT"){//auto_increment
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
                                new ColonnaInt(nome_colonna, false, true));
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
                riga_comando.clear();
                word.clear();
                word2.clear();
                word3.clear();
                auto_increm = false;
                not_null = false;
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
            stream_chiavi >> scarto >> nome_tabella; //in nome tabella ho la tabella esterna (madre)
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
                getline(stream_chiavi, word, ')');  //colonna madre
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

    (*message) = "Operazione completata: tabella creata e aggiunta al database.";
    return tabelle;
}

vector<Tabella*> Drop(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){        //elimina completamente
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
        if(!(*it)->isLinked()) {
            tabelle.erase(it);
            (*message) = "Operazione completata: tabella eliminata dal database.";
            return tabelle;
        } else throw LinkedError();
    }
}

vector<Tabella*> Truncate(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message) {
    string word;
    bool tabella_trovata = false;
    stream_comando >> word;      //butto via "TABLE"
    stream_comando >> word;       //in word ho nome_tab
    word.pop_back();   //rimuovo ';'
    for (Tabella* &s : tabelle) {
        if ( word == s->getNome() ) {
            tabella_trovata = true;
            if(!(s)->isLinked()) {
                s->deleteRecord();
            } else throw LinkedError();
            break;
        }
    }
    if (tabella_trovata){
        (*message) = "Operazione completata: Tabella svuotata correttamente";
        return tabelle;
    }
    else
        throw InexistentTable();
}

vector<Tabella*> Delete(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message) {
    vector<string> operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    string word, word2, operatore, nome_colonna;
    int pos_table, pos_colonna;
    bool tabella_trovata = false, colonna_trovata = false;
    stream_comando >> word;    //butto via "FROM"
    stream_comando >> word;
    //cerco match nome tabella
    for (int a = 0; a < tabelle.size(); a++) {
        if (tabelle[a]->getNome() == word) {
            pos_table = a;
            tabella_trovata = true;
            break;
        }
    }
    if (!tabella_trovata) {
        throw InexistentTable();
    } else {   //pos_table contiene indice della tabella scelta
        stream_comando >> word; //butto via "WHERE"
        stream_comando >> nome_colonna; //in nome_colonna c'è il campo condizione
            stream_comando >> operatore;
            if (!(belong_to(operatore, operatori))) {
                throw InvalidOperator();
            } else {
                if (toUpper(operatore) == "BETWEEN") {
                    stream_comando >> word; //prima condizione
                    if (word[0] == '"') {
                        word = campoTesto(&stream_comando, word);
                    }
                    else if (word[0] == 39){ //se è un char tolgo le '
                        word.pop_back();
                        word.erase(0,1);
                    }
                    stream_comando >> word2; //butto via "AND"
                    stream_comando >> word2; //seconda condizione
                    if (word2[0] == '"') {
                        word2 = campoTesto(&stream_comando, word2);
                    }
                    else if (word2[0] == 39){ //se è un char tolgo le '
                        if (word2[word2.size()-1] == ';')
                            word2.pop_back();
                        word2.pop_back();//'
                        word2.erase(0,1);//'
                    } else {
                        word2.pop_back(); //;
                    }
                    tabelle[pos_table]->deleteRecord(nome_colonna, word, word2);
                }else {
                    stream_comando >> word;
                    if (word[0] == '"') {
                        word = campoTesto(&stream_comando, word);
                    }
                    else if (word[0] == 39){ //se è un char tolgo le '
                        word.pop_back();// ;
                        word.pop_back();// '
                        word.erase(0,1);// '
                    } else {
                        word.pop_back(); // ;
                    }
                    if (operatore == "=") {
                        tabelle[pos_table]->deleteRecord(nome_colonna, word);
                    } else if (operatore == "<") {
                        tabelle[pos_table]->deleteRecord(nome_colonna, word, 1);
                    } else if (operatore == "<=") {
                        tabelle[pos_table]->deleteRecord(nome_colonna, word, 2);
                    } else if (operatore == ">") {
                        tabelle[pos_table]->deleteRecord(nome_colonna, word, 3);
                    } else if (operatore == ">=") {
                        tabelle[pos_table]->deleteRecord(nome_colonna, word, 4);
                    } else if (operatore == "<>") {
                        tabelle[pos_table]->deleteRecord(nome_colonna, word, 5);
                    }
                }
                (*message) = "I record che non erano collegati a chiavi esterne sono stati eliminati.";
                return tabelle;
            }
    }
}

vector<Tabella*> Update(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
    string scarto, word, word2, word3, condizione1, condizione2;
    vector<string> campi, valori, operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    int pos_table;
    bool trovata = false;
    stream_comando >> word;
    for (int a = 0; a < tabelle.size(); a++) {
        if (toUpper(tabelle[a]->getNome()) == word) {
            trovata = true;
            pos_table = a;
            break;
        }
    }
    if(!trovata){
        throw InexistentTable();
    }else {
        stream_comando >> scarto; //scarto SET
        stream_comando >> word;
        while (toUpper(word) != "WHERE") {
            campi.push_back(word);
            stream_comando >> scarto;   //operatore è sempre '='
            stream_comando >> word2;   //valore
            if (word2[0] == '"'){ //campo testo
                word2 = campoTesto(&stream_comando, word2);
                if (word2[word2.size()-1] == '"'){
                    word2.pop_back();
                }
            }
            else if (word2[0] == 39){
                if (word2[word2.size()-1] == ',')
                    word2.pop_back();
                word2.pop_back(); // ' alla fine
                word2.erase(0,1); // ' all'inizio
            }
            else {
                if(word2[word2.size()-1]==',') {
                    word2.pop_back();   //rimuovo virgola
                }
            }
            valori.push_back(word2);
            stream_comando >> word;
        }    //ho opportunamente riempito campi e valori


        stream_comando >> word; //in word c'è campo condizione
        stream_comando >> word2; //operatore
        if(!belong_to(word2, operatori)){
            throw InvalidOperator();
        }else {
            if (toUpper(word2) == "BETWEEN") {
                stream_comando >> condizione1;
                if (condizione1[0] == '"') {
                    condizione1 = campoTesto(&stream_comando, condizione1);
                }
                else if (condizione1[0] == 39){ //se è un char tolgo le '
                    condizione1.pop_back();
                    condizione1.erase(0,1);
                }
                stream_comando >> scarto; //scarto AND
                stream_comando >> condizione2;
                if (condizione2[0] == '"') {
                    condizione2 = campoTesto(&stream_comando, condizione2);
                }
                else if (condizione2[0] == 39){ //se è un char tolgo le '
                    if (condizione2[condizione2.size()-1] == ';')
                        condizione2.pop_back();
                    condizione2.pop_back();//'
                    condizione2.erase(0,1);//'
                } else {
                    if (condizione2[condizione2.size()-1] == ';')
                        condizione2.pop_back(); //;
                }
                tabelle[pos_table]->updateRecord(word, condizione1, condizione2, campi, valori);
            } else {
                stream_comando >> word3; //condizione
                if (word3[0] == '"') {
                    word3 = campoTesto(&stream_comando, word3);
                }
                else if (word3[0] == 39){ //se è un char tolgo le '
                    if (word3[word3.size()-1] == ';')
                        word3.pop_back();
                    word3.pop_back();//'
                    word3.erase(0,1);//'
                } else {
                    if (word3[word3.size()-1] == ';')
                        word3.pop_back(); //;
                }
                if (word2 == "=") {
                    tabelle[pos_table]->updateRecord(word, word3, campi, valori);
                } else if (word2 == "<") {
                    tabelle[pos_table]->updateRecord(word, word3, campi, valori, 1);
                } else if (word2 == "<=") {
                    tabelle[pos_table]->updateRecord(word, word3, campi, valori, 2);
                } else if (word2 == ">") {
                    tabelle[pos_table]->updateRecord(word, word3, campi, valori, 3);
                } else if (word2 == ">=") {
                    tabelle[pos_table]->updateRecord(word, word3, campi, valori, 4);
                } else if (word2 == "<>") {
                    tabelle[pos_table]->updateRecord(word, word3, campi, valori, 5);
                }
            }
        }
    }
    message->assign("I campi dei record non collegati ad altre tabelle sono stati aggiornati.");
    return tabelle;
}

void Select(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message){
    string word, scarto, word2, word3, nome_colonna, condizione1, condizione2, ordine;
    string valori_inesistenti = "Nella tabella selezionata non risultano valori che rispettano i criteri di ricerca.";
    int pos_table;
    bool trovata = false, ordinamento = false, completo = false;
    vector<string> campi, operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    stream_comando >> word;  //leggo seconda parola
    if (word == "*") {
        completo=true;
        stream_comando >> scarto; //FROM
    } else {
        completo=false;
        while (word[word.size()-1] == ',') {   //memorizzo campi finchè non incontro from, suppongo spazio dopo la virgola
            word.pop_back (); //rimuovo virgola
            campi.push_back (word);
            stream_comando >> word;
        }
    }
    if (!completo){
        campi.push_back (word);
        stream_comando >> word; //FROM
    }
    for(string & elem : campi){
        cout << elem << "   ";
    }
    cout << endl;
    stream_comando >> word;
    scarto=word;
    if(scarto[scarto.size()-1]==';')
        scarto.pop_back();
    //cerco match tabella in questione
    for (int a = 0; a < tabelle.size (); a++) {
        if (tabelle[a]->getNome () == scarto) {
            trovata = true;
            pos_table = a;
            break;
        }
    }
    if (!trovata) {
        throw InexistentTable();
    } else {
        if(completo){
            for (int i = 0; i < tabelle[pos_table]->numCampi (); i++) {
                campi.push_back (tabelle[pos_table]->getCol (i)->getNomeColonna ());
            }
            for(string & elem : campi){
                cout << elem << "   ";
            }
            cout << endl;
        }
        if (word[word.size () - 1] == ';') {   //non c'è WHERE
            word.pop_back ();
            //stampo solo campi specificati della tabella
            for (const auto &elem : tabelle[pos_table]->returnData (campi)) {
                cout << elem << endl;
            }
            cout << endl;
        } else {
            stream_comando >> scarto; //WHERE o ORDER
            if(toUpper(scarto)=="ORDER"){
                ordinamento=true;
                stream_comando >> scarto; //scarto by
                stream_comando >> nome_colonna;   //campo ordinamento
                stream_comando >> ordine; //DESC o ASC
                ordine.pop_back();
                if (toUpper(ordine) == "ASC") {
                    for (int z = 0; z < tabelle[pos_table]->returnData(campi, nome_colonna, 1).size(); z++) {
                        cout << tabelle[pos_table]->returnData(campi, nome_colonna,1)[z] << endl;
                        if (tabelle[pos_table]->returnData(campi, nome_colonna,1).empty())
                            message->assign(valori_inesistenti);
                    }
                } else if (toUpper(ordine) == "DESC") {
                    for (int z = 0; z < tabelle[pos_table]->returnData(campi, nome_colonna, 3).size(); z++) {
                        cout << tabelle[pos_table]->returnData(campi, nome_colonna,3)[z] << endl;
                        if (tabelle[pos_table]->returnData(campi, nome_colonna,3).empty())
                            message->assign(valori_inesistenti);
                    }
                }
            } else {
                stream_comando >> word;  //in word c'è il campo condizione
                stream_comando >> word2;  //in word2 c'è l'operatore
                if (!belong_to(word2, operatori)) {
                    throw InvalidOperator();
                } else {
                    //casistica _operatori
                    if (toUpper(word2) == "BETWEEN") {
                        stream_comando >> condizione1;
                        if (condizione1[0] == '"') {
                            condizione1 = campoTesto(&stream_comando, condizione1);
                        }
                        else if (condizione1[0] == 39){ //se è un char tolgo le '
                            condizione1.pop_back();
                            condizione1.erase(0,1);
                        }
                        stream_comando >> scarto; //scarto and
                        stream_comando >> condizione2;
                        if (condizione2[0] == '"') {
                            condizione2 = campoTesto(&stream_comando, condizione2);
                        }
                        else if (condizione2[0] == 39){ //se è un char tolgo le '
                            if (condizione2[condizione2.size()-1] == ';')
                                condizione2.pop_back();
                            condizione2.pop_back();//'
                            condizione2.erase(0,1);//'
                        } else {
                            if (condizione2[condizione2.size()-1] == ';')
                                condizione2.pop_back(); //;
                        }
                        stream_comando >> scarto;  //scarto order
                        if (toUpper(scarto) == "ORDER") {
                            stream_comando >> scarto; //scarto by
                            stream_comando >> nome_colonna;   //campo ordinamento
                            stream_comando >> ordine; //DESC o ASC
                            ordine.pop_back();
                            if (toUpper(ordine) == "ASC") {
                                for (int z = 0;
                                     z < tabelle[pos_table]->returnData(campi, word2, condizione1, condizione2,
                                                                        nome_colonna, 1).size(); z++) {
                                    cout << tabelle[pos_table]->returnData(campi, word2, condizione1, condizione2,
                                                                           nome_colonna,
                                                                           1)[z] << endl;
                                    if (tabelle[pos_table]->returnData(campi, word2, condizione1, condizione2,
                                                                       nome_colonna,
                                                                       1).empty())
                                        message->assign(valori_inesistenti);
                                }
                            } else if (toUpper(ordine) == "DESC") {
                                for (int z = 0;
                                     z < tabelle[pos_table]->returnData(campi, word2, condizione1, condizione2,
                                                                        nome_colonna, 3).size(); z++) {
                                    cout << tabelle[pos_table]->returnData(campi, word2, condizione1, condizione2,
                                                                           nome_colonna,
                                                                           3)[z] << endl;
                                    if (tabelle[pos_table]->returnData(campi, word2, condizione1, condizione2,
                                                                       nome_colonna,
                                                                       3).empty())
                                        message->assign(valori_inesistenti);
                                }
                            }
                        }
                        else {
                            //stampo campi che rispettando condizione senza ordinamento
                            for (const auto &elem : tabelle[pos_table]->returnData(campi, word, condizione1,
                                                                           condizione2)) {
                                cout << elem << endl;
                            }
                            if (tabelle[pos_table]->returnData(campi, word, condizione1, condizione2).empty())
                                message->assign(valori_inesistenti);
                            cout << endl;
                        }
                    } else {
                        stream_comando >> word3; //in word3 c'è la condizione
                        if (word3[0] == '"') {
                            word3 = campoTesto(&stream_comando, word3);
                        }
                        else if (word3[0] == 39){ //se è un char tolgo le '
                            if (word3[word3.size()-1] == ';')
                                word3.pop_back();
                            word3.pop_back();//'
                            word3.erase(0,1);//'
                        } else {
                            if (word3[word3.size()-1] == ';')
                                word3.pop_back(); //;
                        }
                        stream_comando >> scarto;
                        if (toUpper(scarto) == "ORDER") {
                            ordinamento = true;
                            stream_comando >> scarto; //scarto by
                            stream_comando >> nome_colonna; //campo ordinamento
                            stream_comando >> ordine; //DESC o ASC
                            ordine.pop_back();
                        } else {
                            ordinamento = false;
                            //stampo campi che rispettando condizione senza ordinamento
                        }
                        if (word2 == "=") {
                            if (ordinamento) {
                                if (toUpper(ordine) == "ASC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 0, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 0, nome_colonna, 1)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 0, nome_colonna, 1).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                } else if (toUpper(ordine) == "DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 0, nome_colonna,
                                                                3).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 0, nome_colonna, 3)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 0, nome_colonna, 3).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                }
                            } else {
                                for (const auto &elem : tabelle[pos_table]->returnData(campi, word, word3, 0)) {
                                    cout << elem << endl;
                                }
                                if (tabelle[pos_table]->returnData(campi, word, word3, 0).empty())
                                    message->assign(valori_inesistenti);
                            }
                        } else if (word2 == "<") {
                            if (ordinamento) {
                                if (toUpper(ordine) == "ASC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 1, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 1, nome_colonna, 1)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word2, word3, 1, nome_colonna, 1).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                } else if (toUpper(ordine) == "DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 1, nome_colonna,
                                                                3).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 1, nome_colonna, 3)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 1, nome_colonna, 3).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                }
                            } else {
                                for (const auto &elem : tabelle[pos_table]->returnData(campi, word, word3, 1)) {
                                    cout << elem << endl;
                                }
                                if (tabelle[pos_table]->returnData(campi, word, word3, 1).empty())
                                    message->assign(valori_inesistenti);
                            }
                        } else if (word2 == "<=") {
                            if (ordinamento) {
                                if (toUpper(ordine) == "ASC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 2, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 2, nome_colonna, 1)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 2, nome_colonna, 1).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                } else if (toUpper(ordine) == "DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 2, nome_colonna,
                                                                3).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 2, nome_colonna, 3)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 2, nome_colonna, 3).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                }
                            } else {
                                for (const auto &elem : tabelle[pos_table]->returnData(campi, word, word3,2)) {
                                    cout << elem << endl;
                                }
                                if (tabelle[pos_table]->returnData(campi, word, word3, 2).empty())
                                    message->assign(valori_inesistenti);
                            }
                        } else if (word2 == ">") {
                            if (ordinamento) {
                                if (toUpper(ordine) == "ASC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 3, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 3, nome_colonna, 1)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 3, nome_colonna, 1).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                } else if (toUpper(ordine) == "DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 3, nome_colonna,
                                                                3).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 3, nome_colonna, 3)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 3, nome_colonna, 3).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                }
                            } else {
                                for (const auto &elem : tabelle[pos_table]->returnData(campi, word, word3, 3)) {
                                    cout << elem << endl;
                                }
                                if (tabelle[pos_table]->returnData(campi, word, word3, 3).empty())
                                    message->assign(valori_inesistenti);
                            }
                        } else if (word2 == ">=") {
                            if (ordinamento) {
                                if (toUpper(ordine) == "ASC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 4, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 4, nome_colonna, 1)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 4, nome_colonna, 1).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                } else if (toUpper(ordine) == "DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 4, nome_colonna,
                                                                3).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 4, nome_colonna, 3)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 4, nome_colonna, 3).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                }
                            } else {
                                for (const auto &elem : tabelle[pos_table]->returnData(campi, word, word3, 4)) {
                                    cout << elem << endl;
                                }
                                if (tabelle[pos_table]->returnData(campi, word, word3, 4).empty())
                                    message->assign(valori_inesistenti);
                                cout << endl;
                            }
                        } else if (word2 == "<>") {
                            if (ordinamento) {
                                if (toUpper(ordine) == "ASC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 5, nome_colonna,
                                                                1).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 5, nome_colonna, 1)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 5, nome_colonna, 1).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                } else if (toUpper(ordine) == "DESC") {
                                    for (int z = 0;
                                         z <
                                         tabelle[pos_table]->returnData(campi, word, word3, 5, nome_colonna,
                                                                3).size(); z++) {
                                        cout << tabelle[pos_table]->returnData(campi, word, word3, 5, nome_colonna, 3)[z]
                                             << endl;
                                        if (tabelle[pos_table]->returnData(campi, word, word3, 5, nome_colonna, 3).empty())
                                            message->assign(valori_inesistenti);
                                    }
                                }
                            } else {
                                for (const auto &elem : tabelle[pos_table]->returnData(campi, word, word3, 5)) {
                                    cout << elem << endl;
                                }
                                if (tabelle[pos_table]->returnData(campi, word, word3, 5).empty())
                                    message->assign(valori_inesistenti);
                                cout << endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

vector<Tabella*> Insert(vector<Tabella*> &tabelle, stringstream &stream_comando, string *message) {
    string scarto, word, nome_tabella;
    vector<string> campi, valori;
    int a, pos_table;
    bool trovata, flag_fine = false;
    stream_comando >> scarto;  //INTO
    stream_comando >> nome_tabella;
    for (a = 0, trovata = false; a < tabelle.size(); a++) {
        if (tabelle[a]->getNome() == nome_tabella) { ///
            trovata = true;
            pos_table = a;
            break;
        }
    }
    if(!trovata){
        throw InexistentTable();
    } else {
        getline(stream_comando, scarto, '(');
        //salvo n-1 campi
        stream_comando >> word;
        while (word[word.size()-1] != ')') {
            word.pop_back();
            campi.push_back(word);
            stream_comando >> word;
        }

        //leggo e salvo l'ultimo campo
        word.pop_back(); //tolgo la )
        campi.push_back(word);

        stream_comando >> scarto;   //VALUES

        //salvo n-1 valori
        stream_comando >> word;
        word.erase(0,1); //tolgo (

        while (!flag_fine) {
            if (word[0] == '"'){
                word = campoTesto(&stream_comando,word);
                if (word[word.size()-1] == ')'){
                    flag_fine = true;
                    word.pop_back(); // )
                }
                word.pop_back(); // "
            }
            else if (word[0] == 39){
                if (word[word.size()-1] == ';'){
                    flag_fine = true;
                    word.pop_back(); // ;
                }
                word.pop_back(); // , o )
                word.pop_back(); // ' fine
                word.erase(0,1); // ' inizio
            }
            else {
                if (word[word.size()-1] == ';'){
                    flag_fine = true;
                    word.pop_back(); //;
                }
                word.pop_back(); // ) o ,
            }
            valori.push_back(word);
            stream_comando >> word;
        }
        tabelle[pos_table]->addRecord(campi, valori); ///segmentation fault
        (*message) = "Operazione completata: Record aggiunto correttamente alla tabella";
        return tabelle;
    }
}


#endif //PROGETTO_COLONNE_COMANDI_HPP
