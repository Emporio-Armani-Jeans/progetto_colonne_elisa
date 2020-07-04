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


string Tabella::getNome() const {
    return _nome_tabella;
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

bool Tabella::isLinked() {
    for (auto item: _colonne) {
        if (item->isKey()) { //la colonna può essere una "madre" solo se è chiave primaria
            return item->_colonna_figlio!= nullptr;
        }
    } return false;
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
    if(!flag_another_pk_found){ //se non è ancora stata aggiunta una colonna marcata come primary key, posso impostarla
        for(int j = 0; j < _colonne.size() && !flag_colonna_trovata; j++){
            if(_colonne[j]->getNomeColonna() == nomecolonna){ //match nome colonna con colonne della tabella
                _colonne[j]->_primary_key = true;
                flag_colonna_trovata = true;
            }
        }if(!flag_colonna_trovata)
            throw CampoNonTrovato();
    }
    else{
        throw PrimaryKeyAlreadyExisting();
    }
}

void Tabella::setChiaveEsterna(Tabella* tabella_to_link, const string& colonna_this, const string& chiave_esterna) {
    int pos_colonna_madre = INT_MAX, pos_colonna_figlia = INT_MAX;
    for (int i = 0; i < tabella_to_link->_colonne.size(); i++){ //verifico l'esistenza della colonna esterna
        if (tabella_to_link->_colonne[i]->_primary_key && tabella_to_link->_colonne[i]->getNomeColonna() == chiave_esterna)
            pos_colonna_madre = i;
    }
    if (pos_colonna_madre < tabella_to_link->_colonne.size()){
        for (int j = 0; j < _colonne.size(); j++){ // esistenza colonna interna
            if (_colonne[j]->getNomeColonna() == colonna_this) {
                pos_colonna_figlia = j;
            }
        }
        if(_colonne[pos_colonna_figlia]->_foreign_key== nullptr) { //la colonna interna non deve presentare altri collegamenti
            if (pos_colonna_figlia < _colonne.size()) {  //condizione soddisfatta se la colonna è stata trovata
                if (_colonne[pos_colonna_figlia]->getTipo() == tabella_to_link->_colonne[pos_colonna_madre]->getTipo()){
                    _colonne[pos_colonna_figlia]->_foreign_key = tabella_to_link->_colonne[pos_colonna_madre]; //imposto il vincolo per la colonna interna
                    tabella_to_link->_colonne[pos_colonna_madre]->_colonna_figlio = _colonne[pos_colonna_figlia]; //imposto vincolo per la colonna esterna
                    _colonne[pos_colonna_figlia]->_tab_madre=tabella_to_link->getNome(); //imposto il nome della tabella "madre"
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


void Tabella::addRecord(const vector<string>& campi, const vector<string>& valori) {
    bool flag_campo_non_trovato = false, flag_colonna_trovata = false;
    for(int i = 0; i < _colonne.size() && !flag_campo_non_trovato; i++){
        flag_colonna_trovata = false;
        if(_colonne[i]->_not_null){ //se la colonna è marcata come not null, all'interno dei campi da modificare deve esserci il suo nome
            for(int j=0; j<campi.size() && !flag_colonna_trovata; j++){
                if(_colonne[i]->getNomeColonna() == campi[j]) {
                    flag_colonna_trovata = true;
                }
            }
            if(!flag_colonna_trovata) //se alla fine di un ciclo non l'ho ancora trovata
                flag_campo_non_trovato = true;
        }
    }
    if(!flag_campo_non_trovato) { //se tutte le colonne marcate come not null sono presenti nei campi, posso procedere con l'aggiunta del record
        _recs++;
        for (auto &i : _colonne) { //aggiunta del valore di default per mantenere l'indicizzazione corretta e gestione auto_increment
            i->addDefault(0);
        }
        for (int i = 0; i < campi.size(); i++) {
            for (auto &j : _colonne) {
                if (campi[i] == j->getNomeColonna()) //match nome della colonna e campo da inserire
                {
                    if (j->isAutoIncrement())
                        throw TentativoInserimentoAutoIncrement();
                    else
                        j->updateVal(valori[i], int(_recs - 1)); //modifica del valore di default con il valore da inserire
                }

            }
        }
    }else{
        throw NotNullError(); //campo che doveva essere obbligatorio non inserito
    }
}

void Tabella::addRecordMemory(const vector<string> &campi, const vector<string> &valori) {
    int a;
    bool flag_campo_non_trovato = false, flag_colonna_trovata = false;
    _recs++;
    for(a=0; a < campi.size(); a++){
        for (auto &i : _colonne) {
            if (campi[a] == i->getNomeColonna()) {
                if (i->isAutoIncrement())
                    i->addDefault(stoi(valori[a])); //gestione dell'auto_increment alternativa per risalvare i dati in memoria tra una sessione e l'altra
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
        while (i < _colonne.size() && !campo_condizione_trovato) { //match campo condizione - colonna
            if (campo_condizione == _colonne[i]->getNomeColonna())
                campo_condizione_trovato = true;
            else i++;
        } //alla fine del ciclo la colonna che soddisfa il match è in _colonne[i]
        if (campo_condizione_trovato) {
            _colonne[i]->controlloFormato(condizione); //prima di eliminare verifico che il formato della condizione sia giusto
            while (j < _recs) {
                if (_colonne[i]->compareElements(condizione, operatore, j)) { //condizione soddisfatta
                    condizione_trovata = true;
                    if(!erroreSecKey(j)) { //procedo all'eliminazione solo se non ci sono già valori usati in colonne "figlie"
                        _recs--;
                        for (auto &elem : _colonne) {
                            elem->deleteVal(j);
                        }
                    } else j++;
                } else j++;
            }
            if(!condizione_trovata)
                throw ValueNotFound();
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
                } else j++; //se cancello un record tutti gli indici sono scalati indietro, quindi non incremento j
            }else j++;
        }
        if(!trovata2)
            throw ValueNotFound();
    }else{
        throw InvalidCondition();
    }
}


void Tabella::updateRecord(const string& campo_condizione,const string& condizione, const vector<string>& campi, const vector<string>& valori, int operatore){
    bool trovata=false, trovata2 = false;
    int i=0, j;
    while(i<_colonne.size() && !trovata){
        if(campo_condizione == _colonne[i]->getNomeColonna()) //match campo condizione - colonna
            trovata=true;
        else i++;
    }
    if(trovata){ // colonna trovata in _colonne[i]
        for(j=0; j<_recs; j++){
            if(_colonne[i]->compareElements(condizione, operatore, j)) { //soddisfazione condizione
                trovata2 = true;
                        for (int y = 0; y < campi.size(); y++) {
                            for (auto &g : _colonne) {
                                if (campi[y] == g->getNomeColonna()) { //match campo da modificare - colonna
                                    if(!erroreSecKey(j)) { //solo se i valori non sono usati in colonne "figlie"
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
                                if(!erroreSecKey(j)) {
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
    }else{
        throw InvalidCondition();
    }
}


vector<string> Tabella::returnData(const vector<string>& campi, const string& campo_ordinamento, int operatore_ordinamento) const {
    vector<string> righe_testo;
    string riga;
    if(_recs!=0) { //se sono presenti records
        bool trovata = false;
        bool valido = true;
        for(int i=0; i< campi.size(); i++ && valido){ //verifico che esistano tutte le colonne presenti nei <campi>
            trovata = false;
            for (int j=0; j < _colonne.size(); j++ && !trovata) {
                if (campi[i] == _colonne[j]->getNomeColonna()) {
                    trovata = true;
                }
            }
            if (!trovata)
                valido = false;
        }
        if (valido) {//se tutti i campi esistono valido = true
            for (const string & element : campi){ //aggiungo i nomi dei campi da stampare
                riga += element + " ";
            }
            righe_testo.push_back(riga);
            for (int i = 0; i < _recs; i++) { //tutti i record
                riga.clear();
                for (const auto &s : campi) {
                    for (auto j : _colonne) {
                        if (j->getNomeColonna() == s) {
                            if (operatore_ordinamento == 0) { //nessuna richiesta di ordinamento
                                if (j->getElement(i) == j->getElement(-1)) { //se è presente il valore di default
                                    riga += "___ ";
                                } else {
                                    int q = 0;
                                    string temp = j->getElement(i);
                                    while (q < temp.size()) { //gestione virgolette nei campi di testo
                                        if (temp[q] == '"' && temp[q + 1] == '"')
                                            temp.erase(q, 1);
                                        else
                                            q++;
                                    }
                                    riga += temp;
                                    riga += " ";
                                }
                            } else { //richiesta ordinamento
                                vector<int> indici_ordinati = ordinamento(campo_ordinamento, operatore_ordinamento);
                                if (j->getElement(indici_ordinati[i]) == j->getElement(-1)) { //default
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
        }
        else throw CampoNonTrovato();
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
        bool valido = true;
        for(int i=0; i< campi.size(); i++ && valido){ //ciclo per verificare l'effettiva esistenza dei campi selezionati
            trovata = false;
            for (int j=0; j < _colonne.size(); j++ && !trovata) {
                if (campi[i] == _colonne[j]->getNomeColonna()) {
                    trovata = true;
                }
            }
            if (!trovata) valido = false;
        }
        if (valido) { //se tutti i campi esistono valido = true
            trovata = false;
            int a = 0;
            while (a < _colonne.size() && !trovata) { //match campo comdizione - colonna
                if (campo_condizione == _colonne[a]->getNomeColonna())
                    trovata = true;
                else a++;
            }
            if (trovata) {
                _colonne[a]->controlloFormato(condizione);
                for (const string &b : campi) { //aggiungo i campi da stampare
                    riga += b + " ";
                }
                righe_testo.push_back(riga);
                for (int i = 0; i < _recs; i++) {
                    if (operatore_ordinamento == 0) { //nessuna richiesta di ordinamento
                        if (_colonne[a]->compareElements(condizione, operatore, i)) {
                            riga.clear();
                            for (const auto &s : campi) {
                                for (auto j : _colonne) {
                                    if (j->getNomeColonna() == s) {
                                        if (j->getElement(i) == j->getElement(-1)) {
                                            riga += "___ ";
                                        } else {
                                            int q = 0;
                                            string temp = j->getElement(i);
                                            while (q < temp.size()) {//gestione virgolette nei campi di testo
                                                if (temp[q] == '"' && temp[q + 1] == '"')
                                                    temp.erase(q, 1);
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
                    } else { //ordinamento
                        vector<int> indici_ordinati = ordinamento(campo_ordinamento, operatore_ordinamento);
                        if (_colonne[a]->compareElements(condizione, operatore, indici_ordinati[i])) {
                            riga.clear();
                            for (const auto &s : campi) {
                                for (auto j : _colonne) {
                                    if (j->getNomeColonna() == s) {
                                        if (j->getElement(indici_ordinati[i]) == j->getElement(-1)) {//se è presente il valore di default
                                            riga += "___ ";
                                        } else {
                                            int q = 0;
                                            string temp = j->getElement(indici_ordinati[i]);
                                            while (q < temp.size()) {//gestione virgolette nei campi di testo
                                                if (temp[q] == '"' && temp[q + 1] == '"')
                                                    temp.erase(q, 1);
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
        } else throw CampoNonTrovato();
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
        bool valido = true;
        for(int i=0; i< campi.size(); i++ && valido){
            trovata = false;
            for (int j=0; j < _colonne.size(); j++ && !trovata) {//ciclo per verificare l'effettiva esistenza dei campi selezionati
                if (campi[i] == _colonne[j]->getNomeColonna()) {
                    trovata = true;
                }
            }
            if (!trovata) valido = false;
        }
        if (valido) {//se tutti i campi esistono valido = true
            trovata = false;
            int a = 0;
            while (a < _colonne.size() && !trovata) {
                if (campo_condizione == _colonne[a]->getNomeColonna())
                    trovata = true;
                else a++;
            }
            if (trovata) {
                _colonne[a]->controlloFormato(condizione1);
                _colonne[a]->controlloFormato(condizione2);
                for (const string &z : campi) {//aggiungo i nomi dei campi da stampare
                    riga += z + " ";
                }
                righe_testo.push_back(riga);
                for (int i = 0; i < _recs; i++) {
                    if (operatore_ordinamento == 0) {
                        if (_colonne[a]->compareElements(condizione1, 4, i) &&
                            _colonne[a]->compareElements(condizione2, 2, i)) {
                            riga.clear();
                            for (const auto &s : campi) {
                                for (auto j : _colonne) {
                                    if (j->getNomeColonna() == s) {
                                        if (j->getElement(i) == j->getElement(-1)) {//se è presente il valore di default
                                            riga += "___ ";
                                        } else {
                                            int q = 0;
                                            string temp = j->getElement(i);
                                            while (q < temp.size()) {//gestione virgolette nei campi di testo
                                                if (temp[q] == '"' && temp[q + 1] == '"')
                                                    temp.erase(q, 1);
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
                            for (const auto &s : campi) {
                                for (auto j : _colonne) {
                                    if (j->getNomeColonna() == s) {
                                        if (j->getElement(indici_ordinati[i]) == j->getElement(-1)) {//se è presente il valore di default
                                            riga += "___ ";
                                        } else {
                                            int q = 0;
                                            string temp = j->getElement(indici_ordinati[i]);
                                            while (q < temp.size()) {//gestione virgolette nei campi di testo
                                                if (temp[q] == '"' && temp[q + 1] == '"')
                                                    temp.erase(q, 1);
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
        } else throw CampoNonTrovato();
    }else{
        righe_testo.emplace_back("Tabella vuota");
    }
    return righe_testo;
}


vector<int> Tabella::ordinamento(const string &campo, int operatore) const {
    if(operatore==1 || operatore==3) { //operatori < oppure >
        vector<int> indici(_recs);
        int tmp, c;
        for (int i = 0; i < _recs; i++) { //vettore di indici ordinati
            indici[i] = i;
        }
        for (c = 0; c < _colonne.size(); c++) {
            if (_colonne[c]->getNomeColonna() == campo)  //match
                break;
        }
        if (c < _colonne.size()) {
            for (int i = 0; i < _recs-1; i++) { //ordinamento del vettore di indici in base al criterio scelto
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


bool Tabella::erroreSecKey(int indice) {
    for (auto item : _colonne) {
        if (item->isKey()) { //solo la chiave primaria può essere una "madre"
            if (item->_colonna_figlio != nullptr) { //presenza di collegamenti esterni
                bool already_used = false;
                for (int i = 0; i < (item->_colonna_figlio)->getSize(); i++) { //scorro la colonna figlia
                    if ((item->compareElements((item->_colonna_figlio)->getElement(i), 0, indice))) //controllo valori usati
                        already_used = true;
                }
                return already_used;
            }
        }
    }
    return false;
}





