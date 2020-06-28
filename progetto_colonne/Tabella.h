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
#include "CampoNonTrovato.h"
#include "InvalidOperator.h"
#include "SecKeyError.h"
#include "NotNullError.h"
#include "SecKeyNotFound.h"
#include "PrimKeyError.h"
#include "PrimaryKeyAlreadyExisting.h"
#include "InvalidCondition.h"
#include <vector>
#include <string>
#include "SecKeyAlreadyExisting.h"
#include "ValueNotFound.h"
#include "TentativoInserimentoAutoIncrement.h"
#include "UsedValueInSecKey.h"
#include "LinkedError.h"

using namespace std;
class Tabella {
public:
    explicit Tabella(const string &nometabella); //costruttore
    ~Tabella(); //distruttore

    void aggiungiColonna(Colonna *to_be_added); //aggiunta di un nuovo oggetto colonna alla tabella
    void setChiavePrimaria(const string& nomecolonna); //imposta la chiave primaria della tabella
    void setChiaveEsterna(Tabella* tabella_to_link, const string& colonna_this, const string& chiave_esterna);

    /* Funzione per aggiungere un record: per ogni colonna con nome uguale agli elementi del vettore <campi>,
     * aggiunge un elemento e imposta il corrispettivo valore contenuto nel vettore <valori> */
    void addRecord(const vector<string>& campi, const vector<string>& valori, int *increment_value);
    void addRecordMemory(const vector<string>& campi, const vector<string>& valori);
    bool isLinked();

    void deleteRecord(const string& campo_condizione=std::string(), const string& condizione=std::string(), int operatore=0);
    void deleteRecord(const string& campo_condizione, const string& condizione1, const string& condizione2);  //between operator

    void updateRecord(const string& campo_condizione, const string& condizione, const vector<string>& campi, const vector<string>& valori, int operatore=0);
    void updateRecord(const string& campo_condizioni, const string& condizione1, const string& condizione2, const vector<string>& campi, const vector<string>& valori);

    string getNome()const;
    Colonna* getCol(int index)const;
    int numCampi()const;
    int numRecs()const;

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
    bool erroreSecKey(int index);
};



#endif //PROGETTO_COLONNE_TABELLA_H