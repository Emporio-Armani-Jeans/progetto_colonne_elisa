//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNAFLOAT_H
#define PROGETTO_COLONNE_COLONNAFLOAT_H

#include <string>
#include <list>
#include "Colonna.h"
using namespace std;

class ColonnaFloat : public Colonna{
public:
    friend class Tabella;
    explicit ColonnaFloat(const string &nomecolonna, bool notnull = false);

    void impostaValore(const string &valore_da_impostare) override;

private:
    list <float> _elementi_float;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNAFLOAT_H
