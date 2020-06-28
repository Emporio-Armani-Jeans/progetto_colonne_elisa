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

using namespace std;

enum comando {CREATE, DROP, INSERT, DELETE, TRUNCATE, UPDATE, SELECT,QUIT};


int compare_first_word_comandi(string &first_word);


int main() {
    ControlloSintassi controllore;
    vector<Tabella*> tabelle;
    string nome_file, riga_comando, message_error, syntax_err = "ERR: Errore di sintassi nel comando, riprovare!",
    status_message, first_word;
    bool correct_file = false;

    vector<string> first_word_comandi {"CREATE", "DROP", "INSERT", "DELETE", "UPDATE", "SELECT","QUIT"},
    campi, valori, words;

    while(!correct_file) {
        try {
            cout << "Inserire nome file database" << endl;
            cin >> nome_file;
            tabelle = Caricamento(nome_file);
            correct_file = true;
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
    comando = controllore.Gestione_caratteri_speciali(comando,&status_message);

    cout << status_message << endl;
    stringstream comando_intero(comando);
    stringstream comando_per_controlli(comando);
    comando_intero >> first_word;

    while(compare_first_word_comandi(first_word)!=QUIT) {
        try {
            switch (compare_first_word_comandi(first_word)) {
                case CREATE :
                    if (controllore.controlloCreate(comando_per_controlli, &message_error)) {
                        Create(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        cout << status_message << endl;
                    } else
                        cout << message_error << endl;
                    break;
                case DROP :
                    if (controllore.controlloDrop(comando_per_controlli, &message_error)) {
                        Drop(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        cout << status_message << endl;
                    } else
                        cout << message_error << endl;
                    break;
                case INSERT :
                    if (controllore.controlloInsert(comando_per_controlli, &message_error)) {
                        Insert(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        cout << status_message << endl;
                    } else
                        cout << message_error << endl;
                    break;
                case DELETE :
                    if (controllore.controlloDelete(comando_per_controlli, &message_error)) {
                        Delete(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        cout << status_message << endl;
                    } else
                        cout << message_error << endl;
                    break;
                case TRUNCATE :
                    if (controllore.controlloTruncate(comando_per_controlli, &message_error)) {
                        Truncate(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        cout << status_message << endl;
                    } else
                        cout << message_error << endl;
                    break;
                case UPDATE :
                    if (controllore.controlloUpdate(comando_per_controlli, &message_error)) {
                        Update(tabelle, comando_intero, &status_message);
                        Salvataggio(nome_file, tabelle);
                        cout << status_message << endl;
                    } else
                        cout << message_error << endl;
                    break;
                case SELECT :
                    if (controllore.controlloSelect(comando_per_controlli, &message_error)) {
                        Select(tabelle, comando_intero, &status_message);
                        cout << status_message << endl;
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
        catch(InexistentTable &tab){cout << "Eccezione: " << tab.what() << endl;}
        catch(PrimKeyError &prk){cout << "Eccezione: " << prk.what() << endl;}
        catch(TentativoInserimentoAutoIncrement &tiai){cout << "Eccezione: " << tiai.what() << endl;}
        catch(FormatTypeError &type){cout << "Eccezione: " << type.what() << endl;}
        catch(InvalidCondition &cond){cout << "Eccezione: " << cond.what() << endl;}
        catch(InvalidOperator &op){cout << "Eccezione: " << op.what() << endl;}
        catch(NotNullError &nn){cout << "Eccezione: " << nn.what() << endl;}
        catch(PrimaryKeyAlreadyExisting &pka){cout << "Eccezione: " << pka.what() << endl;}
        catch(SecKeyAlreadyExisting &ska){cout << "Eccezione: " << ska.what() << endl;}
        catch(SecKeyError &ske){cout << "Eccezione: " << ske.what() << endl;}
        catch(SecKeyNotFound &skn){cout << "Eccezione: " << skn.what() << endl;}
        catch(ValueNotFound &vnf){cout << "Eccezione: " << vnf.what() << endl;}
        catch (UsedValueInSecKey &uvsk) {cout << "Eccezione: " << uvsk.what() << endl;}
        catch(exception &unexpected) {
            cout << "Eccezione: " << unexpected.what() << endl;
            exit(EXIT_FAILURE);
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
        comando = controllore.Gestione_caratteri_speciali(comando,&status_message);
        cout << status_message << endl;
        comando_intero << comando;
        comando_per_controlli << comando;
        comando_intero >> first_word;
    }

    cout << "Salvataggio in corso..." << endl;

    Salvataggio(nome_file, tabelle); //salvo i dati sul file di testo

    for (auto & i : tabelle){ //elimino tabelle
        delete i;
    }

    cout << "Arresto eseguito correttamente." << endl;

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

