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
    explicit Tabella(const string &nometabella); //costruttore
    ~Tabella(); //distruttore

    void aggiungiColonna(Colonna *to_be_added); //aggiunta di un nuovo oggetto colonna alla tabella
    void setChiavePrimaria(const string& nomecolonna); //imposta la chiave primaria della tabella

    /* Funzione per aggiungere un record: per ogni colonna con nome uguale agli elementi del vettore <campi>,
     * aggiunge un elemento e imposta il corrispettivo valore contenuto nel vettore <valori> */
    void addRecord(const vector<string>& campi, const vector<string>& valori);

    void deleteRecord(const string& nome_col, const string& condizione, int operatore=0);
    void deleteRecord(const string& nome_col, const string& condizione1, const string& condizione2);  //between operator

    void updateRecord(const string& condizione, const string& nome_col, const vector<string>& campi,const vector<string>& valori, int operatore=0);
    void updateRecord(const string& condizione1, const string& condizione2, const string& nome_col, const vector<string>& campi, const vector<string>& valori);

    Colonna* getCol(int index)const; //boh ahah
    int numCampi()const;
    int numRecs()const;

    vector<string> returnData(const string& campo_ordinamento=string(), int operatore_ordinamento=0)const;
    vector<string> returnData(const vector<string>& campi, const string& campo_ordinamento=string(), int operatore_ordinamento=0)const;
    vector<string> returnData(const vector<string>& campi, const string& campo_condizione, const string& condizione,
            int operatore=0, const string& campo_ordinamento=string(), int operatore_ordinamento=0)const;
    vector<string> returnData(const vector<string>& campi, const string& campo_condizione, const string& condizione1,
            const string& condizione2, const string& campo_ordinamento=string(), int operatore_ordinamento=0)const;

private:
    vector<Colonna*> _colonne;
    string _nome_tabella;
    int _recs;
    vector<int> ordinamento(const string& campo, int operatore)const;
};


#endif //PROGETTO_COLONNE_TABELLA_H