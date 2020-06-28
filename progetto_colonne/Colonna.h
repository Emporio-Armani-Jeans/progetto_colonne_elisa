//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNA_H
#define PROGETTO_COLONNE_COLONNA_H
#include "CampoNonTrovato.h"
#include "InvalidOperator.h"
#include "SecKeyError.h"
#include "SecKeyNotFound.h"
#include "PrimKeyError.h"
#include "PrimaryKeyAlreadyExisting.h"
#include "InvalidCondition.h"
#include "FormatTypeError.h"
#include <string>
using namespace std;

class Colonna {
public:
    friend class Tabella;
    
    virtual ~Colonna() = default;
    
    string getNomeColonna() const { return _nome_colonna; }
    
    virtual void updateVal(const string& val, int index) = 0; //aggiorna a <val> il valore dell'elemento di indice <index> di una colonna
    virtual string getElement(int index) = 0; //ritorna sottoforma di stringa l'elemento di indice <index> di una colonna
    virtual void deleteVal(int index) = 0; //cancella l'elemento di indice <index> della colonna
    virtual void addDefault(int val) = 0;
    virtual bool compareElements(const string& condizione, int operatore, int index) const = 0; /*confronta il valore dell'elemento di indice <index>con <condizione>, secondo l'operatore <operatore>*/
    virtual int getSize() const = 0;
    virtual void controlloFormato(const string &value) const = 0;
    bool isNotNull()const{
        return _not_null;
    }
    bool isKey()const{
        return _primary_key;
    }
    virtual bool isAutoIncrement()const=0;
    void setNotNull(){
        _not_null=true;
    }
    virtual string getTipo()const=0;
    
protected:
    string _nome_colonna;
    bool _not_null;
    bool _primary_key;
    Colonna* _foreign_key;
    Colonna* _colonna_figlio;
};


#endif //PROGETTO_COLONNE_COLONNA_H
