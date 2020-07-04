#ifndef PROGETTO_COLONNE_INVALIDDATE_H
#define PROGETTO_COLONNE_INVALIDDATE_H
#include <exception>

class InvalidDate : public std::exception{
public:
    InvalidDate()
            : _message ("Data non valida.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_INVALIDDATE_H
