#ifndef PROGETTO_COLONNE_FILEERROR_H
#define PROGETTO_COLONNE_FILEERROR_H
#include <exception>

class FileError : public std::exception{
public:
    FileError()
            : _message ("Errore nell'apertura del file database.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_FILEERROR_H
