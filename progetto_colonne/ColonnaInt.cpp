//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaInt.h"

ColonnaInt::ColonnaInt(const string &nomecolonna, bool notnull, bool autoincrement, int* increment_value) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _auto_increment = autoincrement;
    _default_value = 0;
    _increment_value = increment_value;
    _primary_key = false;
    _foreign_key = nullptr;
}

string ColonnaInt::getElement(int index) {
    if(index == -1)  //indice non valido
        return to_string(_default_value);
    else
        return to_string(_elementi_interi[index]);
}

void ColonnaInt::deleteVal(int index){
    auto it = _elementi_interi.begin();
    it += index;
    _elementi_interi.erase(it);
}

void ColonnaInt::updateVal(const string& val, int index){
    for(char i : val){                 //controllo formato
        if(i < 48 || i > 57)
            throw FormatTypeError();
    }
    if (!_auto_increment) { //aggiornamento del valore solo se la colonna non è auto increment (se lo è viene già aggiornata precedentemente)
        int new_value = std::stoi(val);
        if (!_primary_key) {
            if (_foreign_key == nullptr)
                _elementi_interi[index] = new_value;
            else {
                bool valore_trovato = false;
                for (int i = 0; i < _foreign_key->getSize(); i++){
                    if (_foreign_key->getElement(i) == val){
                        valore_trovato = true;
                        _elementi_interi[index] = new_value;
                    }
                }
                if (!valore_trovato) {
                    throw SecKeyError();
                }
            }
        }
        else { //se la colonna è una chiave primaria, controllo che il valore che si sta cercando di aggiornare non sia già presente in un altro record
            bool flag_duplicate_found = false;
            for (int i = 0; i < _elementi_interi.size() && !flag_duplicate_found; i++) {
                if(_elementi_interi[i] == new_value)
                    flag_duplicate_found = true;
            }
            if(flag_duplicate_found) {
                throw PrimKeyError();
            }
            else //se non ci sono valori uguali presenti, l'aggiornamento è permesso
                _elementi_interi[index] = new_value;
        }
    }
}

void ColonnaInt::addDefault() {
    if(!_auto_increment) {
        _elementi_interi.push_back(_default_value);
    }
    else {
        _elementi_interi.push_back((*_increment_value)+1);
        (*_increment_value)++;
    }
}

bool ColonnaInt::compareElements(const string& condizione, int operatore, int index)const{
    int to_compare=std::stoi(condizione);
    switch (operatore){
        case 0:
            return (_elementi_interi[index] == to_compare);
        case 1:
            return (_elementi_interi[index] < to_compare);
        case 2:
            return (_elementi_interi[index] <= to_compare);
        case 3:
            return (_elementi_interi[index] > to_compare);
        case 4:
            return (_elementi_interi[index] >= to_compare);
        case 5:
            return (_elementi_interi[index] != to_compare);
        default:
            throw InvalidOperator();
    }
}

int ColonnaInt::getSize() const {
    return _elementi_interi.size();
}

bool ColonnaInt::isAutoIncrement() const {
    return _auto_increment;
}

string ColonnaInt::getTipo() const {
    return "int";
}
