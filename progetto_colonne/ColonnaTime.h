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
    ~ColonnaTime() override = default;

    void setValore(const string &value_to_set) override;
    string getElemento(int index) const override;
    void deleteValore(int index) override;

private:
    vector <Time> _elementi_time;
};


#endif //PROGETTO_COLONNE_COLONNATIME_H
