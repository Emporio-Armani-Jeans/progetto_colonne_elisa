#include <iostream>
#include "ColonnaText.h"
#include "ColonnaFloat.h"
#include "ColonnaInt.h"
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
    list<Tabella*> tabelle;
    vector<int> ciao;
    int increment=0;

    //creazione tabella + aggiunta colonne
    Tabella *tab1 = new Tabella("Customers");
    tabelle.push_back(tab1);
    ColonnaInt *age = new ColonnaInt("Age");
    ColonnaInt *id=new ColonnaInt("Id", false, true, &increment);
    ColonnaFloat *salary = new ColonnaFloat("Salary");
    ColonnaText *address = new ColonnaText("Address");
    ColonnaDate *data = new ColonnaDate("Data_di_nascita");
    tab1->aggiungiColonna(age);
    tab1->aggiungiColonna(salary);
    tab1->aggiungiColonna(address);
    tab1->aggiungiColonna(data);
    tab1->aggiungiColonna(id);
    tab1->setChiavePrimaria("Id");

    string s1 = "Age", s2 = "Salary", s3 = "Address", s4="Data_di_nascita", s5="Id";
    string s1_1 = "12", s2_1 = "35.6", s3_1 = "Via dei Gigli 33", s4_1="17/04/1999", s5_1="2";
    string s1_2 = "9", s2_2= "69.420", s3_2= "Via Petalosa 77", s4_2="04/06/1989", s5_2="2";
    string s1_3="8", s2_3="556.95", s3_3="Via Dal Cazzo 666", s4_3="23/07/2002", s5_3="2";

    //prova per impostare i valori:
    //attraverso la insert salviamo tutto in delle stringhe e con cicli controlli ecc: (cast all'interno della singola colonna per salvare il tipo giusto!)
    vector<string> v1, v2, v3, v4;

    v1.push_back(s1);
    v1.push_back(s2);
    v1.push_back(s4);
    v1.push_back(s5);

    v2.push_back(s1_1);
    v2.push_back(s2_1);
    v2.push_back(s4_1);
    v2.push_back(s5_1);

    tab1->addRecord(v1, v2);

    v1.push_back(s3);

    v3.push_back(s1_2);
    v3.push_back(s2_2);
    v3.push_back(s4_2);
    v3.push_back(s5_2);
    v3.push_back(s3_2);
    tab1->addRecord(v1, v3);

    v4.push_back(s1_3);
    v4.push_back(s2_3);
    v4.push_back(s4_3);
    v4.push_back(s5_3);
    v4.push_back(s3_3);
    tab1->addRecord(v1, v4);

    for(int i=0; i<tab1->numRecs(); i++) {
        cout << tab1->returnData()[i] << endl;
    }
    cout << endl;

    //const vector<string>& campi, const string& campo_condizione, const string& condizione1,
    //            const string& condizione2, const string& campo_ordinamento=string(), int operatore_ordinamento=0)
    vector<string> campi;
    campi.emplace_back("Age");
    campi.emplace_back("Address");

    for(int j=0; j<tab1->returnData(campi, "Age", "5", 4, "Age", 4).size(); j++){
        cout << tab1->returnData(campi, "Age", "5", 4, "Age", 4)[j] << endl;
    }
    cout << endl;

    deleteOggettoTabella(&tab1);
    //rimozione tabella:
    /*  tabelle.remove(tab1); //elimina la tabella dalla lista delle tabelle
    deleteOggettoTabella(&tab1); //elimina l'oggetto */

    return 0;
}
