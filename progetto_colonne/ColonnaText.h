//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNATEXT_H
#define PROGETTO_COLONNE_COLONNATEXT_H

#include <string>
#include <list>
#include "Colonna.h"
using namespace std;
class ColonnaText : public Colonna{
public:
    friend class Tabella;
    explicit ColonnaText(const string &nomecolonna, bool notnull = false);

    void impostaValore(const string &valore_da_impostare) override;

private:
    list <string> _elementi_di_testo;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNATEXT_H
