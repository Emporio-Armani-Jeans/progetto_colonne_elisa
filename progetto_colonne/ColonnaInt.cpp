//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaInt.h"

ColonnaInt::ColonnaInt(const string &nomecolonna, bool notnull, bool autoincrement, int* increment_value) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _auto_increment = autoincrement;
    _default_value=0;
    _increment_value=increment_value;
    _primary_key=false;
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
    if (!_auto_increment) {
        int new_value = std::stoi(val);
        if (!_primary_key) {
            _elementi_interi[index] = new_value;
        } else {
            bool flag=false;
            for (int i = 0; i < _elementi_interi.size() && !flag; i++) {
                if(_elementi_interi[i]==new_value) flag=true;
            }
            if(flag) {
                //eccezione doppione per primary key
            }else{
                _elementi_interi[index]=new_value;
            }
        }
    }
}

void ColonnaInt::addDefault() {
    if(!_auto_increment) {
        _elementi_interi.push_back(_default_value);
    }else{
        _elementi_interi.push_back((*_increment_value)+1);
        (*_increment_value)++;
    }
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