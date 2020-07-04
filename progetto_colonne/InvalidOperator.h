#ifndef PROGETTO_COLONNE_INVALIDOPERATOR_H
#define PROGETTO_COLONNE_INVALIDOPERATOR_H
#include <exception>

class InvalidOperator : public std::exception{
public:
    InvalidOperator()
            : _message ("Operatore non valido.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_INVALIDOPERATOR_H
