//
// Created by andry on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNADATE_H
#define PROGETTO_COLONNE_COLONNADATE_H
#include "Colonna.h"
#include "data.h"
#include <list>
#include <iostream>
using namespace std;


class ColonnaDate : public Colonna{
public:
    friend class Tabella;
    explicit ColonnaDate(const string &nomecolonna, bool notnull = false);

    void impostaValore(const string& valore_da_impostare)override;
private:
    list<Date> _elementi_date;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNADATE_H
