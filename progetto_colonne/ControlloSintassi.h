//
// Created by Elisa Messina on 15/06/2020.
//

#ifndef PROGETTO_COLONNE_CONTROLLOSINTASSI_H
#define PROGETTO_COLONNE_CONTROLLOSINTASSI_H

#include <string>
#include <vector>
#include <sstream>
using namespace std;

class ControlloSintassi {
public:
    ControlloSintassi();

    //controlli tornano true se il formato è corretto e modificano il messaggio di errore caso per caso
    bool controlloCreate(stringstream &comando, string *messaggio) const;
    bool controlloTruncate(stringstream &comando, string *messaggio) const;
    bool controlloDrop(stringstream &comando, string *messaggio) const;
    bool controlloInsert(stringstream &comando, string *messaggio) const;
    bool controlloDelete(stringstream &comando, string *messaggio) const;
    bool controlloUpdate(stringstream &comando, string *messaggio) const;

private:
    string _message_error, _message_error_keyword, _wrong_type_auto_increment, _inexistent_type, _invalid_operator, _message_error_key, _missing_pk;
    vector <string> _keywords{"CREATE", "TABLE", "INT", "FLOAT", "CHAR", "TEXT", "DATE", "TIME", "NOT", "NULL",
                              "AUTO_INCREMENT", "PRIMARY", "KEY", "DROP", "INSERT", "VALUES", "INTO", "DELETE",
                              "FROM", "WHERE", "TRUNCATE", "UPDATE", "SET", "SELECT", "ORDER", "BY", "DESC",
                              "ASC", "FOREIGN", "REFERENCES", "BETWEEN", "AND"};
    vector<string> operatori {"=", "<", ">", ">=", "<=", "<>", "BETWEEN"};
    bool belongs_to_operatori(string &to_be_compared) const;
    bool belongs_to_keywords(string &to_be_compared) const;
};


#endif //PROGETTO_COLONNE_CONTROLLOSINTASSI_H
