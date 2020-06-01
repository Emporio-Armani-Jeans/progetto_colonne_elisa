//
// Created by andry on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNADATE_H
#define PROGETTO_COLONNE_COLONNADATE_H
#include "Colonna.h"
#include "data.h"
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;


class ColonnaDate : public Colonna{
public:
    friend class Tabella;
    explicit ColonnaDate(const string &nomecolonna, bool notnull = false);

    void impostaValore(const string& valore_da_impostare)override;
    string getElement(int i)override;
private:
    vector<Date> _elementi_date;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNADATE_H
