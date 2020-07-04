#ifndef PROGETTO_COLONNE_SECKEYNOTFOUND_H
#define PROGETTO_COLONNE_SECKEYNOTFOUND_H
#include <exception>

class SecKeyNotFound : public std::exception{
public:
    SecKeyNotFound()
            : _message ("Campo chiave esterna non esistente o non marcato come chiave primaria.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_SECKEYNOTFOUND_H
