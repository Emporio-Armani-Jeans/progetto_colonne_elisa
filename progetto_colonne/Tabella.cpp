//
// Created by Elisa Messina on 01/06/2020.
//

#include "Tabella.h"

Tabella::Tabella(const string &nometabella) {
    _nome_tabella = nometabella;
    _recs=0;
}

Tabella::~Tabella() {
    for(auto & i : _colonne){
        delete i;
        i = nullptr;
    }
}

void Tabella::aggiungiColonna(Colonna *to_be_added) {
    _colonne.push_back(to_be_added);
}

void Tabella::setChiavePrimaria(const string& nomecolonna) {
    bool flag_another_pk_found = false, flag_colonna_trovata = false;
    for(auto & i : _colonne) {   //verifica che non ci siano altre colonne marcate come primary key
        if(i->_primary_key)
            flag_another_pk_found = true;
    }
    if(!flag_another_pk_found){ //se effettivamente non è ancora stata aggiunta una colonna marcata come primary key, posso impostarla
        for(int j = 0; j < _colonne.size() && !flag_colonna_trovata; j++){
            if(_colonne[j]->getNomeColonna() == nomecolonna){
                _colonne[j]->_primary_key = true;
                _colonne[j]->_not_null = !(_colonne[j]->getTipo() == "int" && _colonne[j]->isAutoIncrement());
                flag_colonna_trovata = true;
            }
        }if(!flag_colonna_trovata)
            throw CampoNonTrovato();
    }
    else{
        throw PrimaryKeyAlreadyExisting();
    }
}

Colonna *Tabella::getCol(int index) const {
    return _colonne[index];
}

int Tabella::numCampi() const {
    return _colonne.size();
}

int Tabella::numRecs() const {
    return _recs;
}

void Tabella::addRecord(const vector<string>& campi, const vector<string>& valori) {
    bool flag_campo_non_trovato = false, flag_colonna_trovata = false;
    for(int i = 0; i < _colonne.size() && !flag_campo_non_trovato; i++){
        flag_colonna_trovata = false;
        if(_colonne[i]->_not_null){ //se la colonna è marcata come not null, all'interno dei campi da modificare deve esserci il suo nome
            for(int j=0; j<campi.size() && !flag_colonna_trovata; j++){
                if(_colonne[i]->getNomeColonna() == campi[j]) {
                    flag_colonna_trovata = true;
                    if (_colonne[i]->isAutoIncrement())
                        throw TentativoInserimentoAutoIncrement();
                }
            }
            if(!flag_colonna_trovata)
                flag_campo_non_trovato = true;
        }
    }
    if(!flag_campo_non_trovato) { //se tutte le colonne marcate come not null sono presenti nei campi, posso procedere con l'aggiunta del record
        _recs++;
        for (auto &i : _colonne) {
            i->addDefault(0);
        }
        for (int i = 0; i < campi.size(); i++) {
            for (auto &j : _colonne) {
                if (campi[i] == j->getNomeColonna())
                    j->updateVal(valori[i], int(_recs - 1));
            }
        }
    }else{
        throw NotNullError(); //campo che doveva essere obbligatorio non inserito
    }
}

void Tabella::deleteRecord(const string& campo_condizione, const string &condizione, int operatore) {
    bool campo_condizione_trovato = false, condizione_trovata=false;
    int i = 0, j = 0;
    if(campo_condizione.empty() && condizione.empty()){   //se parametri sono tutti default cancello tutti i record
        for(int x=0; x<numCampi(); x++){
            for(int y=0; y<numRecs(); y++){
                _colonne[x]->deleteVal(0);
            }
        }
        _recs=0;
    }else {
        while (i < _colonne.size() && !campo_condizione_trovato) {
            if (campo_condizione == _colonne[i]->getNomeColonna())
                campo_condizione_trovato = true;
            else i++;
        }
        if (campo_condizione_trovato) {
            _colonne[i]->controlloFormato(condizione); //prima di eliminare verifico che il formato della condizione sia giusto
            while (j < _recs) {
                if (_colonne[i]->compareElements(condizione, operatore, j)) {
                    condizione_trovata = true;
                    if(!erroreSecKey(j)) {
                        _recs--;
                        for (auto &elem : _colonne) {
                            elem->deleteVal(j);
                        }
                    } else j++;
                } else j++;
            }
            if(!condizione_trovata)
                throw ValueNotFound();
            // inserire eccezione se nessun record viene modificato per la secondary key
        } else {
            throw InvalidCondition();
        }
    }
}

void Tabella::deleteRecord(const string& campo_condizione, const string& condizione1, const string& condizione2){
    bool trovata=false, trovata2=false;
    int i=0, j=0;
    while(i<_colonne.size() && !trovata){
        if(campo_condizione == _colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        _colonne[i]->controlloFormato(condizione1);//prima di eliminare verifico che il formato delle condizioni sia giusto
        _colonne[i]->controlloFormato(condizione2);
        while(j<_recs){
            if (_colonne[i]->compareElements(condizione1, 4, j) && _colonne[i]->compareElements(condizione2, 2, j)) {
                trovata2 = true;
                if(!erroreSecKey(j)) {
                    _recs--;
                    for (auto &elem : _colonne) {
                        elem->deleteVal(j);
                    }
                } else j++;
            }else j++;
        }
        if(!trovata2)
            throw ValueNotFound();
        // inserire eccezione se nessun record viene cancellato per la secondary key
    }else{
        throw InvalidCondition();
    }
}

void Tabella::updateRecord(const string& campo_condizione,const string& condizione, const vector<string>& campi, const vector<string>& valori, int operatore){
    bool trovata=false, trovata2 = false;
    int i=0, j;
    while(i<_colonne.size() && !trovata){
        if(campo_condizione == _colonne[i]->getNomeColonna()) trovata=true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs; j++){
            if(_colonne[i]->compareElements(condizione, operatore, j)) {
                trovata2 = true;
                        for (int y = 0; y < campi.size(); y++) {
                            for (auto &g : _colonne) {
                                if (campi[y] == g->getNomeColonna()) {
                                    if(!erroreSecKey(j) || !g->isKey()) {
                                        if (g->isAutoIncrement())
                                            throw TentativoInserimentoAutoIncrement();
                                        else
                                            g->updateVal(valori[y], j);
                                    }
                                }
                            }
                        }
            }
        }
        if(!trovata2)
            throw ValueNotFound();
        // inserire eccezione se nessun record viene modificato per la secondary key
    }else{
        throw InvalidCondition();
    }
}

void Tabella::updateRecord(const string& campo_condizioni, const string& condizione1, const string& condizione2, const vector<string>& campi, const vector<string>& valori){
    bool trovata=false, trovata2 = false;
    int i=0, j;
    while(i<_colonne.size() && !trovata){
        if(campo_condizioni == _colonne[i]->getNomeColonna())
            trovata=true;
        else i++;
    }
    if(trovata){
        for(j=0; j<_recs; j++){
            if(_colonne[i]->compareElements(condizione1, 4, j) && _colonne[i]->compareElements(condizione2, 2, j)) {
                trovata2 = true;
                    for (int y = 0; y < campi.size(); y++) {
                        for (auto &g : _colonne) {
                            if (campi[y] == g->getNomeColonna()) {
                                if(!erroreSecKey(j) || !_colonne[i]->isKey()) {
                                    if (g->isAutoIncrement())
                                        throw TentativoInserimentoAutoIncrement();
                                    else
                                        g->updateVal(valori[y], j);
                                }
                            }
                        }
                    }
            }
        }
        if(!trovata2)
            throw ValueNotFound();
        // inserire eccezione se nessun record viene modificato per la secondary key
    }else{
        throw InvalidCondition();
    }
}

vector<string> Tabella::returnData(const vector<string>& campi, const string& campo_ordinamento, int operatore_ordinamento) const {
    vector<string> righe_testo;
    string riga;
    if(_recs!=0) {
        for(const string & a : campi){
            riga += a + " ";
        }
        righe_testo.push_back(riga);
        for (int i = 0; i < _recs; i++) {
            riga.clear();
            for (auto j : _colonne) {
                for (const auto &s : campi) {
                    if (j->getNomeColonna() == s) {
                        if (operatore_ordinamento == 0) {
                            if (j->getElement(i) == j->getElement(-1)) {
                                riga += "___ ";
                            } else {
                                int q = 0;
                                string temp = j->getElement(i);
                                while (q < temp.size()){
                                    if (temp[q] == '"' && temp[q+1] == '"')
                                        temp.erase(q,1);
                                    else
                                        q++;
                                }
                                riga += temp;
                                riga += " ";
                            }
                        } else {
                            vector<int> indici_ordinati = ordinamento(campo_ordinamento, operatore_ordinamento);
                            if (j->getElement(indici_ordinati[i]) == j->getElement(-1)) {
                                riga += "___ ";
                            } else {
                                riga += j->getElement(indici_ordinati[i]);
                                riga += " ";
                            }
                        }
                    }
                }
            }
            righe_testo.push_back(riga);
        }
    }else{
        righe_testo.emplace_back("La tabella è vuota.");
    }

    return righe_testo;
}

vector<string> Tabella::returnData(const vector<string> &campi, const string& campo_condizione, const string &condizione,
        int operatore, const string& campo_ordinamento, int operatore_ordinamento)const {
    vector<string> righe_testo;
    righe_testo.clear();
    string riga;
    if(_recs!=0) {
        bool trovata = false;
        int a = 0;
        while (a < _colonne.size() && !trovata) {
            if (campo_condizione == _colonne[a]->getNomeColonna()) trovata = true;
            else a++;
        }
        if (trovata) {
            for(const string & b : campi){
                riga += b + " ";
            }
            righe_testo.push_back(riga);
            for (int i = 0; i < _recs; i++) {
                if (operatore_ordinamento == 0) {
                    if (_colonne[a]->compareElements(condizione, operatore, i)) {
                        riga.clear();
                        for (auto j : _colonne) {
                            for (const auto &s : campi) {
                                if (j->getNomeColonna() == s) {
                                    if (j->getElement(i) == j->getElement(-1)) {
                                        riga += "___ ";
                                    } else {
                                        int q = 0;
                                        string temp = j->getElement(i);
                                        while (q < temp.size()){
                                            if (temp[q] == '"' && temp[q+1] == '"')
                                                temp.erase(q,1);
                                            else
                                                q++;
                                        }
                                        riga += temp;
                                        riga += " ";
                                    }
                                }
                            }
                        }
                        righe_testo.push_back(riga);
                    }
                } else {
                    vector<int> indici_ordinati = ordinamento(campo_ordinamento, operatore_ordinamento);
                    if (_colonne[a]->compareElements(condizione, operatore, indici_ordinati[i])) {
                        riga.clear();
                        for (auto j : _colonne) {
                            for (const auto &s : campi) {
                                if (j->getNomeColonna() == s) {
                                    if (j->getElement(indici_ordinati[i]) == j->getElement(-1)) {
                                        riga += "___ ";
                                    } else {
                                        int q = 0;
                                        string temp = j->getElement(indici_ordinati[i]);
                                        while (q < temp.size()){
                                            if (temp[q] == '"' && temp[q+1] == '"')
                                                temp.erase(q,1);
                                            else
                                                q++;
                                        }
                                        riga += temp;
                                        riga += " ";
                                    }
                                }
                            }
                        }
                        righe_testo.push_back(riga);
                    }
                }
            }
        } else {
            throw InvalidCondition();
        }
    }else{
        righe_testo.emplace_back("Tabella vuota");
    }
    return righe_testo;
}

vector<string> Tabella::returnData(const vector<string> &campi, const string& campo_condizione,
        const string &condizione1, const string& condizione2, const string& campo_ordinamento,
        int operatore_ordinamento) const{
    vector<string> righe_testo;
    righe_testo.clear();
    string riga;
    if(_recs!=0) {
        bool trovata = false;
        int a = 0;
        while (a < _colonne.size() && !trovata) {
            if (campo_condizione == _colonne[a]->getNomeColonna()) trovata = true;
            else a++;
        }
        if (trovata) {
            for(const string & z : campi){
                riga += z + " ";
            }
            righe_testo.push_back(riga);
            for (int i = 0; i < _recs; i++) {
                if (operatore_ordinamento == 0) {
                    if (_colonne[a]->compareElements(condizione1, 4, i) &&
                        _colonne[a]->compareElements(condizione2, 2, i)) {
                        riga.clear();
                        for (auto j : _colonne) {
                            for (const auto &s : campi) {
                                if (j->getNomeColonna() == s) {
                                    if (j->getElement(i) == j->getElement(-1)) {
                                        riga += "___ ";
                                    } else {
                                        int q = 0;
                                        string temp = j->getElement(i);
                                        while (q < temp.size()){
                                            if (temp[q] == '"' && temp[q+1] == '"')
                                                temp.erase(q,1);
                                            else
                                                q++;
                                        }
                                        riga += temp;
                                        riga += " ";
                                    }
                                }
                            }
                        }
                        righe_testo.push_back(riga);
                    }
                } else {
                    vector<int> indici_ordinati = ordinamento(campo_ordinamento, operatore_ordinamento);
                    if (_colonne[a]->compareElements(condizione1, 4, indici_ordinati[i]) &&
                        _colonne[a]->compareElements(condizione2, 2, indici_ordinati[i])) {
                        riga.clear();
                        for (auto j : _colonne) {
                            for (const auto &s : campi) {
                                if (j->getNomeColonna() == s) {
                                    if (j->getElement(indici_ordinati[i]) == j->getElement(-1)) {
                                        riga += "___ ";
                                    } else {
                                        int q = 0;
                                        string temp = j->getElement(indici_ordinati[i]);
                                        while (q < temp.size()){
                                            if (temp[q] == '"' && temp[q+1] == '"')
                                                temp.erase(q,1);
                                            else
                                                q++;
                                        }
                                        riga += temp;
                                        riga += " ";
                                    }
                                }
                            }
                        }
                        righe_testo.push_back(riga);
                    }
                }
            }
        } else {
            throw InvalidCondition();
        }
    }else{
        righe_testo.emplace_back("Tabella vuota");
    }
    return righe_testo;
}

vector<int> Tabella::ordinamento(const string &campo, int operatore) const {
    if(operatore==1 || operatore==3) {
        vector<int> indici(_recs);
        int tmp, c;
        for (int i = 0; i < _recs; i++) {
            indici[i] = i;
        }
        for (c = 0; c < _colonne.size(); c++) {
            if (_colonne[c]->getNomeColonna() == campo) break;
        }
        if (c < _colonne.size()) {
            for (int i = 0; i < _recs-1; i++) {
                for (int j = i+1; j < _recs; j++) {
                    if (_colonne[c]->compareElements(_colonne[c]->getElement(indici[i]), operatore, indici[j])) {
                        tmp=indici[i];
                        indici[i] = indici[j];
                        indici[j] = tmp;
                    }
                }
            }
            return indici;
        } else {
            throw CampoNonTrovato();
        }
    }else{
        throw InvalidOperator();
    }
}

void Tabella::setChiaveEsterna(Tabella* tabella_to_link, const string& colonna_this, const string& chiave_esterna) {
    int pos_colonna_madre = INT_MAX, pos_colonna_figlia = INT_MAX;
    for (int i = 0; i < tabella_to_link->_colonne.size(); i++){
       if (tabella_to_link->_colonne[i]->_primary_key && tabella_to_link->_colonne[i]->getNomeColonna() == chiave_esterna)
           pos_colonna_madre = i;
    }
    if (pos_colonna_madre < tabella_to_link->_colonne.size()){
        for (int j = 0; j < _colonne.size(); j++){
            if (_colonne[j]->getNomeColonna() == colonna_this) {
                pos_colonna_figlia = j;
            }
        }
        if(_colonne[pos_colonna_figlia]->_foreign_key== nullptr) {
            if (pos_colonna_figlia < _colonne.size()) {
                if (_colonne[pos_colonna_figlia]->getTipo() == tabella_to_link->_colonne[pos_colonna_madre]->getTipo()){
                    _colonne[pos_colonna_figlia]->_foreign_key = tabella_to_link->_colonne[pos_colonna_madre];
                    tabella_to_link->_colonne[pos_colonna_madre]->_colonna_figlio = _colonne[pos_colonna_figlia];
                    _colonne[pos_colonna_figlia]->_tab_madre=tabella_to_link->getNome();
                } else
                    throw FormatTypeError();
            } else {
                throw CampoNonTrovato();
            }
        }else{
            throw SecKeyAlreadyExisting();
        }
    } else {
        throw SecKeyNotFound();
    }
}

string Tabella::getNome() const {
    return _nome_tabella;
}

void Tabella::addRecordMemory(const vector<string> &campi, const vector<string> &valori) {
    int a;
    bool flag_campo_non_trovato = false, flag_colonna_trovata = false;
        _recs++;
        for(a=0; a < campi.size(); a++){
            for (auto &i : _colonne) {
                if (campi[a] == i->getNomeColonna()) {
                    if (i->isAutoIncrement())
                        i->addDefault(stoi(valori[a]));
                    else
                        i->addDefault(0);
                }
            }
        }
        for (int i = 0; i < campi.size(); i++) {
            for (auto &j : _colonne) {
                if (campi[i] == j->getNomeColonna()) {
                   j->updateVal(valori[i], int(_recs - 1));
                }
            }
        }
}

bool Tabella::erroreSecKey(int indice) {
    for (auto item : _colonne) {
        if (item->isKey()) {
            if (item->_colonna_figlio != nullptr) {
                bool already_used = false;
                for (int i = 0; i < (item->_colonna_figlio)->getSize(); i++) { //scorro la colonna figlia
                    if ((item->compareElements((item->_colonna_figlio)->getElement(i), 0, indice)))
                        already_used = true;
                }
                return already_used;
            }
        }
    }
    return false;
}

bool Tabella::isLinked() {
    for (auto item: _colonne) {
        if (item->isKey()) {
            return item->_colonna_figlio!= nullptr;
        }
    } return false;
}



