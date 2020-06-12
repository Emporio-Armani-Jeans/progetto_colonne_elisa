//
// Created by Elisa Messina on 01/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNAINT_H
#define PROGETTO_COLONNE_COLONNAINT_H

#include <string>
#include <list>
#include "Colonna.h"
#include <vector>
using namespace std;

class ColonnaInt : public Colonna {
public:
    friend class Tabella;

    explicit ColonnaInt(const string &nomecolonna, bool notnull = false, bool autoincrement = false, int* increment_value= nullptr); //costruttore
    ~ColonnaInt()  override = default; //distruttore

    /* metodi ereditati */
    void updateVal(const string& val, int index) override;
    string getElement(int index) override;
    void deleteVal(int index) override;
    void addDefault() override;
    bool compareElements(const string& condizione, int operatore, int index) const override;
    int getSize() const override;

private:
    vector <int> _elementi_interi;
    bool _auto_increment;
    int *_increment_value;
    int _default_value;
};


#endif //PROGETTO_COLONNE_COLONNAINT_H
