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

    int autoincrement = 0;
    list<Tabella*> tabelle;

    //creazione tabella + aggiunta colonne
    Tabella *tab1 = new Tabella("Customers");
   // tabelle.push_back(new Tabella("Customers"));
    tabelle.push_back(tab1);
    ColonnaInt *age = new ColonnaInt("Age");
    ColonnaFloat *salary = new ColonnaFloat("Salary");
    ColonnaText *address = new ColonnaText("Address");
    ColonnaDate *data = new ColonnaDate("Data_di_nascita");
    ColonnaTime *ora= new ColonnaTime("Ora_di_nascita");
    tab1->aggiungiColonna(age);
    tab1->aggiungiColonna(salary);
    tab1->aggiungiColonna(address);
    tab1->aggiungiColonna(data);
    tab1->aggiungiColonna(ora);

    string s1 = "Age", s2 = "Salary", s3 = "Address", s4="Data_di_nascita", s5="Ora_di_nascita";
    string s1_1 = "12", s2_1 = "35.6", s3_1 = "Via dei Gigli 33", s4_1="17/04/1999", s5_1="00:47:34";
    string s1_2 = "90", s2_2= "69.420", s3_2= "Via Petalosa 77", s4_2="04/06/1989", s5_2="12:56:59";

    //prova per impostare i valori:
    //attraverso la insert salviamo tutto in delle stringhe e con cicli controlli ecc: (cast all'interno della singola colonna per salvare il tipo giusto!)
    tab1->impostaValore(s1, s1_1);
    tab1->impostaValore(s2, s2_1);
    tab1->impostaValore(s3, s3_1);
    tab1->impostaValore(s4, s4_1);
    tab1->impostaValore(s5, s5_1);
    tab1->addRecord();           //da fare ogni volta che si è impostati i campi del nuovo rec
    tab1->impostaValore(s1, s1_2);
    tab1->impostaValore(s2, s2_2);
    tab1->impostaValore(s3, s3_2);
    tab1->impostaValore(s4, s4_2);
    tab1->impostaValore(s5, s5_2);
    tab1->addRecord();

    for(int i=0; i<tab1->numRecs(); i++) {
        if (tab1->recExists(i)) {
            for (int j = 0; j < tab1->numCampi(); j++) {
                cout << tab1->getCol(j)->getElemento(i) << " ";
            }
        }
        cout << endl;
    }

    tabelle.front()->deleteRecord("Address", "Via dei Gigli 33");
    for(int i=0; i<tab1->numRecs(); i++) {
        if (tab1->recExists(i)) {
            for (int j = 0; j < tab1->numCampi(); j++) {
                cout << tab1->getCol(j)->getElemento(i) << " ";
            }
        }
        cout << endl;
    }

    deleteOggettoTabella(&tab1);
    //rimozione tabella:
    /*  tabelle.remove(tab1); //elimina la tabella dalla lista delle tabelle
    deleteOggettoTabella(&tab1); //elimina l'oggetto */

    return 0;
}
