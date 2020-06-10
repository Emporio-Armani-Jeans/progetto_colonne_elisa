//
// Created by andry on 01/06/2020.
//

#include "ColonnaDate.h"

ColonnaDate::ColonnaDate(const string &nomecolonna, bool notnull){
    _nome_colonna=nomecolonna;
    _not_null = notnull;
    Date d(0,0,0);
    _default_value=d;
    _primary_key=false;
}

string ColonnaDate::getElement(int index){
    if(index==-1) return _default_value.str();
    else return _elementi_date[index].str();
}

void ColonnaDate::deleteVal(int index) {
    auto it=_elementi_date.begin();
    it+=index;
    _elementi_date.erase(it);
}

void ColonnaDate::updateVal(const string &val, int index) {
    int day, month, year;
    day=std::stoi(val.substr(0,2));
    month=std::stoi(val.substr(3,2));
    year=std::stoi(val.substr(6,4));
    Date data(day,month,year);
    _elementi_date[index]=data;
}

void ColonnaDate::addDefault() {
    _elementi_date.push_back(_default_value);
}

bool ColonnaDate::compareElements(const string& condizione, int operatore, int index)const{
    int day, month, year;
    day=std::stoi(condizione.substr(0,2));
    month=std::stoi(condizione.substr(3,2));
    year=std::stoi(condizione.substr(6,4));
    Date to_compare(day,month,year);
    switch (operatore){
        case 0:
            return _elementi_date[index]==to_compare;
        case 1:
            return _elementi_date[index]<to_compare;
        case 2:
            return _elementi_date[index]<=to_compare;
        case 3:
            return _elementi_date[index]>to_compare;
        case 4:
            return _elementi_date[index]>=to_compare;
        default:
            //creare eccezione
            return false;
    }
}