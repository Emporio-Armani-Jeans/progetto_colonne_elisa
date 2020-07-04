#ifndef PROGETTO_COLONNE_INVALIDMATCH_H
#define PROGETTO_COLONNE_INVALIDMATCH_H
#include <exception>

class InvalidMatch: public std::exception {
public:
    InvalidMatch()
            : _message ("Mancata corrispondenza tra i campi e i valori inseriti.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};



#endif //PROGETTO_COLONNE_INVALIDMATCH_H
