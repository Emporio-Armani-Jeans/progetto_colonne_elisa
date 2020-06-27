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

    explicit ColonnaChar(const string &nomecolonna, bool notnull = false); //costruttore
    ~ColonnaChar() override = default; //distruttore

    /* metodi ereditati */
    void updateVal(const string& val, int index) override;
    string getElement(int index) override;
    void deleteVal(int index) override;
    void addDefault() override;
    bool compareElements(const string& condizione, int operatore, int index) const override;
    int getSize() const override;
    string getTipo()const override;
    bool isAutoIncrement()const override{
        return false;
    }
    void controlloFormato(const string &value) const override;

private:
    vector <char> _elementi_char;
    char _default_value;
};


#endif //PROGETTO_COLONNE_COLONNACHAR_H
