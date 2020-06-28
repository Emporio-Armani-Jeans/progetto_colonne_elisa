//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaFloat.h"

ColonnaFloat::ColonnaFloat(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _default_value = 0.0;
    _primary_key = false;
    _foreign_key = nullptr;
}

string ColonnaFloat::getElement(int index){
    if(index == -1) return to_string(_default_value);
    return to_string(_elementi_float[index]);
}

void ColonnaFloat::deleteVal(int index) {
    auto it = _elementi_float.begin();
    it += index;
    _elementi_float.erase(it);
}

void ColonnaFloat::updateVal(const string &val, int index) {
    controlloFormato(val);
    float new_value = std::stof(val);
    if (!_primary_key) {
        if (_foreign_key == nullptr)
            _elementi_float[index] = new_value;
        else {
            bool valore_trovato = false;
            for (int i = 0; i < _foreign_key->getSize(); i++){
                if (_foreign_key->getElement(i) == val){
                    valore_trovato = true;
                    _elementi_float[index] = new_value;
                }
            }
            if (!valore_trovato) {
                throw SecKeyError();
            }
        }
    }
    else { //se la colonna è una chiave primaria, controllo che il valore che si sta cercando di aggiornare non sia già presente in un altro record
        bool flag_duplicate_found = false;
        for (int i = 0; i < _elementi_float.size() && !flag_duplicate_found; i++) {
            if(_elementi_float[i] == new_value)
                flag_duplicate_found = true;
        }
        if(flag_duplicate_found) {
            throw PrimKeyError();
        }
        else { //se non ci sono valori uguali presenti, l'aggiornamento è permesso
            if (_foreign_key == nullptr)
                _elementi_float[index] = new_value;
            else {
                bool valore_trovato = false;
                for (int i = 0; i < _foreign_key->getSize(); i++){
                    if (_foreign_key->getElement(i) == val){
                        valore_trovato = true;
                        _elementi_float[index] = new_value;
                    }
                }
                if (!valore_trovato) {
                    throw SecKeyError();
                }
            }
        }
    }
}

void ColonnaFloat::addDefault(int val) {
    _elementi_float.push_back(_default_value);
}

bool ColonnaFloat::compareElements(const string& condizione, int operatore, int index)const{
    float to_compare = std::stof(condizione);
    switch (operatore){
        case 0:
            return (_elementi_float[index] == to_compare);
        case 1:
            return (_elementi_float[index] < to_compare);
        case 2:
            return (_elementi_float[index] <= to_compare);
        case 3:
            return (_elementi_float[index] > to_compare);
        case 4:
            return (_elementi_float[index] >= to_compare);
        case 5:
            return (_elementi_float[index] != to_compare);
        default:
            throw InvalidOperator();
    }
}


int ColonnaFloat::getSize() const {
    return _elementi_float.size();
}

string ColonnaFloat::getTipo() const {
    return "float";
}

void ColonnaFloat::controlloFormato(const string &value) const {
    for(char i : value){          //controllo formato
        if((i<48 || i>57) && i!='.')
            throw FormatTypeError();
    }
}
