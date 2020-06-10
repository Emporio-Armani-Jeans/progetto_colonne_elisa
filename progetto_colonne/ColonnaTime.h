//
// Created by Elisa Messina on 02/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNATIME_H
#define PROGETTO_COLONNE_COLONNATIME_H

#include "Colonna.h"
#include "Time.h"
#include <vector>

class ColonnaTime : public Colonna {
public:
    friend class Tabella;
    explicit ColonnaTime(const string &nomecolonna, bool notnull = false);
    ~ColonnaTime()  =default;

    void updateVal(const string& val, int index)override;
    string getElement(int index) override;
    void deleteVal(int index)override;
    void addDefault()override;
    bool compareElements(const string& condizione, int operatore, int index)const override;

private:
    vector <Time> _elementi_time;
    Time _default_value;
};


#endif //PROGETTO_COLONNE_COLONNATIME_H
