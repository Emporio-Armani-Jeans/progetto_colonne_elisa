//
// Created by andry on 13/06/2020.
//

#ifndef PROGETTO_COLONNE_SECKEYNOTFOUND_H
#define PROGETTO_COLONNE_SECKEYNOTFOUND_H
#include <exception>

class SecKeyNotFound : public std::exception{
public:
    SecKeyNotFound()
            : _message ("Campo chiave esterna non trovato") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_SECKEYNOTFOUND_H
