//
// Created by andry on 13/06/2020.
//

#ifndef PROGETTO_COLONNE_NOTNULLERROR_H
#define PROGETTO_COLONNE_NOTNULLERROR_H
#include <exception>

class NotNullError : public std::exception{
public:
    NotNullError()
            : _message ("Un campo not null non è stato inserito") {}
    const char* what()const noexcept override { return _message; }
private:
    const char* _message;
};


#endif //PROGETTO_COLONNE_NOTNULLERROR_H
