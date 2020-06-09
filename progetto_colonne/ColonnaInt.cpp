//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaInt.h"

ColonnaInt::ColonnaInt(const string &nomecolonna, bool notnull, bool autoincrement) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _auto_increment = autoincrement;
    _default_value=0;
}

void ColonnaInt::addVal(const string &valore_da_impostare) {
    int value_to_be_added = std::stoi(valore_da_impostare);
    _elementi_interi.push_back(value_to_be_added);
}

string ColonnaInt::getElement(int index) {
    if(index==-1) return to_string(_default_value);
    else return to_string(_elementi_interi[index]);
}

void ColonnaInt::deleteVal(int index){
    auto it = _elementi_interi.begin();
    it += index;
    _elementi_interi.erase(it);
}

void ColonnaInt::updateVal(const string& val, int index){
    int new_value = std::stoi(val);
    _elementi_interi[index]=new_value;
}

void ColonnaInt::addDefault() {
    _elementi_interi.push_back(_default_value);
}

bool ColonnaInt::compareElements(const string& condizione, int operatore, int index)const{
    int to_compare=std::stoi(condizione);
    switch (operatore){
        case 0:
            return _elementi_interi[index]==to_compare;
        case 1:
            return _elementi_interi[index]<to_compare;
        case 2:
            return _elementi_interi[index]<=to_compare;
        case 3:
            return _elementi_interi[index]>to_compare;
        case 4:
            return _elementi_interi[index]>=to_compare;
        default:
            //creare eccezione
            return false;
    }
}