//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNAFLOAT_H
#define PROGETTO_COLONNE_COLONNAFLOAT_H

#include <string>
#include <list>
#include "Colonna.h"
#include <vector>
using namespace std;

class ColonnaFloat : public Colonna{
public:
    friend class Tabella;

    explicit ColonnaFloat(const string &nomecolonna, bool notnull = false); //costruttore
    ~ColonnaFloat()  override = default; //distruttore

    /* metodi ereditati */
    void updateVal(const string& val, int index) override;
    string getElement(int i) override;
    void deleteVal(int index) override;
    void addDefault(int val=0) override;
    bool compareElements(const string& condizione, int operatore, int index) const override;
    int getSize() const override;
    string getTipo()const override;
    bool isAutoIncrement()const override{
        return false;
    }
    void controlloFormato(const string &value) const override;

private:
    vector <float> _elementi_float;
    float _default_value;
};


#endif //PROGETTO_COLONNE_COLONNAFLOAT_H
