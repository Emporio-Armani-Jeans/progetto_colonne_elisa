//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNAINT_H
#define PROGETTO_COLONNE_COLONNAINT_H

#include <string>
#include <list>
#include "Colonna.h"
using namespace std;
class ColonnaInt : public Colonna {
public:
    friend class Tabella;
    explicit ColonnaInt(const string &nomecolonna, bool notnull = false, bool autoincrement = false);

    void impostaValore(const string &valore_da_impostare) override;
    string getNomeColonna() const override;

private:
    string _nome_colonna;
    list <int> _elementi_interi;
    bool _not_null;
    bool _auto_increment;
};


#endif //PROGETTO_COLONNE_COLONNAINT_H
