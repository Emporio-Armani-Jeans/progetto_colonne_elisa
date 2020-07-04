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
#include "InsertAutoIncrement.h"
#include "LinkedError.h"
#include <memory.h>

using namespace std;
class Tabella {
public:
    explicit Tabella(const string &nometabella); //costruttore
    ~Tabella(); //distruttore

    string getNome() const; //ritorna il nome della tabella
    Colonna* getCol(int index) const; //puntatore alla colonna di indice <index> del vettore _colonne
    int numCampi() const; //numero di colonne
    int numRecs()const; //numero di record
    bool isLinked(); //ritorna vero se la primary key è "madre" di altre colonne di altre tabelle

    void aggiungiColonna(Colonna *to_be_added); //aggiunta di un nuovo oggetto colonna alla tabella
    void setChiavePrimaria(const string& nomecolonna); //imposta la chiave primaria della tabella
    void setChiaveEsterna(Tabella* tabella_to_link, const string& colonna_this, const string& chiave_esterna);//imposta i vincoli dovuti all'inserimento di una chiave esterna

    /* Metodo per aggiungere un record: per ogni colonna con nome uguale agli elementi del vettore <campi>,
     * aggiunge un elemento e imposta il corrispettivo valore contenuto nel vettore <valori> */
    void addRecord(const vector<string>& campi, const vector<string>& valori);
    void addRecordMemory(const vector<string>& campi, const vector<string>& valori); //versione alternativa

    /*Metodi per eliminare un record che soddisfa la condizione imposta secondo l'opportuno operatore*/
    void deleteRecord(const string& campo_condizione=std::string(), const string& condizione=std::string(), int operatore=0);
    void deleteRecord(const string& campo_condizione, const string& condizione1, const string& condizione2);  //between operator

    /*metodi per aggiornare le colonne con nome uguale agli elementi del vettore <campi> con i valori contenuti nel
     * vettore <valori>, solo nei record che rispettano la condizione imposta secondo l'opportuno operatore*/
    void updateRecord(const string& campo_condizione, const string& condizione, const vector<string>& campi, const vector<string>& valori, int operatore=0);
    void updateRecord(const string& campo_condizioni, const string& condizione1, const string& condizione2, const vector<string>& campi, const vector<string>& valori);

    /*metodi utili alla stampa delle colonne con nome uguale agli elementi del vettore <campi> che soddisfano, se presente,
     * la condizione imposta, e se richiesto pportunamente ordinati*/
    vector<string> returnData(const vector<string>& campi, const string& campo_ordinamento=string(), int operatore_ordinamento=0)const;
    vector<string> returnData(const vector<string>& campi, const string& campo_condizione, const string& condizione,
            int operatore=0, const string& campo_ordinamento=string(), int operatore_ordinamento=0)const; //condizione + eventuale ordinamento
    vector<string> returnData(const vector<string>& campi, const string& campo_condizione, const string& condizione1,
            const string& condizione2, const string& campo_ordinamento=string(), int operatore_ordinamento=0)const;//condizione between + eventuale ordinamento

private:
    vector<Colonna*> _colonne;
    string _nome_tabella;
    int _recs;
    //ritorna vettore di indici ordinati della colonna <campo> secondo l'operatore <operatore> crescente o decrescente
    vector<int> ordinamento(const string& campo, int operatore) const;
    bool erroreSecKey(int index);//ritorna vero se intercetta errori relativi alla chiave esterna
};



#endif //PROGETTO_COLONNE_TABELLA_H