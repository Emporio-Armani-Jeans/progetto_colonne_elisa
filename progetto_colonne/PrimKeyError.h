//
// Created by andry on 13/06/2020.
//

#ifndef PROGETTO_COLONNE_PRIMKEYERROR_H
#define PROGETTO_COLONNE_PRIMKEYERROR_H
#include <exception>

class PrimKeyError : public std::exception{
public:
    PrimKeyError()
            : _message ("Valore duplicato nel campo chiave primaria") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_PRIMKEYERROR_H