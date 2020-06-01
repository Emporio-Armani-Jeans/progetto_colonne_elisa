//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNA_H
#define PROGETTO_COLONNE_COLONNA_H

#include <string>
using namespace std;

class Colonna {
public:
    friend class Tabella;
    virtual string getNomeColonna() const = 0;
    virtual void impostaValore(const string &valore_da_impostare) = 0;
};


#endif //PROGETTO_COLONNE_COLONNA_H
