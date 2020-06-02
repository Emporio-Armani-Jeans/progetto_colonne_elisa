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
    virtual ~Colonna() = default;
    string getNomeColonna() const { return _nome_colonna; }
    virtual void setValore(const string &value_to_set) = 0;
    virtual string getElemento(const int &i) const = 0; //test
    virtual void deleteValore(const int &index) = 0;
protected:
    string _nome_colonna;
    bool _not_null;
};


#endif //PROGETTO_COLONNE_COLONNA_H
