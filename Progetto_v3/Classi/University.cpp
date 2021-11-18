//
// Created by lucam on 05/11/2021.
//
//#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "University.h"
#include "DbException.h"

///costruisce la struttura dell'intera università leggendo i file da linea di comando

//namespace fs = std::filesystem;
University::University() {
    //if(!fs::exists(fs::file_status(std::string("../Database"))))
    //fs::create_directory("../Database");
    try {
        readStudents();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try {
        readProfessor();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try{
        readClassroom();
    }catch (DbException &exc){
        std::cerr << exc.what() <<std::endl;
    }
    try {
        readStudyCourse();
    }catch (DbException &exc){
        std::cerr << exc.what() <<std::endl;
    }
    try {
        readCourse();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    }

///per ogni riga del file in input scinde le varie info delimitate da ";"
std::vector<std::string> splittedLine(const std::string &s, const char &delimiter) {


    std::vector<std::string> toReturn; //conterrà la riga splittata nelle sue informazioni necessarie e indicizzate
    std::istringstream line(s); // mi serve per poter manipolare le stringhe
    std::string token; //buffer di appoggio per salvare l'informazione appena ricavata


    //fin quando la riga non è finita prende l'intera riga(line) e salva in una stringa del vettore di stringhe(tokens) l'informazione fino al prossimo delimitatore
    while (std::getline(line, token, delimiter)) {toReturn.push_back(token);
    }

    return toReturn;
}

///leggo il database degli studenti
void University::readStudents() {
    std::ifstream fileIn("../Sources/db_studenti.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw DbException("file db_studenti.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> InteroStudente;    //accoglierà il vettore con la riga del file scissa
    char c;    //accoglierà la s della matricola presa dal file, a noi inutile
    int nMatr; //accoglierà il codice identificativo della matricola presa dal file, a noi utile


    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        InteroStudente = splittedLine(line, ';');

        std::stringstream ss(InteroStudente[0]); //manipolo la stringa della matricola
        ss >> c >> nMatr; //la "s" la scarto in "c", tengo il codice identificativo da mettere in un intero

        ///controllo se la matricola è già esistente; in quel caso lancio un'eccezione, altrimenti inserisco lo studente con tutti i suoi dati
        if (_students.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, InteroStudente[1], InteroStudente[2],InteroStudente[3])));//la chiave intera è la matricola; ad ogni chiave/matricola è associato uno studente
    }

    fileIn.close();
}

///identico alla readStudents(); si evita di commentare per non sporcare il codice
void University::readProfessor() {
    std::ifstream fileIn("../Sources/db_professori.txt");
    if (!fileIn.is_open()) {
        throw DbException("file db_professori.txt non esistente");
    }
    std::string line;
    std::vector<std::string> InteroProfessore;
    char c;
    int nMatr;
    while (std::getline(fileIn, line)) {
        InteroProfessore = splittedLine(line, ';');
        std::stringstream ss(InteroProfessore[0]);
        ss >> c >> nMatr;
        if (_professors.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _professors.insert(std::pair<int, Professor>(nMatr, Professor(nMatr, InteroProfessore[1], InteroProfessore[2], InteroProfessore[3])));
    }
    fileIn.close();
}

///lettura delle aule dal database
void University::readClassroom() {
    std::ifstream fileIn("../Sources/db_aule.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw DbException("file db_aule.txt non esistente");
    }
    std::string line;
    std::vector<std::string> InteraClasse;
    char c;
    int nCod = 0;
    while (std::getline(fileIn, line)) {
        InteraClasse = splittedLine(line, ';');
        std::stringstream ss(InteraClasse[0]);
        ss >> c >> nCod;
        if (_classroom.count(nCod))
            throw std::logic_error("due codici uguali");
        else {
            _classroom.insert(std::pair<int, Classroom>(nCod,Classroom(nCod, InteraClasse[1], InteraClasse[2], std::stoi(InteraClasse[3]), std::stoi(InteraClasse[4]))));

        }
    }
    fileIn.close();
}

///lettura dei corsi di studio dal database
void University::readStudyCourse() {
    char c;
    int i;
    std::ifstream fileIn("../Sources/db_corsi_studio.txt");
    if (!fileIn.is_open()) {

        throw DbException("file db_corsi_studio.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> InteroCorsoDiStudi;    //accoglierà il vettore con la riga del file scissa
    std::vector<std::string> corsiSpenti;

    while (std::getline(fileIn, line)) {
        ///codice, livello

        if (line.empty())//non dovrebbe esserci. Sistemare
            break;

        InteroCorsoDiStudi = splittedLine(line, ';');//inserisco i vari campi delimitati dal ;
        std::stringstream ss(InteroCorsoDiStudi[0]);//manipolo la stringa
        int codCorso;
        ss >> c >> codCorso;//in c andrà il carattere A del codice letto. codCorso conterrà i 3 numeri subito dopo A
        std::string levelCourse = InteroCorsoDiStudi[1];//triennale o magistrale


        ///devo leggere i semestri
        std::vector<int> posSem;

        //cerco nella stringa se ci sono i due caratteri inseriti nella find_first_of
        std::size_t found = InteroCorsoDiStudi[2].find_first_of("{}");
        while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            posSem.push_back( found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = InteroCorsoDiStudi[2].find_first_of("{}", found + 1);//continuo a controllare la stringa
        }

        std::vector<std::string> semestri;//semestri va dentro il while perchè dovrà essere creato ogni volta che si ha un nuovo Corso di Studi
        for (i = 0; i < posSem.size() - 1; i = i + 2) {//metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
            int posStart = posSem[i] + 1, len = posSem[i + 1] - posSem[i] - 1;
            semestri.push_back(InteroCorsoDiStudi[2].substr(posStart,len));//salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato

        }

        ///leggo i semestri spenti.
        std::string corsiSpentiSenzaQuadre;
        corsiSpentiSenzaQuadre = InteroCorsoDiStudi[3].substr(1, InteroCorsoDiStudi[3].size() - 2);    //salvo la stringa senza le quadre
        corsiSpenti = splittedLine(corsiSpentiSenzaQuadre, ',');//splitto i corsi spenti senza le quadre


        ///creo StudyCourse
        bool isBachelor = false;
        if (levelCourse.compare("BS") == 0)
            isBachelor = true;

        StudyCourse SCourse(codCorso, isBachelor);
        //carico corsi e semestri letti nello studycourse

        int year = 0, numSemester = 1;
        for (i = 0; i < semestri.size(); i++) {
            year = 1 + i/2; //i=0 => y = 1, i=1 => y = 1, i=2 => y = 2; i=3 => y = 2
            numSemester = 1 + i%2; //i=0 => s = 1, i=1 => s = 2, i=2 => s = 1; i=3 => s = 2
            SCourse.addSemesterCourses(year, numSemester, semestri[i]);//passo: l'anno, primo o secondo semestre,tutta la stringa di corsi del semestre
        }


        SCourse.addOffCourses(corsiSpenti);
        _studyCourse.insert(std::pair<int, StudyCourse>(codCorso, SCourse));
    }

    fileIn.close();
}

///DA FARE
void University::readCourse() {
    std::ifstream fileIn("../Sources/db_corsi.txt");
    if (!fileIn.is_open()) {

        throw DbException("file db_corsi.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> InteroCorso;    //accoglierà il vettore con la riga del file scissa


    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        InteroCorso = splittedLine(line, ';');
        if(InteroCorso[0]=="c"){

            if (_courses.count(InteroCorso[1]))
                throw std::logic_error("due codici uguali");
            else {
                _courses.insert(std::pair<std::string, Course>(InteroCorso[1], Course(InteroCorso[1], InteroCorso[2], stoi(InteroCorso[3]),stoi(InteroCorso[4]),stoi(InteroCorso[5]))));//la chiave è la matricola; ad ogni chiave/matricola è associato un corso

                ///aggiornamento study course
                for(auto iterStudyCourse=_studyCourse.begin(); iterStudyCourse != _studyCourse.end(); iterStudyCourse++){//per tutti i corsi di studio
                           iterStudyCourse->second.updateInfoSemester(InteroCorso); //per tutti i corsi di studio punto al value che sarebbe un oggetto StudyCourse e passo alla funzione il rigo letto splittato. rigo che andrà in una seconda funzione(vedi StudyCourse.cpp)
                }
            }
        } else if (InteroCorso[0]=="a"){
            //da implementare
        }



    }

    fileIn.close();
}

///inserisco un nuovo studente
bool University::insertStuds(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in); //apro il file in lettura
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuovi studenti");
        return false;
    }
    std::string line; //stringa di appoggio per l'intera riga del file
    std::vector<std::string> tokens; //vettore di stringhe che accoglierà il ritorno della funzione splittedLine
    while (std::getline(fIn, line)) {//fino alla fine del file leggo un rigo alla volta
        tokens = splittedLine(line, ';');
        int matr = getNewStudentId(); //calcolo la matricola del nuovo studente
        _students.insert(std::pair<int, Student>(matr, Student(matr, tokens[0], tokens[1],tokens[2]))); //inserisco il nuovo studente nella mappatura interna
    }
    fIn.close();

    return true;
}

///identico alla insertStudents(); si evita di commentare per non sporcare il codice
bool University::insertProfessors(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuovi professori");
        return false;
    }
    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line, ';');
        int matr = getNewProfessorId();
        _professors.insert(std::pair<int, Professor>(matr, Professor(matr, tokens[0], tokens[1], tokens[2])));
    }
    fIn.close();
    return true;
}

///inserisci nuove aule
bool University::insertClassrooms(const std::string &fileIn) {
    std::fstream fIn(fileIn,std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuove aule");
        return false;
    }
    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line, ';');
        int id = getNewClassroomId();
        _classroom.insert(std::pair<int, Classroom>(id, Classroom(id, tokens[0], tokens[1], std::stoi(tokens[2]), std::stoi(tokens[3]))));
    }
    fIn.close();
    return true;
}

///inserimento dei nuovi corsi di studio
bool University::insertStudyCourses(const std::string &fin) {

    int i;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file inserimento nuovi corsi di studio");
        return  false;
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;    //accoglierà il vettore con la riga del file scissa


    bool toContinue = true;
    while (std::getline(fileIn, line) && toContinue) {
        ///codice, livello

        if (line.empty())//non dovrebbe esserci. Sistemare
            toContinue = false;

        else {
            tokens = splittedLine(line, ';');//inserisco i vari campi delimitati dal ;
            int codCorso = getNewStudyCourseId();

            std::string levelCourse = tokens[0];//triennale o magistrale

            ///devo leggere i semestri
            std::vector<int> posSem;

            //cerco nella stringa se ci sono i due caratteri inseriti nella find_first_of
            std::size_t found = tokens[1].find_first_of("{}");
            while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
                posSem.push_back( found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
                found = tokens[1].find_first_of("{}", found + 1);//continuo a controllare la stringa
            }

            std::vector<std::string> semestri;
            for (i = 0; i < posSem.size() - 1; i = i + 2) {//metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
                int posStart = posSem[i] + 1;
                int len = posSem[i + 1] - posSem[i] - 1; //pos(}) - pos({) -1
                semestri.push_back(tokens[1].substr(posStart, len));//salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato
            }


            ///creo StudyCourse
            bool isBachelor = false;
            if (levelCourse.compare("BS") == 0)
                isBachelor = true;
            StudyCourse SCourse(codCorso, isBachelor);
            //carico corsi e semestri letti nello studycourse
            int year = 1;
            int numSemester = 1;
            for (i = 0; i < semestri.size(); i++) {
                year = 1 + i/2; //i=0 => y = 1, i=1 => y = 1, i=2 => y = 2; i=3 => y = 2
                numSemester = 1 + i%2; //i=0 => s = 1, i=1 => s = 2, i=2 => s = 1; i=3 => s = 2
                SCourse.addSemesterCourses(year, numSemester, semestri[i]);//passo: l'anno, primo o secondo semestre,tutta la stringa di corsi del semestre
            }
            _studyCourse.insert(std::pair<int, StudyCourse>(codCorso, SCourse));
        }
    }

    fileIn.close();
}

///cerco la nuova matricola da associare al nuovo studente
const int University::getNewStudentId() const {

    if (_students.empty()) //per non usare la matricola s00000
        return 1;

    auto last = _students.rbegin();  //iteratore che punta all'ultimo studente della mappa
    int toReturn = last->second.getId() +  1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

///identico alla getNewStudentsId(); si evita di commentare per non sporcare il codice
const int University::getNewProfessorId() const {
    //dobbiamo partire da 1 e non da 0 quindi controllo se è vuoto
    if (_professors.empty())
        return 1;

    auto last = _professors.rbegin();//creo un iteratore all'ultimo elemento della mappa.
    int toReturn = last->second.getId() + 1; //leggo l'id corrispondente al professore(second) puntato dall'iteratore last e sommo 1. Sarà la nuova matricola
    return toReturn;
}

///prende il nuovo codice da associare alla nuova aula
const int University::getNewClassroomId() const {
    if (_classroom.empty())
        return 1;
    auto last = _classroom.rbegin();  //
    int toReturn = last->second.getId() +  1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

///cerca il nuovo codice da associare al nuovo corso
const int University::getNewStudyCourseId() const {
    if (_studyCourse.empty())
        return 1;

    auto last = _studyCourse.rbegin();  //iteratore che punta all'ultimo studente della mappa
    int toReturn = last->first + 1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

///DA FARE
const int University::getNewCourseId() const {

}

///aggiorno gli studenti
enum {update_name=1,update_surName=2,update_eMail=3};
bool University::updateStuds(const std::string & fin) {
    int i;
    char c;
    int nMatr=0;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file per aggiornamento studenti");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        tokens = splittedLine(line, ';');
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;

        ///se la matricola non esiste nel database
        if(_students.find(nMatr)==_students.end())//find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            throw  std::invalid_argument("matricola non presente");

        auto iter = _students.find(nMatr);//prendo la posizione della matricola

        tokens = splittedLine(line, ';');
        for(i=1; i < tokens.size(); i++ ){//analizzo i campi della riga del file passato come parametro che andranno aggiornati

            if(!(tokens[i].empty())){//se la stringa raccolta da tokens è vuota l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati
                switch (i) {
                    case update_name ://analizzo il nome
                        if (!(iter->second.getName() == tokens[i])) {//se il nome letto dal file è diverso dal nome del database
                            iter->second.updateName(tokens[i]); //cambio il nome
                        }
                        break;

                    case update_surName ://analizzo il cognome
                        if (!(iter->second.getSurname() == tokens[i])){//se il cognome letto dal file è diverso dal cognome del database
                            iter->second.updateSurnName(tokens[i]); //cambio il cognome
                        }
                        break;
                    case update_eMail : //analizzo l'email
                        if (!(iter->second.getEmail() == tokens[i])){//se l'email letta dal file è diversa dall'email del database
                            iter->second.updateEmail(tokens[i]); //cambia l'email
                        }
                        break;

                }

            }
        }
    }

    fileIn.close();
    return false;
}

///aggiorno i professori
bool University::updateProfessors(const std::string & fin) {
    int i;
    char c;
    int nMatr=0;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file per aggiornamento professori");

    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        tokens = splittedLine(line, ';'); //splitto il rigo nei vari campi di interesse
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;

        if(_professors.find(nMatr)==_professors.end())//find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            throw  std::invalid_argument("matricola non presente");

        auto iter = _professors.find(nMatr);//prendo la posizione della matricola

        tokens = splittedLine(line, ';');
        for(i=1; i < tokens.size(); i++ ){//analizzo i campi della riga del file passato come parametro che andranno aggiornati

            if(!(tokens[i].empty())){//se la stringa raccolta da tokens è vuota l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati

                switch (i) {
                    case update_name ://analizzo il nome
                        if (!(iter->second.getName() == tokens[i])) {//se il nome letto dal file è diverso dal nome del database
                            iter->second.updateName(tokens[i]); //cambia il nome
                        }
                        break;

                    case update_surName : //analizzo il cognome
                        if (!(iter->second.getSurname() == tokens[i])){//se il cognome letto dal file è diverso dal cognome del database
                            iter->second.updateSurnName(tokens[i]); //cambia il cognome
                        }
                        break;
                    case update_eMail ://analizzo l'email
                        if (!(iter->second.getEmail() == tokens[i])){//se l'email letta dal file è diversa dall'email letta dal database
                            iter->second.updateEmail(tokens[i]); //cambia l'email
                        }
                        break;

                }

            }
        }
    }

    fileIn.close();
    return true;
}

///aggiornamento aule
enum{update_lab=1,update_nameClassroom = 2, update_nSeats = 3, update_nSeatsExam = 4};
bool University::updateClassroom(const std::string &fin) {
    int i;
    char c;
    int nMatr=0;

    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw std::invalid_argument("errore apertura file per aggiornamento aule");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        tokens = splittedLine(line, ';');
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;

        if(_classroom.find(nMatr)==_classroom.end())//se non trovo il codice
            throw  std::invalid_argument("matricola non presente");

        auto iter = _classroom.find(nMatr);//prendo la posizione della matricola

        tokens = splittedLine(line, ';');
        for(i=1; i < tokens.size(); i++ ){//cerco i campi della riga del file passato che andranno aggiornati

            if(!(tokens[i].empty())){//se la stringa raccolta da tokens è vuota vuol dire che l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati

                switch (i) {

                    case update_lab : {//analizzo il tipo di classroom: aula o lab
                        bool lab;
                        if (tokens[i] == "L") //se il carattere intercettato è L allora l'aula da aggiungee sarà un lab
                            lab = true;
                        else//altrimenti sarà un'aula
                            lab = false;

                        if (iter->second.getLab() != lab) {//se è cambiata da aula a lab o viceversa
                            iter->second.updateType(lab); //cambia il tipo: lab o aula
                        }
                        break;
                    }
                    case update_nameClassroom : {//analizzo il nome della classroom
                        if (!(iter->second.getName() == tokens[i])) {//se il nome letto dal file è diverso dal nome nel database
                            iter->second.updateName(tokens[i]); //cambia il nome
                        }
                        break;
                    }

                    case update_nSeats : {//analizzo la capienza
                        if (iter->second.getNSeats() != stoi(tokens[i])) {  //se la capienza dell'aula letta da file è diversa dalla capienza dell'aula del database
                            iter->second.updateNSeats(stoi(tokens[i])); //cambia la capienza dell'aula
                        }
                        break;
                    }
                    case update_nSeatsExam : {//analizzo la capienza dell'aula per gli esami
                        if (iter->second.getNExamSeats() != stoi(tokens[i])) {//se la capienza dell'aula pe gli esami letta da file non è uguale alla capienza dell'aula per gli esami del database
                            iter->second.updateNExamSeats(stoi(tokens[i])); //cambia l'email
                        }
                    }
                }

            }
        }
    }

    fileIn.close();
    return true;
}




























