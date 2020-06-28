//
// Created by jccx1 on 28/06/2020.
//

#ifndef PROGETTO_COLONNE_USEDVALUEINSECKEY_H
#define PROGETTO_COLONNE_USEDVALUEINSECKEY_H
#include <exception>

class UsedValueInSecKey : public std::exception{
public:
    UsedValueInSecKey()
            : _message ("Il valore è usato in una chave secondaria collegata a questa tabella, per tanto non può essere modificato."
                        " Per modificare questo record, modificare la tabella figlia.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};

#endif //PROGETTO_COLONNE_USEDVALUEINSECKEY_H
