//
// Created by Elisa Messina on 02/06/2020.
//

#ifndef PROGETTO_COLONNE_COLONNACHAR_H
#define PROGETTO_COLONNE_COLONNACHAR_H

#include "Colonna.h"
#include <vector>

class ColonnaChar : public Colonna {
public:
    friend class Tabella;
    explicit ColonnaChar(const string &nomecolonna, bool notnull = false);
    ~ColonnaChar()  override = default;

    void setValore(const string &value_to_set) override;
    string getElemento(int index) const override;
    void deleteValore(int index)override;

private:
    vector <char> _elementi_char;
};


#endif //PROGETTO_COLONNE_COLONNACHAR_H
