//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_TABELLA_H
#define PROGETTO_COLONNE_TABELLA_H

#include "Colonna.h"
#include "ColonnaInt.h"
#include "ColonnaFloat.h"
#include "ColonnaText.h"
#include <list>
#include <string>
using namespace std;
class Tabella {
public:
    explicit Tabella(const string &nometabella);
    ~Tabella();

    void aggiungiColonna(Colonna *to_be_added);
    void setChiavePrimaria(ColonnaInt *to_be_primary_key);

    void impostaValore(const string &nomecolonna, const string &valore);


private:
    list<Colonna*> _colonne;
    string _nome_tabella;
    ColonnaInt *_chiave_primaria;
    int _numero_colonne;
};




#endif //PROGETTO_COLONNE_TABELLA_H
