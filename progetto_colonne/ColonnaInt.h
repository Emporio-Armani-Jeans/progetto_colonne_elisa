//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNAINT_H
#define PROGETTO_COLONNE_COLONNAINT_H

#include <string>
#include <list>
#include "Colonna.h"
#include <vector>
using namespace std;

class ColonnaInt : public Colonna {
public:
    friend class Tabella;
    explicit ColonnaInt(const string &nomecolonna, bool notnull = false, bool autoincrement = false);
   // ~ColonnaInt() override =default;

    void addVal(const string &valore_da_impostare) override;
    void updateVal(const string& val, int index)override;
    string getElement(int index) override;
    void deleteVal(int index)override;

private:
    vector <int> _elementi_interi;
    bool _auto_increment;
};


#endif //PROGETTO_COLONNE_COLONNAINT_H
