//
// Created by andry on 01/06/2020.
//

#include "ColonnaDate.h"

ColonnaDate::ColonnaDate(const string &nomecolonna, bool notnull){
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    Date d(0,0,0);
    _default_value = d;
    _primary_key = false;
    _foreign_key = nullptr;
    _tab_madre="";
    _colonna_figlio= nullptr;
}

string ColonnaDate::getElement(int index){
    if(index == -1)
        return _default_value.str();
    else
        return _elementi_date[index].str();
}

void ColonnaDate::deleteVal(int index) {
    auto it = _elementi_date.begin();
    it += index;
    _elementi_date.erase(it);
}

void ColonnaDate::updateVal(const string &val, int index) {
    string s1=val.substr(0,2), s2=val.substr(3,2), s3=val.substr(6,4);
    controlloFormato(val);
    int day, month, year;
    day = std::stoi(s1);
    month = std::stoi(s2);
    year = std::stoi(s3);
    Date data(day,month,year);
    if (!_primary_key) {
        if (_foreign_key == nullptr)
            _elementi_date[index] = data;
        else {
            bool valore_trovato = false;
            for (int i = 0; i < _foreign_key->getSize(); i++){
                if (_foreign_key->getElement(i) == val){
                    valore_trovato = true;
                    _elementi_date[index] = data;
                }
            }
            if (!valore_trovato) {
                throw SecKeyError();
            }
        }
    }
    else { //se la colonna è una chiave primaria, controllo che il valore che si sta cercando di aggiornare non sia già presente in un altro record
        bool flag_duplicate_found = false;
        for (int i = 0; i < _elementi_date.size() && !flag_duplicate_found; i++) {
            if (_elementi_date[i] == data)
                flag_duplicate_found = true;
        }
        if (flag_duplicate_found) {
            throw PrimKeyError();
        } else { //se non ci sono valori uguali presenti, l'aggiornamento è permesso
            if (_foreign_key == nullptr)
                _elementi_date[index] = data;
            else {
                bool valore_trovato = false;
                for (int i = 0; i < _foreign_key->getSize(); i++){
                    if (_foreign_key->getElement(i) == val){
                        valore_trovato = true;
                        _elementi_date[index] = data;
                    }
                }
                if (!valore_trovato) {
                    throw SecKeyError();
                }
            }
        }
    }
}

void ColonnaDate::addDefault(int val) {
    _elementi_date.push_back(_default_value);
}

bool ColonnaDate::compareElements(const string& condizione, int operatore, int index)const{
    int day, month, year;
    day = std::stoi(condizione.substr(0,2));
    month = std::stoi(condizione.substr(3,2));
    year = std::stoi(condizione.substr(6,4));
    Date to_compare(day,month,year);
    switch (operatore){
        case 0:
            return (_elementi_date[index] == to_compare);
        case 1:
            return (_elementi_date[index] < to_compare);
        case 2:
            return (_elementi_date[index] <= to_compare);
        case 3:
            return (_elementi_date[index] > to_compare);
        case 4:
            return (_elementi_date[index] >= to_compare);
        case 5:
            return (_elementi_date[index] != to_compare);
        default:
            throw InvalidOperator();
    }
}

int ColonnaDate::getSize() const {
    return _elementi_date.size();
}

string ColonnaDate::getTipo()const{
    return "data";
}

void ColonnaDate::controlloFormato(const string &value) const {
    for(int i=0; i<value.size(); i++){          //controllo formato
        if(i==2 || i==5){
            if(value[i]!='/')
                throw FormatTypeError();
        }else{
            if(value[i]<48 || value[i]>57)
                throw FormatTypeError();
        }
    }
}
