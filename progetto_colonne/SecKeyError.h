//
// Created by andry on 13/06/2020.
//

#ifndef PROGETTO_COLONNE_SECKEYERROR_H
#define PROGETTO_COLONNE_SECKEYERROR_H
#include <exception>

class SecKeyError : public std::exception{
    public:
    SecKeyError()
    : _message ("Mancata corrispondenza con i valori della chiave esterna") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_SECKEYERROR_H
