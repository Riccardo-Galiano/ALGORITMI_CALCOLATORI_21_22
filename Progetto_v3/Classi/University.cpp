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
    //    readStudents();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try {
      //  readProfessor();
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


    std::vector<std::string> toReturn; //conterrà la riga splittata nelle sue informazioni necessarie e indicizzate con vector
    std::istringstream line(s); // mi serve per per poter manipolare le stringhe
    std::string token; //buffer di appoggio per salvare l'informazione appena ricavata


    //fin quando la riga non è finita prende l'intera riga e salva in un vettore di string l'informazione fino al prossimo delimitatore
    while (std::getline(line, token, delimiter)) {toReturn.push_back(token);
    }

    return toReturn;
}




/// come faccio a creare il file se non lo trovo?

void University::readStudents() {
    std::ifstream fileIn("../Sources/db_studenti.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw DbException("file db_studenti.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;    //accoglierà il vettore con la riga del file scissa
    char c;    //accoglierà la s della matricola presa dal file, a noi inutile
    int nMatr; //accoglierà il codice identificativo della matricola presa dal file, a noi utile


    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        tokens = splittedLine(line, ';');

        std::stringstream ss(tokens[0]); //manipolo la stringa della matricola
        ss >> c >> nMatr; //la "s" la scarto in "c", tengo il codice identificativo da mettere in un intero

        ///controllo se la matricola è già esistente; in quel caso lancio un'eccezione, altrimenti inserisco lo studente con tutti i suoi dati
        if (_students.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, tokens[1], tokens[2],tokens[3])));//la chiave intera è la matricola; ad ogni chiave/matricola è associato uno studente
    }

    fileIn.close();
}

///inserisco un nuovo studente
bool University::insertStuds(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in); //apro il file
    if (!fIn.is_open()) { //controllo se il file è aperto
        std::cerr << "rotto\n";
        return false;
    }
    std::string line; //stringa di appoggio
    std::vector<std::string> tokens; //vettore di stringhe che accoglierà il ritorno della funzione split
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line, ';');
        int matr = getNewStudentId(); //calcolo la matricola del nuovo studente
        _students.insert(std::pair<int, Student>(matr, Student(matr, tokens[0], tokens[1],tokens[2]))); //inserisco il nuovo studente nella mappatura interna
    }
    fIn.close();

    return true;
}

///capisce qual è la nuova matricola da associare al nuovo studente
const int University::getNewStudentId() const {

    if (_students.empty())
        return 1;

    auto last = _students.rbegin();  //iteratore che punta all'ultimo studente della mappa
    int toReturn = last->second.getId() +  1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

///identico alla readStudents(); si evita di commentare per non sporcare il codice
void University::readProfessor() {
    std::ifstream fileIn("../Sources/db_professori.txt");
    if (!fileIn.is_open()) {
        throw DbException("file db_professori.txt non esistente");
    }
    std::string line;
    std::vector<std::string> tokens;
    char c;
    int nMatr;
    while (std::getline(fileIn, line)) {
        tokens = splittedLine(line, ';');
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;
        if (_professors.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _professors.insert(std::pair<int, Professor>(nMatr, Professor(nMatr, tokens[1], tokens[2], tokens[3])));
    }
    fileIn.close();
}

///identico alla insertStudents(); si evita di commentare per non sporcare il codice
bool University::insertProfessors(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        std::cerr << "rotto\n";
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

///identico alla getNewStudentsId(); si evita di commentare per non sporcare il codice
const int University::getNewProfessorId() const {
    //dobbiamo partire da 1 e non da 0 quindi controllo se è vuoto
    if (_professors.empty())
        return 1;

    auto last = _professors.rbegin();//creo un iteratore all'ultimo elemento della mappa.
    int toReturn = last->second.getId() +
                   1; //leggo l'id corrispondente al professore(second) puntanto dall'iteratore last e sommo 1. Sarà la nuova matricola
    return toReturn;
}

void University::readClassroom() {
    std::ifstream fileIn("../Sources/db_aule.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw DbException("file db_aule.txt non esistente");
    }
    std::string line;
    std::vector<std::string> tokens;
    char c;
    int nCod = 0;
    while (std::getline(fileIn, line)) {
        tokens = splittedLine(line, ';');
        std::stringstream ss(tokens[0]);
        ss >> c >> nCod;
        if (_classroom.count(nCod))
            throw std::logic_error("due codici uguali");
        else {
            _classroom.insert(std::pair<int, Classroom>(nCod,
                                                        Classroom(nCod, tokens[1], tokens[2], std::stoi(tokens[3]),
                                                                  std::stoi(tokens[4]))));

        }
    }
    fileIn.close();
}


bool University::insertClassrooms(const std::string &fileIn) {
    std::fstream fIn(fileIn,std::ios::in); ///piccolezza: abbiamo aperto gli altri file con ifstream, lasciamo fstream per far vedere che conosciamo altro o uniformiamo il codice?
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


/// potrebbe non esserci niente ad un certo i
/// il codice dell'aula può essere riassegnato?
const int University::getNewClassroomId() const {
    if (_classroom.empty())
        return 1;
    auto last = _classroom.rbegin();  //
    int toReturn = last->second.getId() +  1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}


void University::readStudyCourse() {
    char c;
    int i;
    std::ifstream fileIn("../Sources/db_corsi_studio.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw DbException("file db_corsi_studio.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;    //accoglierà il vettore con la riga del file scissa
    std::vector<std::string> corsiSpenti;

    while (std::getline(fileIn, line)) {
        ///codice, livello

        if (line.empty())//non dovrebbe esserci. Sistemare
            break;

        tokens = splittedLine(line, ';');//inserisco i vari campi delimitati dal ;
        std::stringstream ss(tokens[0]);//manipolo la stringa
        int codCorso;
        ss >> c >> codCorso;//in c andrà il carattere A del codice letto. codCorso conterrà i 3 numeri subito dopo A
        std::string levelCourse = tokens[1];//triennale o magistrale


        ///devo leggere i semestri
        std::vector<int> posSem;

        //cerco nella stringa se ci sono i due caratteri inseriti nella find_first_of
        std::size_t found = tokens[2].find_first_of("{}");
        while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            posSem.push_back( found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = tokens[2].find_first_of("{}", found + 1);//continuo a controllare la stringa
        }

        std::vector<std::string> semestri;//semestri va dentro il while perchè dovrà essere creato ogni volta che si ha un nuovo Corso di Studi
        for (i = 0; i < posSem.size() - 1; i = i + 2) {//metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
            int posStart = posSem[i] + 1, len = posSem[i + 1] - posSem[i] - 1;
            semestri.push_back(tokens[2].substr(posStart,len));//salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato

        }

        ///leggo i semestri spenti.
        std::string corsiSpentiSenzaQuadre;
        corsiSpentiSenzaQuadre = tokens[3].substr(1, tokens[3].size() - 2);    //salvo la stringa senza le quadre
        corsiSpenti = splittedLine(corsiSpentiSenzaQuadre, ',');//splitto i corsi spenti senza le quadre

        //versione compatta senza creare stringa corsiSpentiSenzaQuadre, meno chiaro ma più compatto
        //corsiSpenti = splittedLine(tokens[3].substr(1,tokens[3].size()-2), ',');

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

bool University::insertStudyCourses(const std::string &fin) {

    int i;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw std::invalid_argument("errore apertura file inserimento nuovi corsi di studio");
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


const int University::getNewStudyCourseId() const {
    if (_studyCourse.empty())
        return 1;

    auto last = _studyCourse.rbegin();  //iteratore che punta all'ultimo studente della mappa
    int toReturn = last->first + 1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

const int University::getNewCourseId() const {
}

void University::readCourse() {
    std::ifstream fileIn("../Sources/db_corsi.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw DbException("file db_corsi.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;    //accoglierà il vettore con la riga del file scissa


    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        tokens = splittedLine(line, ';');
        if(tokens[0]=="c"){

            if (_courses.count(tokens[1]))
                throw std::logic_error("due matricole uguali");
            else
                _courses.insert(std::pair<std::string, Course>(tokens[1],Course(tokens[1],tokens[2],stoi(tokens[3]),stoi(tokens[4]),stoi(tokens[5]))));//la chiave è la matricola; ad ogni chiave/matricola è associato un corso
             //devo modificare i parametri dei course in studycourse/_semester
        } else if (tokens[0]=="a"){
            //da implementare
        }



         }

    fileIn.close();
}


