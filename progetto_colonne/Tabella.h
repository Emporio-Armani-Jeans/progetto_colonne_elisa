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

    void addRecord(const vector<string>& campi, const vector<string>& valori);
    void deleteRecord(const string& nome_col, const string& condizione);
    void updateRecord(const string& condizione, const string& nome_col, vector<string> campi, vector<string> valori);
    Colonna* getCol(int index)const;
    bool recExists(int index)const;
    int numCampi()const;
    int numRecs()const;
    vector<string> returnData()const;
    vector<string> returnData(const vector<string>& campi)const;
    vector<string> returnData(const vector<string>& campi, const string& campo_condizione, const string& condizione);

private:
    vector<Colonna*> _colonne;
    string _nome_tabella;
    Colonna *_chiave_primaria;
    vector<bool> _recs;
    void impostaVal(const string &nomecolonna, const string &valore);
};


#endif //PROGETTO_COLONNE_TABELLA_H