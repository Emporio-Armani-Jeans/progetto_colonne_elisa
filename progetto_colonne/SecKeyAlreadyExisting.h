//
// Created by andry on 20/06/2020.
//

#ifndef PROGETTO_COLONNE_SECKEYALREADYEXISTING_H
#define PROGETTO_COLONNE_SECKEYALREADYEXISTING_H
#include <exception>

class SecKeyAlreadyExisting : public std::exception{
public:
    SecKeyAlreadyExisting()
            : _message ("Una chiave secondaria è già associata alla colonna") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_SECKEYALREADYEXISTING_H
