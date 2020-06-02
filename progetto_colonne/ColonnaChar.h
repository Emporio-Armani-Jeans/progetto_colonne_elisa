//
// Created by Elisa Messina on 02/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNACHAR_H
#define PROGETTO_COLONNE_COLONNACHAR_H

#include "Colonna.h"
#include <vector>

class ColonnaChar : public Colonna {
public:
    friend class Tabella;
    explicit ColonnaChar(const string &nomecolonna, bool notnull = false);
   // ~ColonnaChar()  override =default;

    void setVal(const string &valore_da_impostare) override;
    string getElement(int index) override;

private:
    vector <char> _elementi_char;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNACHAR_H
