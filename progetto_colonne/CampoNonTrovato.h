//
// Created by andry on 13/06/2020.
//

#ifndef PROGETTO_COLONNE_CAMPONONTROVATO_H
#define PROGETTO_COLONNE_CAMPONONTROVATO_H
#include <exception>
using namespace std;

class CampoNonTrovato : public exception{
public:
    CampoNonTrovato()
       : _message("Colonna non trovata") {}
    const char* what()const noexcept override {
        return _message;
    }
private:
    const char *_message;
};


#endif //PROGETTO_COLONNE_CAMPONONTROVATO_H
