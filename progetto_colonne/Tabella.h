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
#include "ColonnaChar.h"
#include "ColonnaTime.h"


#include <vector>
#include <string>
using namespace std;
class Tabella {
public:
    explicit Tabella(const string &nometabella);
    ~Tabella();

    void aggiungiColonna(Colonna *to_be_added);
    void setChiavePrimaria(Colonna *to_be_primary_key);

    void addRecord();
    void deleteRecord(const string& nome_col, const string& condizione);
    void impostaVal(const string &nomecolonna, const string &valore);
    Colonna* getCol(int index)const;
    bool recExists(int index)const;
    int numCampi()const;
    int numRecs()const;

private:
    vector<Colonna*> _colonne;
    string _nome_tabella;
    Colonna *_chiave_primaria;
    vector<bool> _recs;
};


#endif //PROGETTO_COLONNE_TABELLA_H