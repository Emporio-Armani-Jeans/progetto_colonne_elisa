#include <iostream>
#include "ColonnaText.h"
#include "ColonnaFloat.h"
#include "ColonnaInt.h"
#include "Tabella.h"
#include "FileError.h"
#include <list>
#include <vector>
#include <fstream>

using namespace std;


void deleteOggettoTabella(Tabella **ptr){
    if (*ptr != nullptr){
        delete *ptr;
        *ptr = nullptr;
    }
}
void Avvio(const string& nome_file);                     ////
void Arresto(const string& nome_file);                   ////

int main() {
    list<Tabella*> tabelle;
    vector<int> ciao;
    int increment=0;

    //creazione tabella + aggiunta colonne
    Tabella *tab1 = new Tabella("Customers");
    Tabella *tab2 = new Tabella("Tabellina");
    tabelle.push_back(tab1);
    tabelle.push_back(tab2);
    ColonnaInt *age = new ColonnaInt("Age");
    ColonnaInt *id=new ColonnaInt("Id", false, true, &increment);
    ColonnaFloat *salary = new ColonnaFloat("Salary");
    ColonnaText *address = new ColonnaText("Address");
    ColonnaDate *data = new ColonnaDate("Data_di_nascita");
    ColonnaInt *countries_id = new ColonnaInt("Countries_id");
    tab1->aggiungiColonna(age);
    tab1->aggiungiColonna(salary);
    tab1->aggiungiColonna(address);
    tab1->aggiungiColonna(data);
    tab1->aggiungiColonna(id);

    //chiavi
    tab1->setChiavePrimaria("Age");
    tab2->aggiungiColonna(countries_id);
    tab2->setChiavePrimaria("Countries_id");
    tab1->setChiaveEsterna(tab2, "Age", "Countries_id");

    //set elementi
    vector<string> vettore1;
    vector<string> valore1, valore2, valore3;

    vettore1.emplace_back("Countries_id");
    valore1.emplace_back("3");
    valore2.emplace_back("10");
    valore3.emplace_back("12");

    tab2->addRecord(vettore1,valore1);
    tab2->addRecord(vettore1, valore2);
    tab2->addRecord(vettore1, valore3);

    string s1 = "Age", s2 = "Salary", s3 = "Address", s4="Data_di_nascita", s5="Id";
    string s1_1 = "10", s2_1 = "35.6", s3_1 = "Via dei Gigli 33", s4_1="17/04/1999", s5_1="2";
    string s1_2 = "12", s2_2= "69.420", s3_2= "Via Petalosa 77", s4_2="04/06/1989", s5_2="2";
    string s1_3="3", s2_3="556.95", s3_3="Via Dal Cazzo 666", s4_3="23/07/2002", s5_3="2";

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

    //stampa
    for(int i=0; i<tab1->numRecs(); i++) {
        cout << tab1->returnData()[i] << endl;
    }
    cout << endl;

    //fine programma
    deleteOggettoTabella(&tab1);

    return 0;
}


void Avvio(const string& nome_file){                ////
    ifstream database;
    database.open(nome_file);
    if(!database){
        throw FileError();
    }else{
        int num_tabs, num_cols, numero;
        string parola;
        vector<string> nomi_colonne;
        database >> num_tabs >> num_cols;
        for(int i=0; i<num_tabs; i++){
            //salvataggio della i-esima tabella
            for(int i_col=0; i_col<num_cols; i_col++){        //da terminare
                database >> parola;
                nomi_colonne.push_back(parola);
            }
        }
    }
}
