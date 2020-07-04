#ifndef PROGETTO_COLONNE_FORMATTYPEERROR_H
#define PROGETTO_COLONNE_FORMATTYPEERROR_H
#include <exception>

class FormatTypeError : public std::exception{
public:
    FormatTypeError()
            : _message ("Il formato dell' input non è compatibile con il tipo") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_FORMATTYPEERROR_H
