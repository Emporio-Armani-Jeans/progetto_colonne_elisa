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

    string controlloCreate(stringstream &comando) const;
    string controlloTruncate(stringstream &comando) const;

private:
    string _message_error;
    string _message_error_keyword;
    string _comando_corretto;
    string _wrong_type_auto_increment;
    vector <string> _keywords{"CREATE", "TABLE", "INT", "FLOAT", "CHAR", "TEXT", "DATE", "TIME", "NOT", "NULL",
                              "AUTO_INCREMENT", "PRIMARY", "KEY", "DROP", "INSERT", "VALUES", "INTO", "DELETE",
                              "FROM", "WHERE", "TRUNCATE", "UPDATE", "SET", "SELECT", "ORDER", "BY", "DESC",
                              "ASC", "FOREIGN", "REFERENCES", "BETWEEN", "AND"};
    bool belongs_to_keywords(string &to_be_compared) const;
};


#endif //PROGETTO_COLONNE_CONTROLLOSINTASSI_H
