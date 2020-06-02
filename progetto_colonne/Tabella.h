//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_TABELLA_H
#define PROGETTO_COLONNE_TABELLA_H

#include "Colonna.h"
#include "ColonnaInt.h"
#include "ColonnaFloat.h"
#include "ColonnaText.h"
#include "ColonnaDate.h"

#include <vector>
#include <string>
using namespace std;
class Tabella {
public:
    explicit Tabella(const string &nometabella);
    ~Tabella();

    void aggiungiColonna(Colonna *to_be_added);
    void setChiavePrimaria(Colonna *to_be_primary_key);

    void impostaValore(const string &nomecolonna, const string &valore);
    Colonna* getCol(int index)const;

private:
    vector<Colonna*> _colonne;
    string _nome_tabella;
    Colonna *_chiave_primaria;
};

#endif //PROGETTO_COLONNE_TABELLA_H