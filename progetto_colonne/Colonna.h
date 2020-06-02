//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNA_H
#define PROGETTO_COLONNE_COLONNA_H

#include <string>
using namespace std;

class Colonna {
public:
    friend class Tabella;
    ~Colonna()=default;
    string getNomeColonna() const {return _nome_colonna;}
    virtual void addVal(const string &valore_da_impostare) = 0;
    virtual void updateVal(const string& val, int index)=0;
    virtual string getElement(int i)=0;
    virtual void deleteVal(int index)=0;
protected:
    string _nome_colonna;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNA_H
