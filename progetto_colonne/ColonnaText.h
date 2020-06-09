//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNATEXT_H
#define PROGETTO_COLONNE_COLONNATEXT_H

#include <string>
#include <list>
#include "Colonna.h"
#include <vector>
using namespace std;
class ColonnaText : public Colonna{
public:
    friend class Tabella;
    explicit ColonnaText(const string &nomecolonna, bool notnull = false);
    ~ColonnaText()  override =default;

    void addVal(const string &valore_da_impostare) override;
    void updateVal(const string& val, int index)override;
    string getElement(int index)override;
    void deleteVal(int index)override;
    void addDefault() override;
    bool compareElements(const string& condizione, int operatore, int index)const override;

private:
    vector<string> _elementi_di_testo;
    string _default_value;
};


#endif //PROGETTO_COLONNE_COLONNATEXT_H
