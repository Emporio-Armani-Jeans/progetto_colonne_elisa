#include <iostream>
#include "ColonnaText.h"
#include "ColonnaFloat.h"
#include "ColonnaInt.h"
#include "Colonna.h"
#include "Tabella.h"
#include <list>
#include <vector>

using namespace std;


void deleteOggettoTabella(Tabella **ptr){
    if (*ptr != nullptr){
        delete *ptr;
        *ptr = nullptr;
    }
}

int main() {

    int autoincrement = 0, a;
    list<Tabella*> tabelle;

    //creazione tabella + aggiunta colonne
    Tabella *tab1 = new Tabella("Customers");
    tabelle.push_back(tab1);
    ColonnaInt *age = new ColonnaInt("Age");
    ColonnaFloat *salary = new ColonnaFloat("Salary");
    ColonnaText *address = new ColonnaText("Address");
    tab1->aggiungiColonna(age);
    tab1->aggiungiColonna(salary);
    tab1->aggiungiColonna(address);

    //rimozione tabella:
    /*  tabelle.remove(tab1); //elimina la tabella dalla lista delle tabelle
    deleteOggettoTabella(&tab1); //elimina l'oggetto */

    string s1 = "Age", s2 = "Salary", s3 = "Address";
    string s1_1 = "12", s2_1 = "35.6", s3_1 = "Via dei Gigli 33";

    //prova per impostare i valori:
    //attraverso la insert salviamo tutto in delle stringhe e con cicli controlli ecc: (cast all'interno della singola colonna per salvare il tipo giusto!)
    tab1->impostaValore(s1,s1_1);
    tab1->impostaValore(s2,s2_1);
    tab1->impostaValore(s3,s3_1);



    return 0;
}
