//
// Created by Elisa Messina on 15/06/2020.
//

#include "ControlloSintassi.h"
#include <sstream>
#include <vector>
#define ERR_COMANDO -1

enum comando {CREATE, DROP, INSERT, DELETE, TRUNCATE, UPDATE, SELECT,QUIT};
enum tipo {INT, FLOAT, CHAR, TEXT, DATE, TIME};


string toUp(string word) {
    for (auto & c : word)
        c = toupper(c);
    return word;
}

int compare_tipo(string &tipo){
    tipo = toUp(tipo);
    if(tipo == "INT")
        return INT;
    else if (tipo == "FLOAT")
        return FLOAT;
    else if (tipo == "CHAR")
        return CHAR;
    else if (tipo == "TEXT")
        return TEXT;
    else if (tipo == "DATE")
        return DATE;
    else if (tipo == "TIME")
        return TIME;
    else
        return ERR_COMANDO;
}




ControlloSintassi::ControlloSintassi() {
    _message_error = "ERR: Errore di sintassi nel comando, riprovare!";
    _message_error_keyword = "ERR: Utilizzo inappropriato di una parola chiave del linguaggio, riprovare!";
    _wrong_type_auto_increment = "ERR: Tipo invalido per l'utilizzo della keyword AUTO_INCREMENT, riprovare!";
    _inexistent_type = "ERR: Tipo assegnato non esistente, riprovare!";
    _invalid_operator = "ERR: Operatore non valido, riprovare!";
    _message_error_key = "ERR: Specificare tutti i campi prima dell'inserimento delle chiavi, riprovare!";
    _missing_pk = "ERR: Primary Key non specificata! Riprovare";
}

bool ControlloSintassi::belongs_to_keywords(string &to_be_compared) const{
    to_be_compared = toUp(to_be_compared);
    bool _keyword_trovata = false;
    for (const auto & _keyword : _keywords){
        if (to_be_compared == _keyword)
            _keyword_trovata = true;
    }
    return _keyword_trovata;
}

bool ControlloSintassi::belongs_to_operatori(string &to_be_compared) const{
    to_be_compared = toUp(to_be_compared);
    bool _op_trovato = false;
    for (const auto & op : operatori){
        if (to_be_compared == op)
            _op_trovato = true;
    }
    return _op_trovato;
}

bool ControlloSintassi::controlloCreate(stringstream &comando, string* messaggio) const {
    string word;
    char c;
    bool comando_corretto = false, chiave_trovata = false, chiave_primaria_trovata = false;
    vector<string> words;
    comando >> word >> word;
    word = toUp(word);
    if (word == "TABLE"){ //controllo che la seconda parola dopo create sia table
        comando >> word;
        if (!belongs_to_keywords(word)){ //controllo che il nome assegnato alla tabella non sia una keyword del linguaggio
            comando >> c;
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
                        if (!belongs_to_keywords(words[0])){ //controllo che non sia una keyword
                            if (words[words.size()-1][words[words.size()-1].size()-1] == ','){ //controllo che ci sia la virgola a fine riga
                                words[words.size()-1].pop_back(); //una volta appurato che la virgola c'è la tolgo
                                if (compare_tipo(words[1]) != -1){ //controllo che il tipo della colonna sia fra quelli permessi
                                    if (words[1] != words[words.size()-1]){ //se il tipo non è l'ultima parola della riga vado avanti
                                        for (int j = 2; j < words.size(); j++){
                                            words[j] = toUp(words[j]);
                                            if (j != (words.size()-1)){ //se non si è ancora arrivati all'ultima parola
                                                if ( (toUp(words[j]) == "NOT" && toUp(words[j+1]) == "NULL")
                                                || (toUp(words[j]) == "NULL" && toUp(words[j-1]) == "NOT")
                                                || (toUp(words[j]) == "AUTO_INCREMENT")){
                                                    if (toUp(words[j]) == "AUTO_INCREMENT"){
                                                        if (compare_tipo(words[1]) != INT){
                                                            messaggio->assign(_wrong_type_auto_increment);
                                                            return false;//_wrong_type_auto_increment;
                                                        }
                                                    }
                                                }
                                                else{
                                                    messaggio->assign(_message_error);
                                                    return false;
                                                }
                                            }
                                            else { //se invece si è arrivati all'ultima parola
                                                if ((toUp(words[j]) == "NULL" && toUp(words[j-1]) == "NOT") ||
                                                (toUp(words[j]) == "AUTO_INCREMENT")){
                                                    if ((toUp(words[j]) == "AUTO_INCREMENT")){
                                                        if (compare_tipo(words[1]) != INT){
                                                            messaggio->assign(_wrong_type_auto_increment);
                                                            return false;//_wrong_type_auto_increment;
                                                        }
                                                    }
                                                    comando_corretto = true;
                                                }
                                                else{
                                                    messaggio->assign(_message_error);
                                                    return false;
                                                }
                                            }
                                        }
                                    }
                                    else
                                        comando_corretto = true;
                                }
                                else{
                                    messaggio->assign(_inexistent_type);
                                    return false;
                                }
                            }
                            else{
                                messaggio->assign(_message_error);
                                return false;
                            }
                        }
                        else{
                            messaggio->assign(_message_error_keyword);
                            return false;
                        }
                        } else {
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
            messaggio->assign(_message_error);
            return false;
        }
    }
    else {
        messaggio->assign(_missing_pk);
        return false;
    }
}

bool ControlloSintassi::controlloTruncate(stringstream &comando, string *messaggio) const {
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
            if (words[2][words[2].size()-1] == ';') { //controllo che alla fine del nome della tabella ci sia il ;
                words[2].pop_back(); //se c'è lo tolgo
                if (!belongs_to_keywords(words[2]))
                    return true;
                else {
                    messaggio->assign(_message_error_keyword);
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

bool ControlloSintassi::controlloDrop(stringstream &comando, string *messaggio) const {
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
                words[2].pop_back(); //se c'è lo tolgo
                if (!belongs_to_keywords(words[2]))
                    return true;
                else {
                    messaggio->assign(_message_error_keyword);
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

bool ControlloSintassi::controlloInsert(stringstream &comando, string *messaggio) const{
    string word;
    char carattere;
    bool fine_testo = false, flag_fine_comando = false;
    int contatore_virgolette;
    vector<string> words;
    comando >> word >> word;
    if (toUp(word) == "INTO"){
        comando >> word;
        if (!belongs_to_keywords(word)) {
            int i = 0;
            words.emplace_back();
            comando >> words[i];
            if (words[i][0] == '(') { //verifico che ci sia il carattere (
                words[i].erase(0,1); //tolgo la (
                    while (words[i][words[i].size() - 1] != ')') {
                        if (words[i][words[i].size() - 1] != ',' && words[i][words[i].size() - 1] != ')') { //provare anche a togliere seconda condizione
                            messaggio->assign(_message_error);
                            return false;
                        }
                        words[i].pop_back(); //tolgo la virgola
                        if (belongs_to_keywords(words[i])) {
                            messaggio->assign(_message_error_keyword);
                            return false;
                        }
                        i++;
                        words.emplace_back();
                        comando >> words[i];
                    }
                    comando >> word;
                    if (toUp(word) != "VALUES") {
                        messaggio->assign(_message_error);
                        return false;
                    } else {
                        words.clear();
                        int j = 0;
                        words.emplace_back();
                        comando >> words[j];
                        if (words[j][0] != '(') {
                            messaggio->assign(_message_error);
                            return false;
                        } else {
                            words[j].erase(0,1); //tolgo (
                            while (words[j][words[j].size()-1] != ';' && !flag_fine_comando){
                                if (words[j][words[j].size() - 1] == ',' || words[j][words[j].size() - 1] != ';' || (words[j][0] == 34) || (words[j][0] == 39)) { //togliere seconda condizione magari
                                    if (words[j][0] == '"') { //caso di un campo di testo
                                        contatore_virgolette = 0;
                                        fine_testo = false;
                                        int k;
                                        for (k = 1; k < words[j].size(); k++){
                                            if (words[j][k] == 34) {
                                                contatore_virgolette++;
                                            } else {
                                                if (contatore_virgolette % 2 != 0) {
                                                    fine_testo = true;
                                                }
                                                contatore_virgolette = 0;
                                            }
                                        }
                                        if (!fine_testo) {
                                            while (!fine_testo) {
                                                comando >> carattere;
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
                                            carattere = words[j][k-1];
                                        if (carattere != ',' && carattere != ')') {
                                            messaggio->assign(_message_error);
                                            return false;
                                        } else if (carattere == ')') {
                                            comando >> carattere;
                                            if (carattere == ';') {
                                                flag_fine_comando = true;
                                            }
                                        }
                                    } else if (words[j][0] == 39) { //caso di un campo char 'c', oppure 'c'); se fine comando
                                        if (words[j][words[j].size()-1] == ','){
                                            if (words[j].size() != 4){ //tipo 1: se la dimensione è > 4 vuol dire che ho messo dei caratteri in più es. 'ciao', non va bene
                                                messaggio->assign(_message_error);
                                                return false;
                                            }
                                            else{
                                                words[j].pop_back(); //tolgo la ,
                                                if (words[j][words[j].size()-1] != 39){ //controllo che venga chiuso il campo char
                                                    messaggio->assign(_message_error);
                                                    return false;
                                                }
                                            }
                                        }
                                        else if (words[j][words[j].size()-1] == ';'){ //vuol dire che si è arrivati alla fine del comando
                                            flag_fine_comando = true;
                                            if (words[j].size() != 5){ //tipo 1: se la dimensione è > 5 vuol dire che ho messo dei caratteri in più es. 'ciao'); non va bene
                                                messaggio->assign(_message_error);
                                                return false;
                                            }
                                            words[j].pop_back(); //tolgo il ;
                                            if (words[j][words[j].size()-1] == ')'){
                                                words[j].pop_back();
                                                if (words[j][words[j].size()-1] == 39){
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
                                    } else { //resto casi: 20, o 20);
                                        if (words[j][words[j].size()-1] != ','){
                                            if (words[j][words[j].size()-1] != ';'){
                                                messaggio->assign(_message_error);
                                                return false;
                                            }
                                            else {
                                                words[j].pop_back(); //tolgo ;
                                                if (words[j][words[j].size()-1] != ')'){
                                                    messaggio->assign(_message_error);
                                                    return false;
                                                }
                                                else
                                                    flag_fine_comando = true;
                                            }
                                        }
                                    }
                                }
                                else {
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                                j++;
                                words.emplace_back();
                                comando >> words[j];
                            }
                        }
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
    if (!flag_fine_comando){ //ulteriore controllo
        messaggio->assign(_message_error);
        return false;
    }
    else return true;
} //ricontrollare questione fine programma

bool ControlloSintassi::controlloDelete(stringstream &comando, string *messaggio) const {
    string word;
    char carattere;
    bool fine_testo = false;
    int contatore_virgolette;
    comando >> word >> word;
    if (toUp(word) == "FROM"){
        comando >> word;
        if (!belongs_to_keywords(word)){
            comando >> word;
            if (toUp(word) == "WHERE"){ //deve esserci per forza la condizione altrimenti il comando sarebbe truncate
                comando >> word >> word;
                if (belongs_to_operatori(word)){
                    comando >> word;
                    if (word[0] == '"'){ //caso di un campo testo
                        contatore_virgolette = 0;
                        fine_testo = false;
                        int k;
                        for (k = 1; k < word.size(); k++){
                            if (word[k] == 34) {
                                contatore_virgolette++;
                            } else {
                                if (contatore_virgolette % 2 != 0) {
                                    fine_testo = true;
                                }
                                contatore_virgolette = 0;
                            }
                        }
                        if (!fine_testo) {
                            while (!fine_testo) {
                                comando >> carattere;
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
                            carattere = word[k-1];
                        if (carattere != ';') {
                            messaggio->assign(_message_error);
                            return false;
                        } else
                            return true;
                    }
                    else { //campo char o qualsiasi altro: unica parola per forza
                        if (word[word.size()-1] != ';'){
                            messaggio->assign(_message_error);
                            return false;
                        } else
                            return true;
                    }
                } else {
                    messaggio->assign(_invalid_operator);
                    return false;
                }
            }
            else {
                messaggio->assign(_message_error);
                return false;
            }
        }
        else {
            messaggio->assign(_message_error_keyword);
            return false;
        }
    } else {
        messaggio->assign(_message_error);
        return false;
    }
}

bool ControlloSintassi::controlloUpdate(stringstream &comando, string *messaggio) const {
    string word;
    char carattere;
    bool fine_testo = false, flag_fine_comando = false, flag_condizione = false;
    int contatore_virgolette;
    vector<string> words;
    comando >> word >> word;
    if (!belongs_to_keywords(word)){
        comando >> word;
        if (toUp(word) == "SET"){
            comando >> word;
            while (!flag_fine_comando) {
                if (!flag_condizione){ //non si è ancora arrivati al where
                    comando >> word;
                    if (belongs_to_operatori(word)){
                        comando >> word;
                        if (word[0] == '"'){ //caso di un campo testo
                            contatore_virgolette = 0;
                            fine_testo = false;
                            int k;
                            for (k = 1; k < word.size(); k++){
                                if (word[k] == 34) {
                                    contatore_virgolette++;
                                } else {
                                    if (contatore_virgolette % 2 != 0) {
                                        fine_testo = true;
                                    }
                                    contatore_virgolette = 0;
                                }
                            }
                            if (!fine_testo) {
                                while (!fine_testo) {
                                    comando >> carattere;
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
                                carattere = word[k-1];
                            if (carattere == 'w') {
                                flag_condizione = true;
                                comando >> word; //campo o Where
                                word.insert(0,1,'w');
                            }
                            else if (carattere != ',' && carattere != 'w'){
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
                    } else {
                        messaggio->assign(_invalid_operator);
                        return false;
                    }
                }
                else { //si è arrivati al where
                    if (toUp(word) == "WHERE"){
                        comando >> word >> word;
                        if (belongs_to_operatori(word)){
                            comando >> word;
                            if (word[0] == '"'){ //caso di un campo testo
                                contatore_virgolette = 0;
                                fine_testo = false;
                                int k;
                                for (k = 1; k < word.size(); k++){
                                    if (word[k] == 34) {
                                        contatore_virgolette++;
                                    } else {
                                        if (contatore_virgolette % 2 != 0) {
                                            fine_testo = true;
                                        }
                                        contatore_virgolette = 0;
                                    }
                                }
                                if (!fine_testo) {
                                    while (!fine_testo) {
                                        comando >> carattere;
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
                                    carattere = word[k-1];
                                if (carattere == ';') {
                                    flag_fine_comando = true;
                                }
                                else {
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                            }
                            else { //campo char o qualsiasi altro: unica parola per forza
                                if (word[word.size()-1] != ';'){
                                    messaggio->assign(_message_error);
                                    return false;
                                }
                                else {
                                    if (word[0] == 39){ //se è un char controllo he l'utente abbia effettivamente inserito solo un carattere con la sintassi giusta
                                        if (word.size() != 4){
                                            messaggio->assign(_message_error);
                                            return false;
                                        }
                                        else {
                                            word.pop_back(); //tolgo ;
                                            if (word[word.size()-1] != 39){
                                                messaggio->assign(_message_error);
                                                return false;
                                            }
                                            else
                                                flag_fine_comando = true;
                                        }
                                    }
                                    else
                                        flag_fine_comando = true;
                                }
                            }
                        } else {
                            messaggio->assign(_invalid_operator);
                            return false;
                        }
                    }
                    else {
                        messaggio->assign(_invalid_operator);
                        return false;
                    }
                }
            }
        }
        else {
            messaggio->assign(_message_error);
            return false;
        }
    } else {
        messaggio->assign(_message_error_keyword);
        return false;
    }

    if (!flag_fine_comando){
        messaggio->assign(_message_error_keyword);
        return false;
    }
    else return true;
}











