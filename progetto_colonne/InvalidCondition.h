#ifndef PROGETTO_COLONNE_INVALIDCONDITION_H
#define PROGETTO_COLONNE_INVALIDCONDITION_H
#include <exception>

class InvalidCondition : public std::exception{
public:
    InvalidCondition()
            : _message ("Campo condizione non valido") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};

#endif //PROGETTO_COLONNE_INVALIDCONDITION_H
