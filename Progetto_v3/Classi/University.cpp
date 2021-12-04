//
// Created by lucam on 05/11/2021.
//
//#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "University.h"
#include "DbException.h"
#include "Parse.hpp"

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
    try {
        readClassroom();
    } catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try {
        readStudyCourse();
    } catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try {
        readCourse();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try {
        readSessionAcYear();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try {
        readProfsAvailability();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
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
        InteroStudente = Parse::splittedLine(line, ';');

        std::stringstream ss(
                InteroStudente[0]); //manipolo la stringa della matricola (iniziallizzo oggetto ss, di classe stringstream, con InteroStudente[0] ovvero matricola)
        ss >> c >> nMatr; //la "s" la scarto in "c", tengo il codice identificativo da mettere in un intero

        ///controllo se la matricola è già esistente; in quel caso lancio un'eccezione, altrimenti inserisco lo studente con tutti i suoi dati
        if (_students.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, InteroStudente[1], InteroStudente[2],
                                                                    InteroStudente[3])));//la chiave intera è la matricola; ad ogni chiave/matricola è associato uno studente
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
        InteroProfessore = Parse::splittedLine(line, ';');
        std::stringstream ss(InteroProfessore[0]);
        ss >> c >> nMatr;
        if (_professors.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _professors.insert(std::pair<int, Professor>(nMatr,
                                                         Professor(nMatr, InteroProfessore[1], InteroProfessore[2],
                                                                   InteroProfessore[3])));
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
        InteraClasse = Parse::splittedLine(line, ';');
        std::stringstream ss(InteraClasse[0]);
        ss >> c >> nCod;
        if (_classroom.count(nCod))
            throw std::logic_error("due codici uguali");
        else {
            _classroom.insert(std::pair<int, Classroom>(nCod, Classroom(nCod, InteraClasse[1], InteraClasse[2],
                                                                        std::stoi(InteraClasse[3]),
                                                                        std::stoi(InteraClasse[4]))));

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

        InteroCorsoDiStudi = Parse::splittedLine(line, ';');//inserisco i vari campi delimitati dal ;
        std::stringstream ss(InteroCorsoDiStudi[0]);//manipolo la stringa
        int codCorso;
        ss >> c >> codCorso;//in c andrà il carattere A del codice letto. codCorso conterrà i 3 numeri subito dopo A
        std::string levelCourse = InteroCorsoDiStudi[1];//triennale o magistrale


        ///devo leggere i semestri
        std::vector<int> posSem = Parse::posCurlyBrackets(InteroCorsoDiStudi[2]);

        std::vector<std::string> semestri;//semestri va dentro il while perchè dovrà essere creato ogni volta che si ha un nuovo Corso di Studi
        for (i = 0; i < posSem.size() - 1; i = i +
                                               2) {//metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
            int posStart = posSem[i] + 1, len = posSem[i + 1] - posSem[i] - 1;
            semestri.push_back(InteroCorsoDiStudi[2].substr(posStart,
                                                            len));//salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato

        }

        ///leggo i semestri spenti.
        std::string corsiSpentiSenzaQuadre;

        if (InteroCorsoDiStudi.size() == 4) {//è possibile leggere i corsi spenti solo se questi, leggendo da database, sono presenti e quindi solo se il vettore InteroCorsoDiStudi presenta un quarto campo
            corsiSpentiSenzaQuadre = InteroCorsoDiStudi[3].substr(1, InteroCorsoDiStudi[3].size() -
                                                                     2);    //salvo la stringa senza le quadre
            corsiSpenti = Parse::splittedLine(corsiSpentiSenzaQuadre, ',');//splitto i corsi spenti senza le quadre
        }

        ///creo un oggetto StudyCourse
        bool isBachelor = false;
        if (levelCourse.compare("BS") == 0)//se corso di studi triennale
            isBachelor = true;

        StudyCourse SCourse(codCorso, isBachelor);
        //carico corsi e semestri letti nello studycourse

        int year = 0, numSemester = 1;
        for (i = 0; i < semestri.size(); i++) {
            year = 1 + i / 2; //i=0 => y = 1, i=1 => y = 1, i=2 => y = 2; i=3 => y = 2
            numSemester = 1 + i % 2; //i=0 => s = 1, i=1 => s = 2, i=2 => s = 1; i=3 => s = 2
            SCourse.addSemesterCourses(year, numSemester,
                                       semestri[i]);//passo: l'anno, primo o secondo semestre,tutta la stringa di corsi del semestre
        }

        if (InteroCorsoDiStudi.size() == 4) {//solo se presenti aggiungo i corsi spenti
            SCourse.addOffCourses(corsiSpenti);
        }

        _studyCourse.insert(std::pair<int, StudyCourse>(codCorso,
                                                        SCourse));//inserisco il corso alla mappa dei corsi di studio la cui chiave è il codice del corso privato di "A" iniziale
    }

    fileIn.close();
}

///lettura dei corsi dal database
void University::readCourse() {
    std::ifstream fileIn("../Sources/db_corsi.txt");
    if (!fileIn.is_open()) {

        throw DbException("file db_corsi.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> interoCorso;    //accoglierà il vettore con la riga del file scissa
    std::vector<std::string> specificYearCourse;
    std::string lastReadCourse; //ogni 'c' letto, viene aggiornato con l'ID del corso
    std::string acYear;
    std::string examData;
    std::string idGroupedCourse;
    bool isActive = false;
    int num_parallel_courses = 0;
    std::string profSenzaQuadre;
    std::vector<std::string> idGrouped;
    std::vector<std::string> splittedExamData;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        interoCorso = Parse::splittedLine(line, ';');

        ///leggo il corso con le info generali e info per anno accademico. Fin quando non verrà letta una nuova stringa che inizia con "c" andrò ad
        /// aggiornare solo gli anni accademici dell'ultimo corso letto(lastReadCourse) così, nel file passato, posso scrivere le info di più anni accademici
        /// per uno stesso corso consecutivamente
        if (interoCorso[0] == "c") {//stringa che inizia per c contiene le info generali di un corso
            lastReadCourse = interoCorso[1];//codice identificativo del corso appena letto

            //inserisco il corso con le relative info nella map _courses
            _courses.insert(std::pair<std::string, Course>(lastReadCourse,Course(interoCorso[1], interoCorso[2], stoi(interoCorso[3]),stoi(interoCorso[4]), stoi(interoCorso[5]),stoi(interoCorso[6]))));

        } else if (interoCorso[0] ==
                   "a") {//se la riga letta inizia con "a" devo inserire le info di un anno accademico per un corso generale letto
            specificYearCourse = interoCorso;//per una maggiore comprensione nella lettura del codice
            acYear = specificYearCourse[1]; //anno accademico
            if (specificYearCourse[2] == "attivo") //se attivo o meno
                isActive = true;
            else
                isActive = false;
            num_parallel_courses = stoi(specificYearCourse[3]);//numero di corsi in parallelo
            profSenzaQuadre = specificYearCourse[4].substr(1, specificYearCourse[4].size() -
                                                              2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
            std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre, num_parallel_courses);//divido i vari corsi in parallelo
            examData = specificYearCourse[5];//informazioni sull'esame
            examData = examData.substr(1, examData.size() - 2);//tolgo le { } che racchiudono le info degli esami
            splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
            idGroupedCourse = specificYearCourse[6];//id dei vari corsi in parallelo
            idGroupedCourse = idGroupedCourse.substr(1, idGroupedCourse.size() -
                                                        2);// tolgo le { } che racchiudono gli id
            idGrouped = Parse::splittedLine(idGroupedCourse, ',');//scissione degli id dei corsi raggruppati

            ///la addSpecificYearCourses serve per accedere e aggiornare una map (_courseOfTheYear) presente in ogni oggetto Course della mappa _courses, che contiene i vari anni accademici per ogni corso
            _courses.at(lastReadCourse).addSpecificYearCourses(acYear, isActive, num_parallel_courses, profCorsoPar,
                                                               splittedExamData,
                                                               idGrouped);//aggiungo ad un corso un anno accademico e le relative info nella map _courses
        }

    }

    fileIn.close();
}

///inserisco un nuovo studente (privo di matricola che bisogna quindi ricavare)
bool University::addStuds(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in); //apro il file in lettura
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuovi studenti");
        return false;
    }
    int line_counter=1;
    std::string line; //stringa di appoggio per l'intera riga del file
    std::vector<std::string> tokens; //vettore di stringhe che accoglierà il ritorno della funzione splittedLine
    while (std::getline(fIn, line)) {//fino alla fine del file leggo un rigo alla volta
        tokens = Parse::splittedLine(line, ';');
        if(tokens.size()!=3){
            throw DbException("errore formato file studenti alla riga: ",line_counter);
        }
        int matr = getNewStudentId(); //calcolo la matricola del nuovo studente
        _students.insert(std::pair<int, Student>(matr, Student(matr, tokens[0], tokens[1],tokens[2]))); //inserisco il nuovo studente nella mappatura interna
        line_counter++;
    }
    fIn.close();

    dbStudsWrite();
    std::cout<<"comando -a:s correttamente eseguito"<<std::endl;
    return true;
}

///identico alla insertStudents(); si evita di commentare per non sporcare il codice
bool University::addProfessors(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuovi professori");
        return false;
    }
    std::string line;
    int line_counter=1;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = Parse::splittedLine(line, ';');
        if(tokens.size()!=3){
            throw DbException("errore formato file professori alla riga: ",line_counter);
        }
        int matr = getNewProfessorId();
        _professors.insert(std::pair<int, Professor>(matr, Professor(matr, tokens[0], tokens[1], tokens[2])));
        line_counter++;
    }
    fIn.close();

    dbProfsWrite();
    std::cout<<"comando -a:d correttamente eseguito"<<std::endl;

    return true;
}

///inserisci nuove aule
bool University::addClassrooms(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuove aule");
        return false;
    }
    std::string line;
    int line_counter=1;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = Parse::splittedLine(line, ';');
        if(tokens.size()!=4){
            throw DbException("errore formato file aule alla riga: ",line_counter);
        }
        int id = getNewClassroomId();
        _classroom.insert(std::pair<int, Classroom>(id, Classroom(id, tokens[0], tokens[1], std::stoi(tokens[2]),std::stoi(tokens[3]))));
        line_counter++;
    }
    fIn.close();

    dbClassRoomWrite();
    std::cout<<"comando -a:a correttamente eseguito"<<std::endl;

    return true;
}

///inserimento dei nuovi corsi di studio
bool University::addStudyCourses(const std::string &fin) {

    int i;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file inserimento nuovi corsi di studio");
        return false;
    }
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter=1;
    std::vector<std::string> tokens;    //accoglierà il vettore con la riga del file scissa


    bool toContinue = true;
    while (std::getline(fileIn, line) && toContinue) {
        ///codice, livello

        if (line.empty())//non dovrebbe esserci. Sistemare
            toContinue = false;

        else {
            tokens = Parse::splittedLine(line, ';');//inserisco i vari campi delimitati dal ;
            if(tokens.size()!=2){
                throw DbException("errore formato file corsi di studio alla riga: ",line_counter);
            }
            int codCorso = getNewStudyCourseId();

            std::string levelCourse = tokens[0];//triennale o magistrale

            ///devo leggere i semestri
            std::vector<int> posSem;

            //cerco nella stringa se ci sono i due caratteri inseriti nella find_first_of
            std::size_t found = tokens[1].find_first_of("{}");
            while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
                posSem.push_back(found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
                found = tokens[1].find_first_of("{}", found + 1);//continuo a controllare la stringa
            } //alla fine di questo while posSem conterrà le posizioni in corrispondenza delle quali nel vettore tokens[1] sono presenti: { o }

            std::vector<std::string> semestri;
            for (i = 0; i < posSem.size() - 1; i = i + 2) { //metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
                int posStart = posSem[i] + 1;// tolgo la graffa
                int len = posSem[i + 1] - posSem[i] - 1; //pos(}) - pos({) -1
                semestri.push_back(tokens[1].substr(posStart,len));   //salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato
            } //alla fine di questo for il vector "semestre" conterrà i corsi di ogni semestre disposti al suo interno in modo che ogni "cella" di "semestre" contiene tutti i corsi di un certo semestre
            //semestre[0] = tutti i corsi di anno1_semestre1, semestre[1] = tutti i  corsi anno1_semestre2, semestre[2] = tutti i  corsi anno2_semestre1, ...

            if((levelCourse=="BS"&&semestri.size()!=6)||(levelCourse=="MS"&&semestri.size()!=4)){
                throw DbException("formato file corsi di studio non valido: ci sono semestri senza corsi o numero semestri incompatibile con tipo di laurea alla riga:",line_counter);
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
                year = 1 + i / 2; //i=0 => y = 1, i=1 => y = 1, i=2 => y = 2; i=3 => y = 2
                numSemester = 1 + i % 2; //i=0 => s = 1, i=1 => s = 2, i=2 => s = 1; i=3 => s = 2
                SCourse.addSemesterCourses(year, numSemester,
                                           semestri[i]);//passo: l'anno, primo o secondo semestre,tutta la stringa di corsi del semestre
            }
            _studyCourse.insert(std::pair<int, StudyCourse>(codCorso, SCourse));
            line_counter++;
        }
    }

    fileIn.close();

    dbStudyCourseWrite();
    std::cout<<"comando -a:f correttamente eseguito"<<std::endl;

    return true;
}

///inserisco dei nuovi corsi

///da fare controllo se corso non esiste ancora in base dati e sulla correttezza del file: tokens.size = 10
bool University::addCourses(const std::string &fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file inserimento nuovi corsi");
        return false;
    }
    std::string line;
    std::vector<std::string> specificYearCourse;
    std::string acYear;
    std::string examData;
    std::string idGroupedCourse;
    bool isActive = true;
    int num_parallel_courses = 0;
    std::string profSenzaQuadre;
    std::vector<std::string> idGrouped;
    std::vector<std::string> splittedExamData;

    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        specificYearCourse = Parse::splittedLine(line, ';');
        std::string newIdCourse = getNewCourseId();

        _courses.insert(std::pair<std::string, Course>(newIdCourse, Course(newIdCourse, specificYearCourse[1],
                                                                           stoi(specificYearCourse[2]),
                                                                           stoi(specificYearCourse[3]),
                                                                           stoi(specificYearCourse[4]),
                                                                           stoi(specificYearCourse[5]))));

        acYear = specificYearCourse[0]; //anno accademico


        num_parallel_courses = stoi(specificYearCourse[6]);//numero di corsi in parallelo
        profSenzaQuadre = specificYearCourse[7].substr(1, specificYearCourse[7].size() -
                                                          2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
        std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre, num_parallel_courses);//divido i vari corsi in parallelo
        examData = specificYearCourse[8];//informazioni sull'esame
        examData = examData.substr(1, examData.size() - 2);//tolgo le { } che racchiudono le info degli esami
        splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
        idGroupedCourse = specificYearCourse[9];//id dei vari corsi in parallelo
        idGroupedCourse = idGroupedCourse.substr(1,
                                                 idGroupedCourse.size() - 2);// tolgo le { } che racchiudono gli id
        idGrouped = Parse::splittedLine(idGroupedCourse, ',');//scissione degli id dei corsi raggruppati
        _courses.at(newIdCourse).addSpecificYearCourses(acYear, isActive, num_parallel_courses, profCorsoPar,
                                                        splittedExamData, idGrouped);
    }
    fileIn.close();

    dbCourseWrite();

    return true;
}

///cerco la nuova matricola da associare al nuovo studente
const int University::getNewStudentId() const {

    if (_students.empty()) //per non usare la matricola s00000
        return 1;

    auto last = _students.rbegin();  //iteratore che punta all'ultimo studente della mappa
    int toReturn = last->second.getId() +
                   1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

///identico alla getNewStudentsId(); si evita di commentare per non sporcare il codice
const int University::getNewProfessorId() const {
    //dobbiamo partire da 1 e non da 0 quindi controllo se è vuoto
    if (_professors.empty())
        return 1;

    auto last = _professors.rbegin();//creo un iteratore all'ultimo elemento della mappa.
    int toReturn = last->second.getId() +
                   1; //leggo l'id corrispondente al professore(second) puntato dall'iteratore last e sommo 1. Sarà la nuova matricola
    return toReturn;
}

///prende il nuovo codice da associare alla nuova aula
const int University::getNewClassroomId() const {
    if (_classroom.empty())
        return 1;
    auto last = _classroom.rbegin();  //
    int toReturn = last->second.getId() +
                   1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

///cerca il nuovo codice da associare al nuovo corso di studio
const int University::getNewStudyCourseId() const {
    if (_studyCourse.empty())
        return 1;

    auto last = _studyCourse.rbegin();  //iteratore che punta all'ultimo studente della mappa
    int toReturn =
            last->first + 1; //leggo l'Id dell'ultima aula della mappa e aggiungo 1. Nuovo id per la prossima aula
    return toReturn;
}

///cerca il nuovo codice da associare al nuovo corso
const std::string University::getNewCourseId() const {

    auto last = _courses.rbegin();  //iteratore che punta all'ultimo corso della mappa
    bool noZ = false;
    int num;
    std::string cod;

    if (_courses.empty())
        return "01AAAAA";

    std::stringstream Id(last->first);
    Id >> num >> cod;

    for (int i = cod.size() - 1; i >= 0 && !noZ; i--) {
        if (cod[i] != 'Z') {
            cod[i]++;
            noZ = true;
        } else {
            cod[i] = 'A';
        }
    }

    if (cod == "AAAAA")
        num++;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << num << cod;
    std::string newId = ss.str();

    return newId;
}

///aggiorno gli studenti
enum {
    update_name = 1, update_surName = 2, update_eMail = 3
};

bool University::updateStuds(const std::string &fin) {
    int i;
    char c;
    int nMatr = 0;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file per aggiornamento studenti");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> infoStud;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoStud = Parse::splittedLine(line, ';');
        std::stringstream ss(infoStud[0]);
        ss >> c >> nMatr;

        ///se la matricola non esiste nel database
        if (_students.find(nMatr) ==
            _students.end()) //find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            throw std::invalid_argument("matricola non presente");

        auto iter = _students.find(nMatr);//prendo la posizione della matricola


        for (i = 1; i <
                    infoStud.size(); i++) {  //analizzo i campi della riga del file passato come parametro che andranno aggiornati

            if (!(infoStud[i].empty())) {//se la stringa raccolta da tokens è vuota l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati
                switch (i) {
                    case update_name ://analizzo il nome
                        if (!(iter->second.getName() ==
                              infoStud[i])) { //se il nome letto dal file è diverso dal nome del database
                            iter->second.updateName(infoStud[i]); //cambio il nome
                        }
                        break;

                    case update_surName ://analizzo il cognome
                        if (!(iter->second.getSurname() ==
                              infoStud[i])) {//se il cognome letto dal file è diverso dal cognome del database
                            iter->second.updateSurnName(infoStud[i]); //cambio il cognome
                        }
                        break;
                    case update_eMail : //analizzo l'email
                        if (!(iter->second.getEmail() ==
                              infoStud[i])) {//se l'email letta dal file è diversa dall'email del database
                            iter->second.updateEmail(infoStud[i]); //cambia l'email
                        }
                        break;

                }

            }
        }
    }

    fileIn.close();

    dbStudsWrite();
    return false;
}

///aggiorno i professori
bool University::updateProfessors(const std::string &fin) {
    int i;
    char c;
    int nMatr = 0;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file per aggiornamento professori");

    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> infoProf;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoProf = Parse::splittedLine(line, ';'); //splitto il rigo nei vari campi di interesse
        std::stringstream ss(infoProf[0]);
        ss >> c >> nMatr;

        if (_professors.find(nMatr) ==
            _professors.end())//find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            throw std::invalid_argument("matricola non presente");

        auto iter = _professors.find(nMatr);//prendo la posizione della matricola


        for (i = 1; i <
                    infoProf.size(); i++) {//analizzo i campi della riga del file passato come parametro che andranno aggiornati

            if (!(infoProf[i].empty())) {//se la stringa raccolta da tokens è vuota l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati

                switch (i) {
                    case update_name ://analizzo il nome
                        if (!(iter->second.getName() ==
                              infoProf[i])) {//se il nome letto dal file è diverso dal nome del database
                            iter->second.updateName(infoProf[i]); //cambia il nome
                        }
                        break;

                    case update_surName : //analizzo il cognome
                        if (!(iter->second.getSurname() ==
                              infoProf[i])) {//se il cognome letto dal file è diverso dal cognome del database
                            iter->second.updateSurnName(infoProf[i]); //cambia il cognome
                        }
                        break;
                    case update_eMail ://analizzo l'email
                        if (!(iter->second.getEmail() ==
                              infoProf[i])) {//se l'email letta dal file è diversa dall'email letta dal database
                            iter->second.updateEmail(infoProf[i]); //cambia l'email
                        }
                        break;

                }

            }
        }
    }

    fileIn.close();

    dbProfsWrite();
    return true;
}

///aggiornamento aule
enum {
    update_lab = 1, update_nameClassroom = 2, update_nSeats = 3, update_nSeatsExam = 4
};

///aggiorno le Classroom
bool University::updateClassroom(const std::string &fin) {
    int i;
    char c;
    int nMatr = 0;

    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file per aggiornamento aule");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> infoClassroom;

    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoClassroom = Parse::splittedLine(line, ';');
        std::stringstream ss(infoClassroom[0]);
        ss >> c >> nMatr;

        if (_classroom.find(nMatr) == _classroom.end())//se non trovo il codice
            throw std::invalid_argument("matricola non presente");

        auto iter = _classroom.find(nMatr);//prendo la posizione della matricola

        for (i = 1; i < infoClassroom.size(); i++) {//cerco i campi della riga del file passato che andranno aggiornati

            if (!(infoClassroom[i].empty())) {//se la stringa raccolta da tokens è vuota vuol dire che l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati

                switch (i) {

                    case update_lab : {//analizzo il tipo di classroom: aula o lab
                        bool lab;
                        if (infoClassroom[i] ==
                            "L") //se il carattere intercettato è L allora l'aula da aggiungee sarà un lab
                            lab = true;
                        else//altrimenti sarà un'aula
                            lab = false;

                        if (iter->second.getLab() != lab) {//se è cambiata da aula a lab o viceversa
                            iter->second.updateType(lab); //cambia il tipo: lab o aula
                        }
                        break;
                    }
                    case update_nameClassroom : {//analizzo il nome della classroom
                        if (!(iter->second.getName() ==
                              infoClassroom[i])) {//se il nome letto dal file è diverso dal nome nel database
                            iter->second.updateName(infoClassroom[i]); //cambia il nome
                        }
                        break;
                    }

                    case update_nSeats : {//analizzo la capienza
                        if (iter->second.getNSeats() !=
                            stoi(infoClassroom[i])) {  //se la capienza dell'aula letta da file è diversa dalla capienza dell'aula del database
                            iter->second.updateNSeats(stoi(infoClassroom[i])); //cambia la capienza dell'aula
                        }
                        break;
                    }
                    case update_nSeatsExam : {//analizzo la capienza dell'aula per gli esami
                        if (iter->second.getNExamSeats() !=
                            stoi(infoClassroom[i])) {//se la capienza dell'aula pe gli esami letta da file non è uguale alla capienza dell'aula per gli esami del database
                            iter->second.updateNExamSeats(stoi(infoClassroom[i])); //cambia l'email
                        }
                    }
                }

            }
        }
    }
    fileIn.close();

    dbClassRoomWrite();

    return true;
}

///inserisce un nuovo anno accademico ad un corso già esistente
bool University::insertCourses(const std::string &fin) {

    std::vector<std::string> specificYearCourse;
    std::string acYear;
    std::string examData;
    bool isActive = true;
    int num_parallel_courses = 0;
    std::string profSenzaQuadre;
    std::vector<std::string> idGrouped;
    std::vector<std::string> splittedExamData;

    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file per inserimento di una nuova organizzazione del corso");
    }

    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        std::vector<std::string> specificYearCourse = Parse::splittedLine(line, ';');

        if (_courses.find(specificYearCourse[0]) ==
            _courses.end()) {//find mi restituisce literatore alla chiave inserita(IdCorso). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            throw std::invalid_argument("IdCorso non presente");
        }

        auto course_with_given_id = _courses.find(specificYearCourse[0]);//iteratore al corso da aggiornare
        ///fillSpecificYearCourse mi aggiorna il vettore specificYearCourse aggiungendo le info dell'anno accademico precedente negli spazi vuoti
        course_with_given_id->second.fillSpecificYearCourse(specificYearCourse);//passo l'intero vettore di stringhe by reference

        acYear = specificYearCourse[1]; //anno accademico
        if (specificYearCourse[2] == "attivo") //se attivo o meno
            isActive = true;
        else {
            isActive = false;
            for (auto iterStudyCourse = _studyCourse.begin();
                 iterStudyCourse != _studyCourse.end(); iterStudyCourse++) {
                iterStudyCourse->second.updateSemestersAndOffCourses(specificYearCourse[0]);
            }
        }
        ///come per la readCourse, aggiorno la mappa _courses
        num_parallel_courses = stoi(specificYearCourse[3]);//numero di corsi in parallelo
        profSenzaQuadre = specificYearCourse[4].substr(1, specificYearCourse[4].size() - 2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
        std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre, num_parallel_courses);//divido i vari corsi in parallelo
        examData = specificYearCourse[5];//informazioni sull'esame
        examData = examData.substr(1, examData.size() - 2);//tolgo le { } che racchiudono le info degli esami
        splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
        idGrouped = Parse::SplittedGroupedID(specificYearCourse[6]);
        _courses.at(specificYearCourse[0]).addSpecificYearCourses(acYear, isActive, num_parallel_courses, profCorsoPar,
                                                                  splittedExamData, idGrouped);
    }
    fileIn.close();

    dbCourseWrite();
    dbStudyCourseWrite();

    return true;
}

///riscrive il database degli studenti
void University::dbStudsWrite() {

    std::fstream fout;
    fout.open("../Sources/db_studenti.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterStud = _students.begin(); iterStud != _students.end(); iterStud++) {
        Student stud = _students.at( iterStud->first);//salvo in un oggetto Student temporaneo, l'intero oggetto puntato da iterStud
        fout << stud  << std::endl;//grazie all'overload dell'operatore << scrivo su file l'oggetto stud(si rimanda all'overload dell'operatore in Student.cpp)
    }
    fout.close();

}

///riscrive il database dei professori
void University::dbProfsWrite() {

    std::fstream fout;
    fout.open("../Sources/db_professori.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterProf = _professors.begin(); iterProf != _professors.end(); iterProf++) {
        Professor prof = _professors.at(iterProf->first);

        fout << prof << std::endl;
    }
    fout.close();
}

///riscrive il database delle classroom
void University::dbClassRoomWrite() {

    std::fstream fout;
    fout.open("../Sources/db_aule.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterClassRoom = _classroom.begin(); iterClassRoom != _classroom.end(); iterClassRoom++) {
        Classroom room = _classroom.at(iterClassRoom->first);
        fout << room << std::endl; //mettendo direttamente fout<<room come per stud e prof non funziona....
    }
    fout.close();
}

///riscrive il database dei corsi di studio
void University::dbStudyCourseWrite() {
    std::fstream fout;
    fout.open("../Sources/db_corsi_studio.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterStudyCourse = _studyCourse.begin(); iterStudyCourse != _studyCourse.end(); iterStudyCourse++) {
        StudyCourse sC = _studyCourse.at(iterStudyCourse->first);
        std::stringstream tok;
        tok << sC;
        fout << tok.str() << std::endl;
    }
    fout.close();

}

///riscrive il database dei corsi
void University::dbCourseWrite() {

    std::fstream fout;
    fout.open("../Sources/db_corsi.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        Course generalCourse = _courses.at(iterCourse->first);//prendo lintero oggetto Course generale per scrivere su file la riga che inizia con "c"
        fout << generalCourse;
    }
    fout.close();

}

///inserimento dell einfo per gli studenti che si iscrivono
bool University::enrollStudents(const std::string &fin) {
    std::fstream fileIn(fin);
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw DbException("file input non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> InteroStudente;    //accoglierà il vettore con la riga del file scissa
    char c;    //accoglierà la s della matricola presa dal file, a noi inutile
    int nMatr; //accoglierà il codice identificativo della matricola presa dal file, a noi utile
    std::vector<std::string> tokens;
    std::getline(fileIn, line);
    std::string cds, course;
    std::string coursesWithoutSquareBrackets;
    std::vector<std::string> courses;
    std::vector<std::string> infoStud;
    std::string idCorso, endrolAcYear;
    int mark;
    int year;
    cds = line.substr(0,4);
    year = stoi(line.substr(5,4));
    while (std::getline(fileIn, line)) {//fino alla fine del file leggo un rigo alla volta = 1 studente
        tokens = Parse::splittedLine(line, ';');
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr; //la "s" la scarto in "c", tengo il codice identificativo da mettere in un intero
        Student stud = _students.at(nMatr); //preso l'istanza dello studente di cui si parla
        coursesWithoutSquareBrackets = tokens[1].substr(1,tokens[1].size()-2);
        courses = Parse::splittedLine(coursesWithoutSquareBrackets,'%');
        for(int i=0; i<courses.size(); i++){
            course = courses[i].substr(1,courses[i].size()-2);
            infoStud = Parse::splittedLine(course,',');
            idCorso = infoStud[0];
            endrolAcYear = infoStud[1];
            if(infoStud[2].empty())
                mark = -1;
            else
                mark = stoi(infoStud[2]);
            _courses.at(idCorso).addStudentToSpecYearCourse(year,stud,endrolAcYear,mark);
        }
    }
    fileIn.close();
    return true;
}

///legge le date della sessione esame
void University::readSessionAcYear() {
    std::ifstream fileIn("../Sources/dateSessioni.txt");
    if (!fileIn.is_open()) {
        throw DbException("file dateSessioni.txt non esistente");
    }

    std::string line;
    while (std::getline(fileIn, line)){
        std::vector<std::string> infoSessions = Parse::splittedLine(line,';');
        _acYearSessions.addSession(infoSessions[0],infoSessions[1],"winterSession");//infoSessions[0] = anno accademico. Aggiungo all'anno accademico la sessione invernale
        _acYearSessions.addSession(infoSessions[0],infoSessions[2],"summerSession");//estiva
        _acYearSessions.addSession(infoSessions[0],infoSessions[3],"autumnSession");//e autunnale
    }
    fileIn.close();

}

///setta il periodo delle sessioni
bool University::set_session_period(const std::string& acYear,const std::string &wSession, const std::string &sSession, const std::string &aSession) {

    std::string accademicYear = acYear;//anno accademico da settare
    std::string winterSession = wSession.substr(0,wSession.size());//tolgo le virgolette
    std::string summerSession = sSession.substr(0,sSession.size());
    std::string autumnSession = aSession.substr(0,aSession.size());
    //pulisci mappa delle date delle sessioni
    _acYearSessions.clearYearSession();//anche se in teoria sovrascrive comunque
    _acYearSessions.addSession(accademicYear,winterSession,"winterSession");//aggiungo all'anno accademico la sessione invernale
    _acYearSessions.addSession(accademicYear,summerSession,"summerSession");//estiva
    _acYearSessions.addSession(accademicYear,autumnSession,"autumnSession");//e autunnale

    dateSessionsWrite();
    return true;
}

///salva le date delle sessioni su file
void University::dateSessionsWrite() {

    std::fstream fout;
    fout.open("../Sources/dateSessioni.txt", std::fstream::out | std::fstream::trunc);
    SessionYear dateSessions = _acYearSessions;
    fout << dateSessions;

    fout.close();

}

///lettura delle indisponibilità professori
void University::readProfsAvailability() {
    std::ifstream fileIn("../Sources/tutte_le_indisponibilita.txt");
    if (!fileIn.is_open()) {

        throw DbException("file tutte_le_indisponibilita non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> profAvailability;
    while (std::getline(fileIn, line)) {
       if(!line.empty()) {
           profAvailability = Parse::splittedLine(line, ';');
           int acYear = Parse::getAcStartYear(profAvailability[0]);
           int nMatr = Parse::getMatr(profAvailability[1]);
           for (int i = 2;
                i < profAvailability.size(); i++) {//per il numero di periodi di indisponibilità del singolo prof
               _professors.at(nMatr).setAvaibilities(acYear,
                                                     profAvailability[i]);//vado a settare l'indisponibilità del prof nella map _professor
           }
       }
    }
}

///set indisponibilità professori
bool University::setProfsAvailability(std::string acYear,const std::string& fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw DbException("file availabilities non esistente");
    }
    std::string line;
    std::vector<std::string> profAvailability;
    int nMatr, year;
    year = Parse::getAcStartYear(acYear);

    while (std::getline(fileIn, line)) {//fino alla fine del file leggo un rigo alla volta
        profAvailability = Parse::splittedLine(line,';');
        nMatr = Parse::getMatr(profAvailability[0]);//prendo la matricola
        auto iterProf = _professors.find(nMatr);//punto al prof con matricola nMatr
        iterProf->second.availabilityClear();//pulisco la map delle indisponibilità
        for(int i=1; i<profAvailability.size(); i++){//per ogni campo della riga letta
            _professors.at(nMatr).setAvaibilities(year,profAvailability[i]);//vado a settare uno dei periodi di indisponibilità del prof nella map _professor
        }
    }
    fileIn.close();

    availabilityWrite(year);//riscrivo solo le info dell'anno richiesto dal terminale

       return true;
}

///scrittura indisponibilità professori
void University::availabilityWrite(int year) {
    std::fstream fout;
    fout.open("../Sources/tutte_le_indisponibilita.txt", std::fstream::out | std::fstream::trunc);
    std::vector<std::string> allprofsAvailabilities = allProfsAvailabilities(year);//ritorna le indisponibilità per ogni prof già sottoforam di stringhe da salvare sul file.txt
    for(int i = 0; i<allprofsAvailabilities.size();i++){//salvo le varie stringhe di allprofsAvailabilities
        fout<<allprofsAvailabilities[i]<<std::endl;
    }
    fout.close();
}

///prende tutte le indisponibilità di tutti i prof
std::vector<std::string> University::allProfsAvailabilities(int year) {
    std::vector<std::string> allProfsAvailabilities;
    for(auto iterProfs = _professors.begin(); iterProfs != _professors.end(); iterProfs++){//per ogni professore

        int id = iterProfs->first;
        if(!availabilityPeriodIsEmpty(id, year)) {//controllo se il professore ha un periodo di indisponibilità
            std::string profAvailabilities = iterProfs->second.outputAvailabilities(id, year);//prende le indisponibilità di un professore
            allProfsAvailabilities.push_back(profAvailabilities);//salvo le indisponibilità di un professore
        }
    }
    return allProfsAvailabilities;
}

bool University::availabilityPeriodIsEmpty(int id,int year) {
 auto iterator = _professors.find(id);//iteratore al prof analizzato
 return iterator -> second.availabilityPeriodIsEmpty(year);//controlla che per quell'anno accademico sia vuoto il periodo di indisponibilità
 }









































