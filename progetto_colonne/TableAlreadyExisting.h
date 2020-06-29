//
// Created by calog on 29/06/2020.
//

#ifndef PROGETTO_COLONNE_TABLEALREADYEXISTING_H
#define PROGETTO_COLONNE_TABLEALREADYEXISTING_H

#include <exception>
class TableAlreadyExisting : public std::exception {
public:
    TableAlreadyExisting()
            : _message ("Una tabella con lo stesso nome è già esistente nel database.") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_TABLEALREADYEXISTING_H
