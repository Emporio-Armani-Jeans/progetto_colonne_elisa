#ifndef PROGETTO_COLONNE_COLONNADATE_H
#define PROGETTO_COLONNE_COLONNADATE_H
#include "Colonna.h"
#include "data.h"
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;


class ColonnaDate : public Colonna{
public:
    friend class Tabella;

    explicit ColonnaDate(const string &nomecolonna, bool notnull = false); //costruttore
    ~ColonnaDate()  override = default; //distruttore

    /* metodi ereditati */
    void updateVal(const string& val, int index) override;
    void deleteVal(int index) override;
    string getElement(int index) override;
    void addDefault(int val) override;
    bool compareElements(const string& condizione, int operatore, int index) const override;
    int getSize() const override;
    string getTipo()const override;
    bool isAutoIncrement()const override{
        return false;
    }
    void controlloFormato(const string &value) const override;

private:
    vector<Date> _elementi_date;
    Date _default_value;
};


#endif //PROGETTO_COLONNE_COLONNADATE_H
