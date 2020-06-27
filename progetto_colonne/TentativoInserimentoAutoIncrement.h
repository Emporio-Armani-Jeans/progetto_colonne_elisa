//
// Created by andry on 27/06/2020.
//

#ifndef PROGETTO_COLONNE_TENTATIVOINSERIMENTOAUTOINCREMENT_H
#define PROGETTO_COLONNE_TENTATIVOINSERIMENTOAUTOINCREMENT_H
#include <exception>

class TentativoInserimentoAutoIncrement : public std::exception{
public:
    TentativoInserimentoAutoIncrement()
            : _message ("Tentativo di inserire un valore auto increment") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_TENTATIVOINSERIMENTOAUTOINCREMENT_H
