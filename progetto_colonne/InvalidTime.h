//
// Created by calog on 03/07/2020.
//

#ifndef PROGETTO_COLONNE_INVALIDTIME_H
#define PROGETTO_COLONNE_INVALIDTIME_H
#include <exception>

class InvalidTime : public std::exception{
public:
    InvalidTime()
            : _message ("Ora non valida.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};



#endif //PROGETTO_COLONNE_INVALIDTIME_H
