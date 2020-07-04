#ifndef PROGETTO_COLONNE_CONTROLLOSINTASSI_H
#define PROGETTO_COLONNE_CONTROLLOSINTASSI_H

#include <string>
#include <vector>
#include <sstream>
#include "FormatTypeError.h"

using namespace std;

class ControlloSintassi {
public:
    ControlloSintassi(); //costruttore
    ~ControlloSintassi() = default; //distruttore

    //controllo iniziale che gestisce i caratteri speciali
    static string Gestione_caratteri_speciali(string &comando, string *status_message);

    //controlli tornano true se il formato è corretto e modificano il messaggio di errore caso per caso
    bool controlloCreate(stringstream &comando, string *messaggio);
    bool controlloTruncate(stringstream &comando, string *messaggio);
    bool controlloDrop(stringstream &comando, string *messaggio);
    bool controlloInsert(stringstream &comando, string *messaggio);
    bool controlloDelete(stringstream &comando, string *messaggio);
    bool controlloUpdate(stringstream &comando, string *messaggio);
    bool controlloSelect(stringstream &comando, string *messaggio);

private:
    string _message_error, _message_error_keyword, _inexistent_type,_message_error_key, _missing_pk, _duplicate_col;
    vector <string> _keywords {"CREATE", "TABLE", "INT", "FLOAT", "CHAR", "TEXT", "DATE", "TIME", "NOT", "NULL",
                              "AUTO_INCREMENT", "PRIMARY", "KEY", "DROP", "INSERT", "VALUES", "INTO", "DELETE",
                              "FROM", "WHERE", "TRUNCATE", "UPDATE", "SET", "SELECT", "ORDER", "BY", "DESC",
                              "ASC", "FOREIGN", "REFERENCES", "BETWEEN", "AND"};
    vector<string> _tipi {"INT", "FLOAT", "CHAR", "TEXT", "DATE", "TIME"};
    //metodo che verifica se la stringa <to_be_compared> appartiene al vettore di stringhe <insieme>
    static bool belongs_to(string &to_be_compared, const vector<string>& insieme);
    static string toUp(string word);//conversione in maiuscolo
    static char GestioneTesto(stringstream *comando, string &word);
};


#endif //PROGETTO_COLONNE_CONTROLLOSINTASSI_H
