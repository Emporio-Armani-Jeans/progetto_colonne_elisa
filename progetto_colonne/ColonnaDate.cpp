//
// Created by andry on 01/06/2020.
//

#include "ColonnaDate.h"

ColonnaDate::ColonnaDate(const string &nomecolonna, bool notnull){
    _nome_colonna=nomecolonna;
    _not_null = notnull;
}

void ColonnaDate::impostaValore(const string &valore_da_impostare){
    int day, month, year;
    day=std::stoi(valore_da_impostare.substr(0,2));
    month=std::stoi(valore_da_impostare.substr(3,2));
    year=std::stoi(valore_da_impostare.substr(6,4));
    Date data(day,month,year);
    _elementi_date.push_back(data);
}
