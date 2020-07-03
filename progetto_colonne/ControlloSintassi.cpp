//
// Created by Elisa Messina on 15/06/2020.
//

#include "ControlloSintassi.h"
#include <sstream>
#include <vector>
#define ERR_COMANDO -1

enum comando {CREATE, DROP, INSERT, DELETE, TRUNCATE, UPDATE, SELECT,QUIT};

char ControlloSintassi::GestioneTesto(stringstream *comando, string &word) {
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
            (*comando) >> carattere;
            if (carattere == 34) {
                contatore_virgolette++;
            } else {
                if (contatore_virgolette % 2 != 0) {
                    fine_testo = true;
                }
                contatore_virgolette = 0;
            }
        }
    } else
        carattere = word[k];
    return carattere;
}

string ControlloSintassi::toUp(string word) {
    for (auto & c : word)
        c = toupper(c);
    return word;
}

bool ControlloSintassi::belongs_to(string &to_be_compared, const vector<string>& insieme) {
    for(const auto & elem : insieme){
        if(toUp(to_be_compared) == elem)
            return true;
    }
    return false;
}


ControlloSintassi::ControlloSintassi() {
    _message_error = "Error: Errore di sintassi nel comando, riprovare!";
    _message_error_keyword = "Error: Utilizzo inappropriato di una parola chiave del linguaggio, riprovare!";
    _inexistent_type = "Error: Tipo assegnato non esistente, riprovare!";
    _message_error_key = "Error: Specificare tutti i campi prima dell'inserimento delle chiavi, riprovare!";
    _missing_pk = "Error: Primary Key non specificata! Riprovare";
    _duplicate_col = "Error: Due colonne di una stessa tabella non possono avere lo stesso nome, riprovare!";
}



bool ControlloSintassi::controlloCreate(stringstream &comando, string* messaggio) {
    string word;
    char c;
    bool comando_corretto = false, chiave_trovata = false, chiave_primaria_trovata = false;
    vector<string> words, nomi_colonne;
    comando >> word >> word;
    word = toUp(word);
    if (word == "TABLE"){ //controllo che la seconda parola dopo create sia table
        comando >> word;
        if (word[word.size()-1] == '(') //caso create table nome(
            word.pop_back();
        else
            comando >> c; //caso create table nome (
        if (!belongs_to(word,_keywords)){ //controllo che il nome assegnato alla tabella non sia una keyword del linguaggio
            if (c == '('){ //controllo la presenza della (
                while(!comando.eof()){ //finchè non finisce il comando
                    comando_corretto = false;
                    words.clear();
                    int i = 0;
                    words.emplace_back();
                    comando >> words[i];

                    if (toUp(words[0]) == "PRIMARY") { //caso in cui la prima parola della riga sia primary
                        chiave_trovata = true;
                        chiave_primaria_trovata = true;
                        while(words[i][words[i].size()-1] != ')' && words[i][words[i].size()-1] != ';') { //leggo tutta la riga e salvo le parole lette per ogni riga nel vettore di stringhe words
                            i++;
                            words.emplace_back();
                            comando >> words[i];
                        }
                        if (toUp(words[1]) == "KEY"){
                            if ( (words[words.size()-1][words[words.size()-1].size()-1] == ')' && words[words.size()-1][0] == '(') || //caso in cui ci sia uno spazio primary key (id) ); oppure ci sia dopo la foreign key
                                 (words[words.size()-1][0] == '(' &&  words[words.size()-1][words[words.size()-1].size()-1] == ';' && words[words.size()-1][words[words.size()-1].size()-2] == ')') ) { //caso in cui sia la fine del comando e non ci sia uno spazio primary key (id));
                                comando_corretto = true;
                            }
                            else{
                                messaggio->assign(_message_error);
                                return false;
                            }
                        }
                        else{
                            messaggio->assign(_message_error);
                            return false;
                        }
                    }

                    else if (toUp(words[0]) == "FOREIGN"){
                        chiave_trovata = true;
                        int contatore_parentesi = 0;
                        while(contatore_parentesi < 2 && words[i][words[i].size()-1] != ';') { //leggo tutta la riga e salvo le parole lette per ogni riga nel vettore di stringhe words
                            if (words[i][words[i].size()-1] == ')')
                                contatore_parentesi++;
                            if (contatore_parentesi < 2 ){
                                i++;
                                words.emplace_back();
                                comando >> words[i];
                            }
                        }
                        if (toUp(words[1]) == "KEY"){
                            if ((words[2][0]) == '(' && (words[2][words[2].size()-1]) == ')'){
                                if (toUp(words[3]) == "REFERENCES"){
                                    if ( (words[5][0]) == '(' && ( words[5][words[5].size()-1] == ')' ||  //caso di uno spazio
                                       ( (words[5][words[5].size()-1] == ';' && words[5][words[5].size()-2] == ')' && words[5][words[5].size()-3] == ')') ) ) ) //caso senza spazio
                                        comando_corretto = true;
                                    else
                                    {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                }
                                else{
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                            }
                            else{
                                messaggio->assign(_message_error);
                                return false;
                            }
                        }
                        else{
                            messaggio->assign(_message_error);
                            return false;
                        }
                    }

                    else if (words[0] != ");") {//se la prima parola di una riga non è nè foreign nè primary allora è per forza il nome di una colonna
                        if (!chiave_trovata){
                        while(words[i][words[i].size()-1] != ',' && words[i][words[i].size()-1] != ';') { //leggo tutta la riga e salvo le parole lette per ogni riga nel vettore di stringhe words
                            i++;
                            words.emplace_back();
                            comando >> words[i];
                        }
                        if (!belongs_to(words[0], _keywords)) { //controllo che non sia una keyword
                            if (!belongs_to(words[0], nomi_colonne)) {
                                nomi_colonne.push_back(words[0]);
                                if (words[words.size() - 1][words[words.size() - 1].size() - 1] ==
                                    ',') { //controllo che ci sia la virgola a fine riga
                                    words[words.size() - 1].pop_back(); //una volta appurato che la virgola c'è la tolgo
                                    if (belongs_to(words[1],
                                                   _tipi)) { //controllo che il tipo della colonna sia fra quelli permessi
                                        if (words[1] != words[words.size() -
                                                              1]) { //se il tipo non è l'ultima parola della riga vado avanti
                                            for (int j = 2; j < words.size(); j++) {
                                                words[j] = toUp(words[j]);
                                                if (j != (words.size() -
                                                          1)) { //se non si è ancora arrivati all'ultima parola
                                                    if ((toUp(words[j]) == "NOT" && toUp(words[j + 1]) == "NULL")
                                                        || (toUp(words[j]) == "NULL" && toUp(words[j - 1]) == "NOT")
                                                        || (toUp(words[j]) == "AUTO_INCREMENT")) {
                                                        if (toUp(words[j]) == "AUTO_INCREMENT") {
                                                            if (toUp(words[1]) != "INT") {
                                                                throw FormatTypeError();
                                                            }
                                                        }
                                                    } else {
                                                        messaggio->assign(_message_error);
                                                        return false;
                                                    }
                                                } else { //se invece si è arrivati all'ultima parola
                                                    if ((toUp(words[j]) == "NULL" && toUp(words[j - 1]) == "NOT") ||
                                                        (toUp(words[j]) == "AUTO_INCREMENT")) {
                                                        if ((toUp(words[j]) == "AUTO_INCREMENT")) {
                                                            if (toUp(words[1]) != "INT") {
                                                                throw FormatTypeError();
                                                            }
                                                        }
                                                        comando_corretto = true;
                                                    } else {
                                                        messaggio->assign(_message_error);
                                                        return false;
                                                    }
                                                }
                                            }
                                        } else
                                            comando_corretto = true;
                                    } else {
                                        messaggio->assign(_inexistent_type);
                                        return false;
                                    }
                                } else {
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                            }
                            else {
                                messaggio->assign(_duplicate_col);
                                return false;
                            }
                        }
                        else {
                            messaggio->assign(_message_error_keyword);
                            return false;
                        }
                        }else {
                            messaggio->assign(_message_error_key);
                            return false;
                        }
                    }
                    if (!comando_corretto) //se alla fine di un ciclo di un while (fine di una riga) il comando non è corretto ritorno il messaggio di errore
                        break;
                }
            } else {
                messaggio->assign(_message_error);
                return false;
            }
        }
        else{
            messaggio->assign(_message_error_keyword);
            return false;
        }
    }
    else{
        messaggio->assign(_message_error);
        return false;
    }
    if (chiave_primaria_trovata){
        if (comando_corretto){
            return true;
        }
        else {
            if (words[0] == ");")
                return true;
            else{
                messaggio->assign(_message_error);
                return false;
            }
        }
    }
    else {
        messaggio->assign(_missing_pk);
        return false;
    }
}

bool ControlloSintassi::controlloTruncate(stringstream &comando, string *messaggio){
    vector<string> words;
    int i = 0;
    words.emplace_back();
    comando >> words[0];
    while(words[i][words[i].size()-1]!=';'){//salvo riga del comando fino al punto e virgola
        i++;
        words.emplace_back();
        comando >> words[i];
    }
    if(words.size()!= 3){
        messaggio->assign(_message_error);
        return false;
    }
    else{
        if(toUp(words[1])!= "TABLE"){
            messaggio->assign(_message_error);
            return false;
        }
        else{
            if (words[2][words[2].size()-1] != ';') { //controllo che alla fine del nome della tabella ci sia il ;
                messaggio->assign(_message_error);
                return false;
            }
            else
                return true;
        }
    }
}

bool ControlloSintassi::controlloDrop(stringstream &comando, string *messaggio){
    vector<string> words;
    int i = 0;
    words.emplace_back();
    comando >> words[0];
    while(words[i][words[i].size()-1]!=';'){//salvo riga del comando fino al punto e virgola
        i++;
        words.emplace_back();
        comando >> words[i];
    }
    if(words.size()!= 3 ){ //comando del tipo DROP TABLE <NOMETABELLA>; (DROP già controllato)
        messaggio->assign(_message_error);
        return false;
    }
    else {
        if(toUp(words[1])!= "TABLE"){
            messaggio->assign(_message_error);
            return false;
        }
        else{
            if (words[2][words[2].size()-1] == ';') { //controllo che alla fine del nome della tabella ci sia il ;
                return true;
            }
            else {
                messaggio->assign(_message_error);
                return false;
            }
        }
    }
}

bool ControlloSintassi::controlloInsert (stringstream &comando, string *messaggio) {
    string word;
    char carattere;
    bool fine_testo = false, flag_fine_comando = false;
    int contatore_virgolette;
    vector<string> words;
    comando >> word >> word;
    if (toUp (word) == "INTO") {
        comando >> word;
        int i = 0;
        words.emplace_back ();
        comando >> words[i];
        if (words[i][0] == '(') { //verifico che ci sia il carattere (
            words[i].erase (0, 1); //tolgo la (
            while (words[i][words[i].size () - 1] != ')') {
                if (words[i][words[i].size () - 1] != ',' &&
                    words[i][words[i].size () - 1] != ')') {
                    messaggio -> assign (_message_error);
                    return false;
                } else {
                    words[i].pop_back (); //tolgo la virgola
                    i ++;
                    words.emplace_back ();
                    comando >> words[i];
                }
            }
            comando >> word;
            if (toUp (word) != "VALUES") {
                messaggio -> assign (_message_error);
                return false;
            } else {
                words.clear ();
                int j = 0;
                words . emplace_back ();
                comando >> words[j];
                if (words[j][0] != '(') {
                    messaggio -> assign (_message_error);
                    return false;
                } else {
                    words[j] . erase (0, 1); //tolgo (
                    while (! flag_fine_comando) {
                        if (words[j][words[j] . size () - 1] == ',' || words[j][words[j] . size () - 1] == ';' ||
                            (words[j][0] == 34) || (words[j][0] == 39)) { //togliere seconda condizione magari
                            if (words[j][0] == '"') { //caso di un campo di testo
                                carattere = GestioneTesto (&comando, words[j]);
                                if (carattere != ',' && carattere != ')') {
                                    messaggio -> assign (_message_error);
                                    return false;
                                } else if (carattere == ')') {
                                    comando >> carattere;
                                    if (carattere == ';' || words[j][words[j] . size () - 1] == ';') {
                                        flag_fine_comando = true;
                                    }
                                }
                            } else if (words[j][0] == 39) { //caso di un campo char 'c', oppure 'c'); se fine comando
                                if (words[j][words[j] . size () - 1] == ',') {
                                    if (words[j] . size () !=
                                        4) { //tipo 1: se la dimensione è > 4 vuol dire che ho messo dei caratteri in più es. 'ciao', non va bene
                                        messaggio -> assign (_message_error);
                                        return false;
                                    } else {
                                        words[j] . pop_back (); //tolgo la ,
                                        if (words[j][words[j] . size () - 1] !=
                                            39) { //controllo che venga chiuso il campo char
                                            messaggio -> assign (_message_error);
                                            return false;
                                        }
                                    }
                                } else if (words[j][words[j] . size () - 1] ==
                                           ';') { //vuol dire che si è arrivati alla fine del comando
                                    if (words[j] . size () !=
                                        5) { //tipo 1: se la dimensione è > 5 vuol dire che ho messo dei caratteri in più es. 'ciao'); non va bene
                                        messaggio -> assign (_message_error);
                                        return false;
                                    }
                                    if (words[j][words[j] . size () - 2] == ')') {
                                        if (words[j][words[j] . size () - 3] == 39) {
                                            flag_fine_comando = true;
                                        } else {
                                            messaggio -> assign (_message_error);
                                            return false;
                                        }
                                    } else {
                                        messaggio -> assign (_message_error);
                                        return false;
                                    }
                                } else {
                                    messaggio -> assign (_message_error);
                                    return false;
                                }
                            } else { //resto casi: 20, o 20);
                                if (words[j][words[j] . size () - 1] != ',') {
                                    if (words[j][words[j] . size () - 1] != ';') {
                                        messaggio -> assign (_message_error);
                                        return false;
                                    } else {
                                        if (words[j][words[j] . size () - 2] != ')') {
                                            messaggio -> assign (_message_error);
                                            return false;
                                        } else
                                            flag_fine_comando = true;
                                    }
                                }
                            }
                        } else {
                            messaggio -> assign (_message_error);
                            return false;
                        }
                        if (! flag_fine_comando) {
                            j ++;
                            words . emplace_back ();
                            comando >> words[j];
                        }
                    }
                }
            }
        } else {
            messaggio -> assign (_message_error);
            return false;
        }
    } else {
        messaggio -> assign (_message_error);
        return false;
    }
    if (! flag_fine_comando) { //ulteriore controllo
        messaggio -> assign (_message_error);
        return false;
    } else return true;
}

bool ControlloSintassi::controlloDelete(stringstream &comando, string *messaggio){
    string word;
    char carattere;
    comando >> word >> word;
    if (toUp(word) == "FROM"){
        comando >> word;
            comando >> word;
            if (toUp(word) == "WHERE"){ //deve esserci per forza la condizione altrimenti il comando sarebbe truncate
                comando >> word >> word;
                    if (toUp(word) == "BETWEEN") {
                        comando >> word;
                        if (word[0] == '"') {//caso di un campo testo
                            carattere = GestioneTesto(&comando, word);
                            if (toupper(carattere) != 'A') {
                                messaggio->assign(_message_error);
                                return false;
                            } else {
                                comando >> word;
                                word.insert(0, 1, carattere);
                            }
                        } else if (word[0] == 39) { //'c'
                            if (word.size() != 3) {
                                messaggio->assign(_message_error);
                                return false;
                            }
                            comando >> word;
                        }
                        else
                            comando >> word;
                        if (toUp(word) == "AND") {
                            comando >> word;
                            if (word[0] == '"') {//caso di un campo testo
                                carattere = GestioneTesto(&comando, word);
                                if (carattere == ';')
                                    return true;
                                else {
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                            } else {
                                if (word[0] == 39) {
                                    if (word.size() != 3) {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                }
                                if (word[word.size()-1] == ';')
                                    return true;
                                else {
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                            }
                        }
                    }
                    else {
                        comando >> word;
                        if (word[0] == '"') { //caso di un campo testo
                            carattere = GestioneTesto(&comando, word);
                            if (carattere != ';') {
                                messaggio->assign(_message_error);
                                return false;
                            } else
                                return true;
                        } else { //campo char o qualsiasi altro: unica parola per forza
                            if (word[word.size() - 1] != ';') {
                                messaggio->assign(_message_error);
                                return false;
                            } else
                                return true;
                        }
                    }
            }
            else {
                messaggio->assign(_message_error);
                return false;
            }
    } else {
        messaggio->assign(_message_error);
        return false;
    }
    messaggio->assign(_message_error);
    return false;
}

bool ControlloSintassi::controlloUpdate(stringstream &comando, string *messaggio)  {
    string word;
    char carattere;
    bool fine_testo = false, flag_fine_comando = false, flag_condizione = false;
    int contatore_virgolette;
    vector<string> words;
    comando >> word >> word;
        comando >> word;
        if (toUp(word) == "SET"){
            comando >> word;
            while (!flag_fine_comando) {
                if (!flag_condizione){ //non si è ancora arrivati al where
                    comando >> word >> word; //operatore =, dato da inserire
                        if (word[0] == '"'){ //caso di un campo testo
                            carattere = GestioneTesto(&comando,word);
                            if (toupper(carattere) == 'W') {
                                flag_condizione = true;
                                comando >> word; //campo o Where
                                word.insert(0,1,carattere);
                            }
                            else if (carattere != ',' && toupper(carattere) != 'W'){
                                messaggio->assign(_message_error);
                                return false;
                            }
                            else {
                                comando >> word;
                            }
                        }
                        else { //campo char o qualsiasi altro: unica parola per forza
                             if (word[word.size()-1] == ',') {
                                 if (word[0] == 39) { //se è un char controllo la sintassi giusta
                                     if (word.size() != 4) {
                                         messaggio->assign(_message_error);
                                         return false;
                                     } else {
                                         word.pop_back(); //tolgo ,
                                         if (word[word.size() - 1] != 39) {
                                             messaggio->assign(_message_error);
                                             return false;
                                         }
                                     }
                                 }
                             }
                             else{
                                 if (word[0] == 39) { //se è un char controllo la sintassi giusta
                                     if (word.size() != 3) {
                                         messaggio->assign(_message_error);
                                         return false;
                                     } else {
                                         if (word[word.size() - 1] != 39) {
                                             messaggio->assign(_message_error);
                                             return false;
                                         }
                                     }
                                 }
                                 flag_condizione = true;
                             }
                             comando >> word;
                        }
                }
                else { //si è arrivati al where
                    if (toUp(word) == "WHERE"){
                        comando >> word >> word;
                            if (toUp(word) == "BETWEEN") {
                                comando >> word;
                                if (word[0] == '"') {//caso di un campo testo
                                    carattere = GestioneTesto(&comando, word);
                                    if (toupper(carattere) != 'A') {
                                        messaggio->assign(_message_error);
                                        return false;
                                    } else {
                                        comando >> word;
                                        word.insert(0, 1, carattere);
                                    }
                                } else if (word[0] == 39) { //'c'
                                    if (word.size() != 3) {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                    comando >> word;
                                }
                                else
                                    comando >> word;
                                if (toUp(word) == "AND") {
                                    comando >> word;
                                    if (word[0] == '"') {//caso di un campo testo
                                        carattere = GestioneTesto(&comando, word);
                                        if (carattere == ';')
                                            flag_fine_comando = true;
                                        else {
                                            messaggio->assign(_message_error);
                                            return false;
                                        }
                                    } else {
                                        if (word[0] == 39) {
                                            if (word.size() != 3) {
                                                messaggio->assign(_message_error);
                                                return false;
                                            }
                                        }
                                        if (word[word.size()-1] == ';')
                                            flag_fine_comando = true;
                                        else {
                                            messaggio->assign(_message_error);
                                            return false;
                                        }
                                    }
                                }
                            }
                            else {
                                comando >> word;
                                if (word[0] == '"') { //caso di un campo testo
                                    carattere = GestioneTesto(&comando, word);
                                    if (carattere == ';') {
                                        flag_fine_comando = true;
                                    } else {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                } else { //campo char o qualsiasi altro: unica parola per forza
                                    if (word[word.size() - 1] != ';') {
                                        messaggio->assign(_message_error);
                                        return false;
                                    } else {
                                        if (word[0] ==
                                            39) { //se è un char controllo he l'utente abbia effettivamente inserito solo un carattere con la sintassi giusta
                                            if (word.size() != 4) {
                                                messaggio->assign(_message_error);
                                                return false;
                                            } else {
                                                word.pop_back(); //tolgo ;
                                                if (word[word.size() - 1] != 39) {
                                                    messaggio->assign(_message_error);
                                                    return false;
                                                } else
                                                    flag_fine_comando = true;
                                            }
                                        } else
                                            flag_fine_comando = true;
                                    }
                                }
                            }
                    }
                    else {
                        messaggio->assign(_message_error);
                        return false;
                    }
                }
            }
        }
        else {
            messaggio->assign(_message_error);
            return false;
        }
    if (!flag_fine_comando){
        messaggio->assign(_message_error_keyword);
        return false;
    }
    else return true;
}

bool ControlloSintassi::controlloSelect(stringstream &comando, string *messaggio) {
    string word;
    char carattere;
    bool flag_asterisco = false, flag_fine_comando = false, flag_ordinamento = false, flag_condizione = false;
    int contatore_virgolette;
    vector<string> words;
    comando >> word >> word;
    if (word == "*"){ //caso di stampa tutte le colonne
        flag_asterisco = true;
    }
    else { //caso in cui vengono specificati solo alcuni campi
        while (word[word.size() - 1] == ',') {
            comando >> word;
        }
    }
    comando >> word;
    if (toUp(word) == "FROM") {
        comando >> word;
        if (word[word.size() - 1] == ';') { //fine comando del tipo select <colonne> from <tabella>;
            flag_fine_comando = true;
        } else { //se c'è una condizione e/o un ordinamento
                comando >> word;
                if (toUp(word) == "WHERE") {
                    flag_condizione = true;
                } else if (toUp(word) == "ORDER") {
                    flag_ordinamento = true;
                } else {
                    messaggio->assign(_message_error);
                    return false;
                }
                if (flag_condizione) {
                    comando >> word >> word;
                        if (toUp(word) == "BETWEEN") {
                            comando >> word;
                            if (word[0] == '"') {//caso di un campo testo
                                carattere = GestioneTesto(&comando, word);
                                if (toupper(carattere) != 'A') {
                                    messaggio->assign(_message_error);
                                    return false;
                                } else {
                                    comando >> word;
                                    word.insert(0, 1, carattere);
                                }
                            } else if (word[0] == 39) { //'c'
                                if (word.size() != 3) {
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                                comando >> word;
                            }
                            else
                                comando >> word;
                            if (toUp(word) == "AND") {
                                comando >> word;
                                if (word[0] == '"') {//caso di un campo testo
                                    carattere = GestioneTesto(&comando, word);
                                    if (carattere == ';') {
                                        flag_fine_comando = true;
                                    } else if (toupper(carattere) == 'O'){
                                        comando >> word;
                                        word.insert(0,1,carattere);
                                        if (toUp(word) == "ORDER"){
                                            flag_ordinamento = true;
                                        }
                                        else {
                                            messaggio->assign(_message_error);
                                            return false;
                                        }
                                    }
                                    else {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                } else {
                                    if (word[0] == 39) { //'c'
                                        if (word.size() != 3) {
                                            messaggio->assign(_message_error);
                                            return false;
                                        }
                                    }
                                    if (word[word.size()-1] == ';')
                                        flag_fine_comando = true;
                                    else {
                                        comando >> word;
                                        if (toUp(word) == "ORDER"){
                                            flag_ordinamento = true;
                                        }
                                        else {
                                            messaggio->assign(_message_error);
                                            return false;
                                        }
                                    }
                                }
                            }
                            else {
                                messaggio->assign(_message_error);
                                return false;
                            }
                        }
                        else { //operatore where diverso da between
                            comando >> word;
                            if (word[0] == '"') {//caso di un campo testo
                                carattere = GestioneTesto(&comando, word);
                                if (carattere == ';') {
                                    flag_fine_comando = true;
                                } else if (toupper(carattere) == 'O') {
                                    comando >> word;
                                    word.insert(0,1,carattere);
                                    if (toUp(word) == "ORDER"){
                                        flag_ordinamento = true;
                                    }
                                    else {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                }
                                else {
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                            } else {
                                if (word[0] == 39) { //'c'
                                    if (word.size() != 3) {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                }
                                if (word[word.size()-1] == ';')
                                    flag_fine_comando = true;
                                else {
                                    comando >> word;
                                    if (toUp(word) == "ORDER"){
                                        flag_ordinamento = true;
                                    }
                                    else {
                                        messaggio->assign(_message_error);
                                        return false;
                                    }
                                }
                            }
                        }
                }
                if (flag_ordinamento){
                    comando >> word;
                    if (toUp(word) == "BY"){
                        comando >> word >> word;
                        if (word[word.size()-1] == ';'){
                            word.pop_back();
                            if (toUp(word) == "DESC" || toUp(word) == "ASC"){
                                flag_fine_comando = true;
                            }
                            else {
                                messaggio->assign(_message_error);
                                return false;
                            }
                        }
                        else {
                            messaggio->assign(_message_error);
                            return false;
                        }
                    }
                    else {
                        messaggio->assign(_message_error);
                        return false;
                    }
                }
        }
    }
    else {
        messaggio->assign(_message_error);
        return false;
    }
    if (flag_fine_comando){
        return true;
    }
    else {
        messaggio->assign(_message_error);
        return false;
    }
}

string ControlloSintassi::Gestione_caratteri_speciali(string &comando, string *status_message) {
    bool found_text = false, campo_testo_presente = false, inside_testo = false;
    int contatore_virgolette = 0;
    vector<int> pos_first, pos_last;
    for (int j = 1; j < comando.size(); ++j) { //'"'
        if (!found_text){
            if (comando[j] == 34 && ((int)comando[j-1] != 39 && (int)comando[j+1] != 39)) { //se trovo un " e non è all'interno di un campo char
                found_text = true;
                campo_testo_presente = true;
                pos_first.push_back(j);
            }
        } // numero pari di " --> sono uscito, numero dispari --> non sono uscito
        else {
            if(comando[j] == 34) {
                contatore_virgolette++;
            }else{
                if(contatore_virgolette % 2 != 0){
                    found_text=false;
                    pos_last.push_back(j-1);
                }
                contatore_virgolette=0;
            }
        }
    }

    //controllo sui ';' nei campi di testo
    for (int k = 0; k < comando.size(); ++k) {
        inside_testo = false;
        if (!campo_testo_presente){
            //se trovo ';' e non è un campo char, trascuro il resto del comando perchè devo fermarmi
            if ( k!= comando.size()-1 && comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39) ){
                comando.erase(k+1);
            }
        }
        else{
            if (pos_first.size() != pos_last.size())
                (*status_message)="ERR: campo di testo non chiuso da apposite virgolette";
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
    return comando;
}

