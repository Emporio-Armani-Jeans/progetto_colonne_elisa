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

    //controlli tornano true se il formato è corretto
    bool controlloCreate(stringstream &comando, string *messaggio) const;
    bool controlloTruncate(stringstream &comando, string *messaggio) const;

private:
    string _message_error;
    string _message_error_keyword;
    string _wrong_type_auto_increment;
    string _inexistent_type;
    vector <string> _keywords{"CREATE", "TABLE", "INT", "FLOAT", "CHAR", "TEXT", "DATE", "TIME", "NOT", "NULL",
                              "AUTO_INCREMENT", "PRIMARY", "KEY", "DROP", "INSERT", "VALUES", "INTO", "DELETE",
                              "FROM", "WHERE", "TRUNCATE", "UPDATE", "SET", "SELECT", "ORDER", "BY", "DESC",
                              "ASC", "FOREIGN", "REFERENCES", "BETWEEN", "AND"};
    bool belongs_to_keywords(string &to_be_compared) const;
};


#endif //PROGETTO_COLONNE_CONTROLLOSINTASSI_H
