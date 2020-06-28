//
// Created by jccx1 on 28/06/2020.
//

#ifndef PROGETTO_COLONNE_LINKEDERROR_H
#define PROGETTO_COLONNE_LINKEDERROR_H
#include <exception>

class LinkedError : public std::exception{
public:
    LinkedError()
            : _message ("Tentativo di modificare/eliminare una tabella in cui sono presenti collegamenti esterni") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};
#endif //PROGETTO_COLONNE_LINKEDERROR_H
