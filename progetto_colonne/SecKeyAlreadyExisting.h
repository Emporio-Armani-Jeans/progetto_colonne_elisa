#ifndef PROGETTO_COLONNE_SECKEYALREADYEXISTING_H
#define PROGETTO_COLONNE_SECKEYALREADYEXISTING_H
#include <exception>

class SecKeyAlreadyExisting : public std::exception{
public:
    SecKeyAlreadyExisting()
            : _message ("Una chiave esterna è già stata associata alla colonna.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_SECKEYALREADYEXISTING_H
