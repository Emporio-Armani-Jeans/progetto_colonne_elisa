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
    string getNomeColonna() const {return _nome_colonna;}
    virtual void impostaValore(const string &valore_da_impostare) = 0;
protected:
    string _nome_colonna;
};


#endif //PROGETTO_COLONNE_COLONNA_H
