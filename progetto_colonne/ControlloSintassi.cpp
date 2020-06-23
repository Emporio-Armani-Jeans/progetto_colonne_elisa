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

bool ControlloSintassi::controlloCreate(stringstream &comando, string* messaggio) const {
    string word;
    char c;
    bool comando_corretto = false;
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

                    else if (words[0] == ");"){ //provare
                        return true;
                    }

                    else {//se la prima parola di una riga non è nè foreign nè primary allora è per forza il nome di una colonna
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
                                                if ( (toUp(words[j]) == "NOT" && toUp(words[j+1]) == "NULL") || (toUp(words[j]) == "NULL" && toUp(words[j-1]) == "NOT") || ((toUp(words[j]) == "AUTO") && (toUp(words[j+1]) == "INCREMENT")) || ((toUp(words[j]) == "INCREMENT") && (toUp(words[j-1]) == "AUTO"))){
                                                    if (((toUp(words[j]) == "AUTO") && (toUp(words[j+1]) == "INCREMENT")) || ((toUp(words[j]) == "INCREMENT") && (toUp(words[j-1]) == "AUTO"))){
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
                                                if ((toUp(words[j]) == "NULL" && toUp(words[j-1]) == "NOT") || ((toUp(words[j]) == "INCREMENT") && (toUp(words[j-1]) == "AUTO"))){
                                                    if ((toUp(words[j]) == "INCREMENT") && (toUp(words[j-1]) == "AUTO")){
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
                    }
                    if (!comando_corretto) //se alla fine di un ciclo di un while (fine di una riga) il comando non è corretto ritorno il messaggio di errore
                        return false;
                }
                if (comando_corretto){ //se dopo aver analizzato tutte le righe il comando è corretto vuol dire che la sintassi è tutta giusta
                    return true;
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
    messaggio->assign(_message_error);
    return false;
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

/*
bool ControlloSintassi::controlloInsert(stringstream &comando, string *messaggio) const{
    string word;
    char car;
    vector<string> words;
    comando >> word;
    if (toUp(word) == "INTO"){
        comando >> word;
        comando >> car;
        if (car == '('){
            words.clear();
            int i = 0;
            words.emplace_back();
            comando >> words[i];
            while (words[i][words[i].size()-1] != ')'){
                if (words[i][words[i].size()-1] != ','){
                    messaggio->assign(_message_error);
                    return false;
                }
                words[i].pop_back(); //una volta appurato che ci sia la , alla fine, la tolgo.
                if (belongs_to_keywords(words[i])){
                    messaggio->assign(_message_error_keyword);
                    return false;
                }
                i++;
                comando >> words[i];
            }
            comando >> word;
            if (toUp(word) != "VALUES"){
                messaggio->assign(_message_error);
                return false;
            }
            else{
                comando >> car;
                if(car != '('){
                    messaggio->assign(_message_error);
                    return false;
                }
                else {
                    while ()
                }
            }
        }else{
            messaggio->assign(_message_error);
            return false;
        }
    }
    else{
        messaggio->assign(_message_error);
        return false;
    }
}*/











