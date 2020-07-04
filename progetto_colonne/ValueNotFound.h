#ifndef PROGETTO_COLONNE_VALUENOTFOUND_H
#define PROGETTO_COLONNE_VALUENOTFOUND_H
#include <exception>

class ValueNotFound : public std::exception {
public:
    ValueNotFound()
            : _message ("Nessun dato corrispondente ai criteri di ricerca.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_VALUENOTFOUND_H
