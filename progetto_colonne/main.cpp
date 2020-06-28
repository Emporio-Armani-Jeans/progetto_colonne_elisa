#include <iostream>
#include "ColonnaText.h"
#include "ColonnaFloat.h"
#include "ColonnaInt.h"
#include "Tabella.h"
#include "FileError.h"
#include <vector>
#include <string>
#include <sstream>
#include "SalvataggioCaricamento.hpp"
#include "ControlloSintassi.h"
#include "Comandi.hpp"
#define ERR_COMANDO -1
#define ERRORE_ECCEZIONE 420

using namespace std;

enum comando {CREATE, DROP, INSERT, DELETE, TRUNCATE, UPDATE, SELECT,QUIT};


int compare_first_word_comandi(string &first_word);
string Gestione_caratteri_speciali(string &comando, string *status_message);

int main() {
    ControlloSintassi controllore;
    vector<Tabella*> tabelle;
    string nome_file, riga_comando, message_error, syntax_err = "ERR: Errore di sintassi nel comando, riprovare!",
    status_message, first_word;
    int *increment_value=new int;
    bool ok=false;

    vector<string> first_word_comandi {"CREATE", "DROP", "INSERT", "DELETE", "UPDATE", "SELECT","QUIT"},
    campi, valori, words;

    while(!ok) {
        try {
            cout << "Inserire nome file database" << endl;
            cin >> nome_file;
            tabelle = Caricamento(nome_file);
            ok = true;
        }
        catch (FileError &ex) {
            cout << "Exception occurred: " << ex.what() << endl;
        }
    }

    stringstream riga_temp;
    string comando;

    getchar();   // per l' "a capo" del cin precedente
    cout << "Inserire il comando: " << endl;

    //leggi comando intero
    getline(cin,comando);
    comando = Gestione_caratteri_speciali(comando,&status_message);

    cout << status_message << endl;
    stringstream comando_intero(comando);
    stringstream comando_per_controlli(comando);
    comando_intero >> first_word;

    while(compare_first_word_comandi(first_word)!=QUIT) {
        try {
            switch (compare_first_word_comandi(first_word)) {
                case CREATE :
                    if (controllore.controlloCreate(comando_per_controlli, &message_error)) {
                        /*Create(tabelle, comando_intero, increment_value, &status_message);
                        Salvataggio(nome_file, tabelle);
                        tabelle=Caricamento(nome_file);
                        cout << status_message << endl;*/
                    } else
                        cout << message_error << endl;
                    break;
                case DROP :
                    if (controllore.controlloDrop(comando_per_controlli, &message_error)) {
                        /*Drop(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        tabelle=Caricamento(nome_file);
                        cout << status_message << endl;*/
                    } else
                        cout << message_error << endl;
                    break;
                case INSERT :   //ricordarsi di controllare nell'implementazione comando che il vector di campi debba avere size uguale al vector di valori!!e controllare anche i _tipi e nelle classi la sintassi corretta delle date
                    if (controllore.controlloInsert(comando_per_controlli, &message_error)) {
                        /*Insert(tabelle, comando_intero, &status_message, increment_value);
                        Salvataggio(nome_file, tabelle);
                        tabelle=Caricamento(nome_file);
                        cout << status_message << endl;*/
                    } else
                        cout << message_error << endl;
                    break;
                case DELETE :
                    if (controllore.controlloDelete(comando_per_controlli, &message_error)) {
                        /*Delete(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        tabelle=Caricamento(nome_file);
                        cout << status_message << endl;*/
                    } else
                        cout << message_error << endl;
                    break;
                case TRUNCATE :
                    if (controllore.controlloTruncate(comando_per_controlli, &message_error)) {
                        /*Truncate(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        tabelle=Caricamento(nome_file);
                        cout << status_message << endl;*/
                    } else
                        cout << message_error << endl;
                    break;
                case UPDATE :
                    if (controllore.controlloUpdate(comando_per_controlli, &message_error)) {
                        /*Update(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        tabelle=Caricamento(nome_file);
                        cout << status_message << endl;*/
                    } else
                        cout << message_error << endl;
                    break;
                case SELECT :
                    if (controllore.controlloSelect(comando_per_controlli, &message_error)) {
                        /*Select(tabelle, comando_intero, &status_message);
                        cout << status_message << endl;*/
                    } else
                        cout << message_error << endl;
                    break;
                case QUIT :
                    break;
                default:
                    cout << syntax_err << endl;
                    break;
            }
        }
            //eccezioni gestibili --> richiedo il comando
            //eccezioni inaspettate --> chiudo il programma (non salvo progressi fatti durante ultimo comando)
        catch(InexistentTable &tab){
            cout << "Eccezione: " << tab.what() << endl;
        }
        catch(PrimKeyError &prk){
            cout << "Eccezione: " << prk.what() << endl;
        }
        catch(TentativoInserimentoAutoIncrement &tiai){
            cout << "Eccezione: " << tiai.what() << endl;
        }
        catch(FormatTypeError &type){
            cout << "Eccezione: " << type.what() << endl;
        }
        catch(InvalidCondition &cond){
            cout << "Eccezione: " << cond.what() << endl;
        }
        catch(InvalidOperator &op){
            cout << "Eccezione: " << op.what() << endl;
        }
        catch(NotNullError &nn){
            cout << "Eccezione: " << nn.what() << endl;
        }
        catch(PrimaryKeyAlreadyExisting &pka){
            cout << "Eccezione: " << pka.what() << endl;
        }
        catch(SecKeyAlreadyExisting &ska){
            cout << "Eccezione: " << ska.what() << endl;
        }
        catch(SecKeyError &ske){
            cout << "Eccezione: " << ske.what() << endl;
        }
        catch(SecKeyNotFound &skn){
            cout << "Eccezione: " << skn.what() << endl;
        }
        catch(ValueNotFound &vnf){
            cout << "Eccezione: " << vnf.what() << endl;
        }
        catch (UsedValueInSecKey &uvsk) {
            cout << "Eccezione: " << uvsk.what() << endl;
        }
        catch(exception &unexpected) {
            cout << "Eccezione: " << unexpected.what() << endl;
            exit(ERRORE_ECCEZIONE);
        }

        comando.clear();
        comando_intero.clear();
        comando_per_controlli.clear();
        comando_intero.str("");
        comando_per_controlli.str("");
        riga_comando.clear();

        cout << "Inserisci comando: " << endl;
        //leggi comando intero
        getline(cin,comando);

        status_message.clear();
        comando = Gestione_caratteri_speciali(comando,&status_message);
        cout << status_message << endl;
        comando_intero << comando;
        comando_per_controlli << comando;
        comando_intero >> first_word;
    }

    cout << "Salvataggio in corso..." << endl;

    Salvataggio(nome_file, tabelle);

    cout << "Arresto eseguito correttamente" << endl;

    return 0;
}


int compare_first_word_comandi(string &first_word) {
    first_word = toUpper(first_word);
    if(first_word == "CREATE")
        return CREATE;
    else if (first_word == "DROP")
        return DROP;
    else if (first_word == "INSERT")
        return INSERT;
    else if (first_word == "DELETE")
        return DELETE;
    else if (first_word == "TRUNCATE")
        return TRUNCATE;
    else if (first_word == "UPDATE")
        return UPDATE;
    else if (first_word == "SELECT")
        return SELECT;
    else if (first_word == "QUIT;")
        return QUIT;
    else
        return ERR_COMANDO;
}

string Gestione_caratteri_speciali(string &comando, string *status_message){
    bool found_text = false, campo_testo_presente = false, inside_testo = false;
    int contatore_virgolette = 0;
    vector<int> pos_first, pos_last;
    for (int j = 1; j < comando.size(); ++j) { //'"'
        if (!found_text){
            if (comando[j] == 34 && ((int)comando[j-1] != 39 && (int)comando[j+1] != 39)) { //se trovo un " e non è all'interno di un campo char
                found_text = true;
                campo_testo_presente = true;
                pos_first.push_back(j);
            }
        } // numero pari di " --> sono uscito, numero dispari --> non sono uscito
        else {
            if(comando[j] == 34) {
                contatore_virgolette++;
            }else{
                if(contatore_virgolette % 2 != 0){
                    found_text=false;
                    pos_last.push_back(j-1);
                }
                contatore_virgolette=0;
            }
        }
    }

    //controllo sui ';' nei campi di testo
    for (int k = 0; k < comando.size(); ++k) {
        inside_testo = false;
        if (!campo_testo_presente){
            //se trovo ';' e non è un campo char, trascuro il resto del comando perchè devo fermarmi
            if ( k!= comando.size()-1 && comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39) ){
                comando.erase(k+1);
            }
        }
        else{
            if (pos_first.size() != pos_last.size())
                (*status_message)="ERR: campo di testo non chiuso da apposite virgolette";
            else{
                for (int j = 0; j < pos_first.size(); ++j) {
                    if (k > pos_first[j] && k < pos_last[j]){
                        inside_testo = true;
                    }
                }
                if (!inside_testo){
                    if ( k != comando.size()-1 && comando[k] == ';' && ((int)comando[k-1] != 39 && (int)comando[k+1] != 39)){
                        comando.erase(k+1);
                    }
                }
            }
        }
    }
    return comando;
}
