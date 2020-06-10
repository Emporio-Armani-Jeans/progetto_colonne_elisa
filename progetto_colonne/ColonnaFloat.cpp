//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaFloat.h"

ColonnaFloat::ColonnaFloat(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _default_value=0.0;
}

string ColonnaFloat::getElement(int index){
    if(index==-1) return to_string(_default_value);
    return to_string(_elementi_float[index]);
}

void ColonnaFloat::deleteVal(int index) {
    auto it=_elementi_float.begin();
    it+=index;
    _elementi_float.erase(it);
}

void ColonnaFloat::updateVal(const string &val, int index) {
    float new_val = std::stof(val);
    _elementi_float[index]=new_val;
}

void ColonnaFloat::addDefault() {
    _elementi_float.push_back(_default_value);
}

bool ColonnaFloat::compareElements(const string& condizione, int operatore, int index)const{
    float to_compare=std::stof(condizione);
    switch (operatore){
        case 0:
            return _elementi_float[index]==to_compare;
        case 1:
            return _elementi_float[index]<to_compare;
        case 2:
            return _elementi_float[index]<=to_compare;
        case 3:
            return _elementi_float[index]>to_compare;
        case 4:
            return _elementi_float[index]>=to_compare;
        default:
            //creare eccezione
            return false;
    }
}