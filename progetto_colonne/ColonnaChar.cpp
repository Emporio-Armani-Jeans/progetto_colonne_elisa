//
// Created by Elisa Messina on 02/06/2020.
//

#include "ColonnaChar.h"

ColonnaChar::ColonnaChar(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _default_value = '\0';
    _primary_key = false;
    _foreign_key = nullptr;
}
/*
void ColonnaChar::addVal(const string &valore_da_impostare) {
    char value_to_be_added = valore_da_impostare[0];
    _elementi_char.push_back(value_to_be_added);
}
*/
string ColonnaChar::getElement(int index) {
    string str_to_return;
    if(index == -1){
        str_to_return.push_back(_default_value);
    }
    else
        str_to_return.push_back(_elementi_char[index]);
    return str_to_return;
}

void ColonnaChar::deleteVal(int index) {
    auto it = _elementi_char.begin();
    it += index;
    _elementi_char.erase(it);
}

void ColonnaChar::updateVal(const string &val, int index) {
    char new_value = val[0];
    if (!_primary_key) {
        if (_foreign_key == nullptr)
            _elementi_char[index] = new_value;
        else {
            bool valore_trovato = false;
            for (int i = 0; i < _foreign_key->getSize(); i++){
                if (_foreign_key->getElement(i) == val){
                    valore_trovato = true;
                    _elementi_char[index] = new_value;
                }
            }
            if (!valore_trovato) {
                //eccezione valore non esistente quindi non valido
            }
        }
    }
    else { //se la colonna è una chiave primaria, controllo che il valore che si sta cercando di aggiornare non sia già presente in un altro record
        bool flag_duplicate_found = false;
        for (int i = 0; i < _elementi_char.size() && !flag_duplicate_found; i++) {
            if(_elementi_char[i] == new_value)
                flag_duplicate_found = true;
        }
        if(flag_duplicate_found) {
            //eccezione doppione per primary key
        }
        else //se non ci sono valori uguali presenti, l'aggiornamento è permesso
            _elementi_char[index] = new_value;
    }
}

void ColonnaChar::addDefault() {
    _elementi_char.push_back(_default_value);
}

bool ColonnaChar::compareElements(const string& condizione, int operatore, int index)const{
    switch (operatore){
        case 0:
            return (_elementi_char[index] == condizione[0]);
        case 1:
            return (_elementi_char[index] < condizione[0]);
        case 2:
            return (_elementi_char[index] <= condizione[0]);
        case 3:
            return (_elementi_char[index] > condizione[0]);
        case 4:
            return (_elementi_char[index] >= condizione[0]);
        default:
            return false;
            //creare eccezione
    }
}

int ColonnaChar::getSize() const {
    return _elementi_char.size();
}
