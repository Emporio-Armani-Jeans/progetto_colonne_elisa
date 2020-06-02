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
    ~ColonnaDate() override = default;

    void setValore(const string& value_to_set) override;
    void deleteValore(int index) override;
    string getElemento(int index) const override;
private:
    vector<Date> _elementi_date;
};


#endif //PROGETTO_COLONNE_COLONNADATE_H
