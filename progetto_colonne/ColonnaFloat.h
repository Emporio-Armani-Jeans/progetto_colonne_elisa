//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNAFLOAT_H
#define PROGETTO_COLONNE_COLONNAFLOAT_H

#include <string>
#include <list>
#include "Colonna.h"
#include <vector>
using namespace std;

class ColonnaFloat : public Colonna{
public:
    friend class Tabella;
    explicit ColonnaFloat(const string &nomecolonna, bool notnull = false);
   // ~ColonnaFloat() override =default;

    void setVal(const string &valore_da_impostare) override;
    string getElement(int i) override;
private:
    vector <float> _elementi_float;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNAFLOAT_H
