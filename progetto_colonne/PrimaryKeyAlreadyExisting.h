#ifndef PROGETTO_COLONNE_PRIMARYKEYALREADYEXISTING_H
#define PROGETTO_COLONNE_PRIMARYKEYALREADYEXISTING_H
#include <exception>

class PrimaryKeyAlreadyExisting : public std::exception{
public:
    PrimaryKeyAlreadyExisting()
    : _message ("Chiave primaria già presente nella tabella.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_PRIMARYKEYALREADYEXISTING_H
