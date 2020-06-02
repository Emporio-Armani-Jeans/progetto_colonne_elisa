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
    ~ColonnaFloat() override = default;

    void setValore(const string &value_to_set) override;
    string getElemento(const int &i) const override;
    void deleteValore(const int &index) override;
private:
    vector <float> _elementi_float;
};


#endif //PROGETTO_COLONNE_COLONNAFLOAT_H
