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
  //  ~ColonnaTime() override =default;

    void setVal(const string &valore_da_impostare) override;
    string getElement(int index) override;
    void deleteVal(int index)override;

private:
    vector <Time> _elementi_time;
};


#endif //PROGETTO_COLONNE_COLONNATIME_H
