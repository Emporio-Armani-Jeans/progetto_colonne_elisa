//
// Created by Elisa Messina on 01/06/2020.
//

#include "ColonnaText.h"

ColonnaText::ColonnaText(const string &nomecolonna, bool notnull) {
    _nome_colonna = nomecolonna;
    _not_null = notnull;
    _default_value='\0';
}

string ColonnaText::getElement(int index){
    if(index==-1) return _default_value;
    else return _elementi_di_testo[index];
}

void ColonnaText::deleteVal(int index) {
    auto it=_elementi_di_testo.begin();
    it+=index;
    _elementi_di_testo.erase(it);
}

void ColonnaText::updateVal(const string &val, int index) {
    _elementi_di_testo[index]=val;
}

void ColonnaText::addDefault() {
    _elementi_di_testo.push_back(_default_value);
}

bool ColonnaText::compareElements(const string& condizione, int operatore, int index)const{
    switch (operatore){
        case 0:
            return _elementi_di_testo[index]==condizione;
        case 1:
            return _elementi_di_testo[index]<condizione;
        case 2:
            return _elementi_di_testo[index]<=condizione;
        case 3:
            return _elementi_di_testo[index]>condizione;
        case 4:
            return _elementi_di_testo[index]>=condizione;
        default:
            //creare eccezione
            return false;
    }
}