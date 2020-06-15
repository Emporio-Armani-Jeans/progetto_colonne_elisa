//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaText.h"

ColonnaText::ColonnaText(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _default_value = '\0';
    _primary_key = false;
    _foreign_key = nullptr;
}

string ColonnaText::getElement(int index){
    if(index == -1)
        return _default_value;
    else
        return _elementi_di_testo[index];
}

void ColonnaText::deleteVal(int index) {
    auto it = _elementi_di_testo.begin();
    it += index;
    _elementi_di_testo.erase(it);
}

void ColonnaText::updateVal(const string &val, int index) {
    if (!_primary_key) {
        if (_foreign_key == nullptr)
            _elementi_di_testo[index] = val;
        else {
            bool valore_trovato = false;
            for (int i = 0; i < _foreign_key->getSize(); i++){
                if (_foreign_key->getElement(i) == val){
                    valore_trovato = true;
                    _elementi_di_testo[index] = val;
                }
            }
            if (!valore_trovato) {
                throw SecKeyError();
            }
        }
    }
    else { //se la colonna è una chiave primaria, controllo che il valore che si sta cercando di aggiornare non sia già presente in un altro record
        bool flag_duplicate_found = false;
        for (int i = 0; i < _elementi_di_testo.size() && !flag_duplicate_found; i++) {
            if(_elementi_di_testo[i] == val)
                flag_duplicate_found = true;
        }
        if(flag_duplicate_found) {
            throw PrimKeyError();
        }
        else //se non ci sono valori uguali presenti, l'aggiornamento è permesso
            _elementi_di_testo[index] = val;
    }
}

void ColonnaText::addDefault() {
    _elementi_di_testo.push_back(_default_value);
}

bool ColonnaText::compareElements(const string& condizione, int operatore, int index)const{
    switch (operatore){
        case 0:
            return (_elementi_di_testo[index] == condizione);
        case 1:
            return (_elementi_di_testo[index] < condizione);
        case 2:
            return (_elementi_di_testo[index] <= condizione);
        case 3:
            return (_elementi_di_testo[index] > condizione);
        case 4:
            return (_elementi_di_testo[index] >= condizione);
        default:
            throw InvalidOperator();
    }
}

int ColonnaText::getSize() const {
    return _elementi_di_testo.size();
}

string ColonnaText::getTipo() const {
    return "text";
}
