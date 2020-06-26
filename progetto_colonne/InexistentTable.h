//
// Created by calog on 26/06/2020.
//

#ifndef PROGETTO_COLONNE_INEXISTENTTABLE_H
#define PROGETTO_COLONNE_INEXISTENTTABLE_H

#include <exception>
class InexistentTable : public std::exception {
public:
    InexistentTable() : _message ("ERR: tabella inesistente!") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;

};


#endif //PROGETTO_COLONNE_INEXISTENTTABLE_H
