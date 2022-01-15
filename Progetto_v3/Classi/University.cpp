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
#include "InvalidDbException.h"
#include "Parse.hpp"
#include <algorithm>


//namespace fs = std::filesystem;
int versioning = 0;

University::University() : _version(1) {
    try {
        readVersion();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readStudents();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readProfessor();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readClassroom();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readStudyCourse();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readCourse();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readSessionAcYear();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readProfsNoAvailability();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readCourseNotActive();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readStudyPlan();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readPassedAppeals();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readAllExamAppeals();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl;
    }
    try {
        readAllMinDistanceRequest();
    }
    catch (DbException &exc) {
        //std::cerr << exc.what() << std::endl <<std::endl;
    }
}

///leggo il database degli studenti
void University::readStudents() {
    //dbstudenti
    std::ifstream fileIn("db_studenti.txt");
    ///leggi version
    if (!fileIn.is_open()) {
        throw DbException("file db_studenti.txt non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> InteroStudente;    //accoglierà il vettore con la riga del file scissa
    int nMatr; //accoglierà il codice identificativo della matricola presa dal file, a noi utile

    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        InteroStudente = Parse::splittedLine(line, ';');
        nMatr = Parse::getMatr(InteroStudente[0]);
        ///controllo se la matricola è già esistente; in quel caso lancio un'eccezione, altrimenti inserisco lo studente con tutti i suoi dati
        if (_students.count(nMatr))
            throw std::logic_error("matricola già presente");
        else if (_version == 1) {
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, InteroStudente[1], InteroStudente[2],
                                                                    InteroStudente[3])));//la chiave intera è la matricola; ad ogni chiave/matricola è associato uno studente
        } else {
            ///version >= 2, ai fini di test avremo solo =2 oppure =3
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, InteroStudente[1], InteroStudente[2],
                                                                    InteroStudente[3], InteroStudente[4],
                                                                    InteroStudente[5], InteroStudente[6])));
        }
    }
    fileIn.close();
}

void University::readVersion() {
    std::ifstream file("versione.txt");
    if (!file.is_open()) {
        throw DbException("file versione.txt non esistente. Non e' ancora stato usato il comando di versioning");
    }
    std::string version;
    std::getline(file, version);
    _version = stoi(version);
    file.close();
}

///identico alla readStudents(); si evita di commentare per non sporcare il codice
void University::readProfessor() {
    //dbprofessori
    std::ifstream fileIn("db_professori.txt");
    if (!fileIn.is_open()) {
        throw DbException("file db_professori.txt non esistente");
    }
    std::string line;
    std::vector<std::string> InteroProfessore;
    int nMatr;
    while (std::getline(fileIn, line)) {
        InteroProfessore = Parse::splittedLine(line, ';');
        nMatr = Parse::getMatr(InteroProfessore[0]);
        if (_professors.count(nMatr))
            throw std::logic_error("due matricole uguali");

        else if (_version == 2) {
            _professors.insert(std::pair<int, Professor>(nMatr,
                                                         Professor(nMatr, InteroProfessore[1], InteroProfessore[2],
                                                                   InteroProfessore[3], InteroProfessore[4],
                                                                   InteroProfessore[5], InteroProfessore[6])));
        } else
            _professors.insert(std::pair<int, Professor>(nMatr,
                                                         Professor(nMatr, InteroProfessore[1], InteroProfessore[2],
                                                                   InteroProfessore[3])));
    }
    fileIn.close();
}

///lettura delle aule dal database
void University::readClassroom() {

    //dbClassroom
    std::ifstream fileIn("db_aule.txt");
    if (!fileIn.is_open()) {
        throw DbException("file db_aule.txt non esistente");
    }
    std::string line;
    std::vector<std::string> InteraClasse;
    int nCod = 0;
    while (std::getline(fileIn, line)) {
        InteraClasse = Parse::splittedLine(line, ';');
        nCod = Parse::getMatr(InteraClasse[0]);

        if (_classroom.count(nCod))
            throw std::logic_error("due codici uguali");
        else if (_version == 2) {
            _classroom.insert(std::pair<int, Classroom>(nCod, Classroom(nCod, InteraClasse[1], InteraClasse[2],
                                                                        std::stoi(InteraClasse[3]),
                                                                        std::stoi(InteraClasse[4]),
                                                                        std::stoi(InteraClasse[5]),
                                                                        std::stoi(InteraClasse[6]),
                                                                        std::stoi(InteraClasse[7]),
                                                                        std::stoi(InteraClasse[8]))));

        } else
            _classroom.insert(std::pair<int, Classroom>(nCod, Classroom(nCod, InteraClasse[1], InteraClasse[2],
                                                                        std::stoi(InteraClasse[3]),
                                                                        std::stoi(InteraClasse[4]))));

    }
    fileIn.close();
}

///lettura dei corsi dal database
void University::readCourse() {
    std::ifstream fileIn("db_corsi.txt");
    if (!fileIn.is_open()) {
        throw DbException("file db_corsi.txt non esistente");
    }
    int line_counter = 1;
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
            _courses.insert(std::pair<std::string, Course>(lastReadCourse,
                                                           Course(interoCorso[1], interoCorso[2], stoi(interoCorso[3]),
                                                                  stoi(interoCorso[4]), stoi(interoCorso[5]),
                                                                  stoi(interoCorso[6]))));

        } else if (interoCorso[0] ==
                   "a") {//se la riga letta inizia con "a" devo inserire le info di un anno accademico per un corso generale letto
            specificYearCourse = interoCorso;//per una maggiore comprensione nella lettura del codice
            acYear = specificYearCourse[1]; //anno accademico
            if (specificYearCourse[2] == "attivo") //se attivo o meno
                isActive = true;
            else
                isActive = false;
            ///numero di corsi in parallelo
            num_parallel_courses = stoi(specificYearCourse[3]);
            ///estraggo gli id di tutti i prof di tutti i corsi in parallelo
            profSenzaQuadre = specificYearCourse[4].substr(1, specificYearCourse[4].size() - 2);
            ///divido i vari corsi in parallelo
            std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre, num_parallel_courses);
            examData = specificYearCourse[5];//informazioni sull'esame
            ///tolgo le { } che racchiudono le info degli esami
            examData = examData.substr(1, examData.size() - 2);
            splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
            idGroupedCourse = specificYearCourse[6];//id dei vari corsi in parallelo
            /// tolgo le { } che racchiudono gli id
            idGroupedCourse = idGroupedCourse.substr(1, idGroupedCourse.size() - 2);
            idGrouped = Parse::splittedLine(idGroupedCourse, ',');//scissione degli id dei corsi raggruppati
            ///ricerca "anno-semestre" di questo corso
            std::string yy_semester;
            std::vector<int> studyCourse;
            for (int i = 1; i <= _studyCourse.size(); i++) {
                if (isActive) {
                    std::string result = _studyCourse.at(i).isInWhichSemester(lastReadCourse);
                    if (!result.empty()) {
                        //ho trovato il suo corso di studi ed è attivo
                        yy_semester = result;
                    }
                }
                auto iterStudyCourse = _studyCourse.find(i);
                if (iterStudyCourse->second.assignStudyCourse(lastReadCourse))
                    studyCourse.push_back(iterStudyCourse->first);
            }

            ///la addSpecificYearCourses serve per accedere e aggiornare una map (_courseOfTheYear) presente in ogni oggetto Course della mappa _courses, che contiene i vari anni accademici per ogni corso
            _courses.at(lastReadCourse).addSpecificYearCourses(acYear, isActive, num_parallel_courses, profCorsoPar,
                                                               splittedExamData, idGrouped, yy_semester,
                                                               studyCourse,
                                                               line_counter);//aggiungo ad un corso un anno accademico e le relative info nella map _courses
        }
    }
    line_counter++;
    fileIn.close();
}

///lettura dei corsi di studio dal database
void University::readStudyCourse() {

    int i;
    std::ifstream fileIn("db_corsi_studio.txt");
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
        int codCorso = Parse::getMatr(InteroCorsoDiStudi[0]);
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
        ///è possibile leggere i corsi spenti solo se questi, leggendo da database, sono presenti
        ///e quindi solo se il vettore InteroCorsoDiStudi presenta un quarto campo
        if (InteroCorsoDiStudi.size() == 4) {
            corsiSpentiSenzaQuadre = InteroCorsoDiStudi[3].substr(1, InteroCorsoDiStudi[3].size() -
                                                                     2);    //salvo la stringa senza le quadre
            corsiSpenti = Parse::splittedLine(corsiSpentiSenzaQuadre, ',');//splitto i corsi spenti senza le quadre
        }
        ///creo un oggetto StudyCourse
        bool isBachelor = false;
        if (levelCourse.compare("BS") == 0)//se corso di studi triennale
            isBachelor = true;

        if ((isBachelor && semestri.size() != 6) || (!isBachelor && semestri.size() != 4)) {
            throw InvalidDbException(
                    "formato file corsi di studio non valido: ci sono semestri senza corsi o numero semestri incompatibile con tipo di laurea");
        }

        StudyCourse SCourse(codCorso, isBachelor);
        //carico corsi e semestri letti nello studycourse
        int year = 0, numSemester = 1;
        for (i = 0; i < semestri.size(); i++) {
            year = 1 + i / 2; //i=0 => y = 1, i=1 => y = 1, i=2 => y = 2; i=3 => y = 2
            numSemester = 1 + i % 2; //i=0 => s = 1, i=1 => s = 2, i=2 => s = 1; i=3 => s = 2
            SCourse.addSemesterCourses(year, numSemester, semestri[i], _studyCourse, _courses,
                                       codCorso);//passo: l'anno, primo o secondo semestre,tutta la stringa di corsi del semestre
        }

        ///solo se presenti aggiungo i corsi spenti
        if (InteroCorsoDiStudi.size() == 4) {
            SCourse.addOffCourses(corsiSpenti);
        }
        _studyCourse.insert(std::pair<int, StudyCourse>(codCorso,
                                                        SCourse));//inserisco il corso alla mappa dei corsi di studio la cui chiave è il codice del corso privato di "A" iniziale
    }

    fileIn.close();
}

///inserisco un nuovo studente (privo di matricola che bisogna quindi ricavare)
void University::addStuds(const std::string &fileIn) {

    std::fstream fIn(fileIn, std::ios::in); //apro il file in lettura
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuovi studenti \n");
    }
    bool doDbWrite = true;
    std::string error;
    int line_counter = 1;
    std::string line; //stringa di appoggio per l'intera riga del file
    std::vector<std::string> infoStud; //vettore di stringhe che accoglierà il ritorno della funzione splittedLine
    while (std::getline(fIn, line)) {//fino alla fine del file leggo un rigo alla volta
        infoStud = Parse::splittedLine(line, ';');
        int matr = getNewStudentId(); //calcolo la matricola del nuovo studente
        //controllo che formato file sia corretto: 3 campi
        if (_version == 2) {
            if (infoStud.size() != 6) {
                error.append("errore formato file studenti alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (Parse::controlFieldsVectorAreEmpty(infoStud)) {
                error.append("uno dei campi di informazione per l'aggiunta degli studenti e' vuoto alla riga: " +
                             std::to_string(line_counter) + "\n");
                doDbWrite = false;
            } else if (infoStud.size() == 6) {
                _students.insert(std::pair<int, Student>(matr, Student(matr, infoStud[0], infoStud[1], infoStud[2],
                                                                       infoStud[3], infoStud[4],
                                                                       infoStud[5]))); //inserisco il nuovo studente nella mappatura interna
            }
        } else {
            if (infoStud.size() != 3) {
                error.append("errore formato file studenti alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (Parse::controlFieldsVectorAreEmpty(infoStud)) {
                error.append("uno o piu campi di informazione per l'aggiunta degli studenti e' vuoto alla riga: " +
                             std::to_string(line_counter) + "\n");
                doDbWrite = false;
            } else if (infoStud.size() == 3) {
                _students.insert(std::pair<int, Student>(matr, Student(matr, infoStud[0], infoStud[1],
                                                                       infoStud[2]))); //inserisco il nuovo studente nella mappatura interna
            }
        }
        line_counter++;
    }
    fIn.close();
    if (doDbWrite)
        dbStudsWrite();
    else
        throw std::invalid_argument(error);
}

///identico alla insertStudents(); si evita di commentare per non sporcare il codice
void University::addProfessors(const std::string &fileIn) {
    //addprofessor
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuovi professori \n");
    }
    bool doDbWrite = true;
    std::string line;
    int line_counter = 1;
    std::vector<std::string> infoProf;
    std::string error;
    while (std::getline(fIn, line)) {
        int matr = getNewProfessorId();
        infoProf = Parse::splittedLine(line, ';');
        if (_version == 2) {
            //controllo che formato file sia corretto: 3 campi
            if (infoProf.size() != 6) {
                error.append("errore formato file professori alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (Parse::controlFieldsVectorAreEmpty(infoProf)) {
                error.append("uno o piu' campi di informazione per l'aggiunta dei professori e' vuoto alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }else if (infoProf.size() == 6) {
                _professors.insert(std::pair<int, Professor>(matr,Professor(matr, infoProf[0], infoProf[1], infoProf[2],infoProf[3],infoProf[4], infoProf[5])));
            }
        } else {
            //controllo che formato file sia corretto: 3 campi
            if (infoProf.size() != 3) {
                error.append("errore formato file professori alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (Parse::controlFieldsVectorAreEmpty(infoProf)) {
                error.append("uno o piu' campi di informazione per l'aggiunta dei professori e' vuoto alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }else if (infoProf.size() == 3)
                _professors.insert(std::pair<int, Professor>(matr, Professor(matr, infoProf[0], infoProf[1], infoProf[2])));
        }
        line_counter++;
    }
    fIn.close();

    if (doDbWrite)
        dbProfsWrite();
    else
        throw std::invalid_argument(error);

}

///inserisci nuove aule
void University::addClassrooms(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuove aule");
    }
    std::string line;
    bool doDbWrite = true;
    int line_counter = 1;
    std::string error;
    std::vector<std::string> infoClassroom;
    while (std::getline(fIn, line)) {
        infoClassroom = Parse::splittedLine(line, ';');
        int id = getNewClassroomId();
        if (_version == 3) {
            //controllo che il formato del file sia corretto: 7 campi
            if (infoClassroom.size() != 7) {
                error.append("errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (Parse::controlFieldsVectorAreEmpty(infoClassroom)) {
                error.append("uno o piu' campi di informazione per l'aggiunta delle aule e' vuoto alla riga: " +
                             std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (doDbWrite) {
                try {
                    _classroom.insert(std::pair<int, Classroom>(id, Classroom(id, infoClassroom[0], infoClassroom[1],
                                                                              std::stoi(infoClassroom[2]),
                                                                              std::stoi(infoClassroom[3]),
                                                                              std::stoi(infoClassroom[4]),
                                                                              std::stoi(infoClassroom[5]),
                                                                              std::stoi(infoClassroom[6]),
                                                                              std::stoi(infoClassroom[7]))));

                } catch (std::exception &err) {
                    std::string generalError = err.what();
                    error.append(generalError + " alla riga: " + std::to_string(line_counter));
                    doDbWrite = false;
                }
            }
        }else {
            //controllo che il formato del file sia corretto: 4 campi
            if (infoClassroom.size() != 4) {
                error.append("errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (Parse::controlFieldsVectorAreEmpty(infoClassroom)) {
                error.append("uno o piu' campi di informazione per l'aggiunta delle aule e' vuoto alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (doDbWrite) {
                try {
                    _classroom.insert(std::pair<int, Classroom>(id, Classroom(id, infoClassroom[0], infoClassroom[1],
                                                                              std::stoi(infoClassroom[2]),
                                                                              std::stoi(infoClassroom[3]))));
                } catch (std::exception &err) {
                    std::string generalError = err.what();
                    error.append(generalError + " alla riga: " + std::to_string(line_counter));
                    doDbWrite = false;
                }
            }
        }
        line_counter++;
    }
    fIn.close();

    if (doDbWrite)
        dbClassRoomWrite();
    else
        throw std::invalid_argument(error);
}


///inserimento dei nuovi corsi di studio
std::vector<std::string> University::addStudyCourses(const std::string &fin) {

    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento nuovi corsi di studio");
    }
    int i;
    bool doDbwrite = true;
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::vector<std::string> infoStudyCourse;    //accoglierà il vettore con la riga del file scissa
    while (std::getline(fileIn, line)) {
        ///codice, livello
        infoStudyCourse = Parse::splittedLine(line, ';');//inserisco i vari campi delimitati dal ;
        if (infoStudyCourse.size() != 2) {
            _errorStringUniversity.push_back(
                    "errore formato file corsi di studio alla riga: " + std::to_string(line_counter));
            doDbwrite = false;
        } else {
            int codCorso = getNewStudyCourseId();
            std::string levelCourse = infoStudyCourse[0];//triennale o magistrale
            ///devo leggere i semestri
            std::vector<int> posSem;
            ///cerco nella stringa se ci sono i due caratteri inseriti nella find_first_of
            std::size_t found = infoStudyCourse[1].find_first_of("{}");
            while (found !=
                   std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
                posSem.push_back(
                        found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
                found = infoStudyCourse[1].find_first_of("{}", found + 1);//continuo a controllare la stringa
            } //alla fine di questo while posSem conterrà le posizioni in corrispondenza delle quali nel vettore tokens[1] sono presenti: { o }

            std::vector<std::string> semestri;
            for (i = 0; i < posSem.size() - 1; i = i +
                                                   2) { //metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
                int posStart = posSem[i] + 1;// tolgo la graffa
                int len = posSem[i + 1] - posSem[i] - 1; //pos(}) - pos({) -1
                semestri.push_back(infoStudyCourse[1].substr(posStart,
                                                             len));   //salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato
            } //alla fine di questo for il vector "semestre" conterrà i corsi di ogni semestre disposti al suo interno in modo che ogni "cella" di "semestre" contiene tutti i corsi di un certo semestre
            ///semestre[0] = tutti i corsi di anno1_semestre1, semestre[1] = tutti i  corsi anno1_semestre2, semestre[2] = tutti i  corsi anno2_semestre1, ...
            ///controllo che formato file sia corretto:
            ///se L3 -> 6 semestri, se LM -> 4 semestri
            if ((levelCourse == "BS" && semestri.size() != 6) || (levelCourse == "MS" && semestri.size() != 4)) {
                _errorStringUniversity.push_back(
                        "formato file corsi di studio non valido: ci sono semestri senza corsi o numero semestri incompatibile con tipo di laurea alla riga:" +
                        std::to_string(line_counter));
                doDbwrite = false;
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
                std::vector<std::string> possibleErrors = SCourse.addSemesterCourses(year, numSemester, semestri[i],
                                                                                     _studyCourse, _courses,
                                                                                     line_counter);//passo: l'anno, primo o secondo semestre,tutta la stringa di corsi del semestre
                if (possibleErrors.empty() == false) {
                    _errorStringUniversity.insert(_errorStringUniversity.end(), possibleErrors.begin(),
                                                  possibleErrors.end());
                    doDbwrite = false;
                }
            }
            _studyCourse.insert(std::pair<int, StudyCourse>(codCorso, SCourse));
            ifThereAreAlreadyCoursesFillYYSemesterVar(SCourse);
        }
        line_counter++;
    }
    ///controllo che i corsi raggruppati non siano negli stessi cds
    if(!controlAGAINGroupedCoursesDifferentCds_Reciprocy())
        doDbwrite = false;
    fileIn.close();
    if (doDbwrite)
        dbStudyCourseWrite();

    return _errorStringUniversity;
}

///inserisco dei nuovi corsi
std::vector<std::string> University::addCourses(const std::string &fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento nuovi corsi");
    }
    std::string line;
    int line_counter = 1;
    std::vector<std::string> specificYearCourse;
    std::string acYear;
    bool doDbWrite;
    std::string examData;
    std::string idGroupedCourse;
    bool isActive = true;
    int num_parallel_courses = 0;
    std::string profSenzaQuadre;
    std::vector<std::string> idGrouped;
    std::vector<std::string> splittedExamData;
    bool doDBwrite = true;
    std::string error;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        specificYearCourse = Parse::splittedLine(line, ';');
        //controllo che il formato file sia corretto: 10 campi
        if (specificYearCourse.size() != 10) {
            error.append("formato file corsi non valido alla riga: " + std::to_string(line_counter) + "\n");
            doDBwrite = false;
        }
        //controllo che tutti campi siano specificati
        for (int i = 0; i < specificYearCourse.size(); i++) {
            if (specificYearCourse[i].empty()) {
                error.append("uno o più campi sono vuoti alla riga: " + std::to_string(line_counter) + "\n");
            }
        }
        //controllo che l'esame non sia già presente in base dati
        //cerco nel DB un esame con stesso titolo e cfu
        for (auto iterCourses = _courses.begin(); iterCourses != _courses.end(); iterCourses++) {
            if (iterCourses->second.getName() == specificYearCourse[1] &&
                iterCourses->second.getCfu() == stoi(specificYearCourse[2])) {
                error.append("c'è un corso già presente in base dati alla riga: " + std::to_string(line_counter)+"\n");
                doDBwrite = false;
            }
        }
        if (doDBwrite) {
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
            std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre,
                                                                      num_parallel_courses);//divido i vari corsi in parallelo
            examData = specificYearCourse[8];//informazioni sull'esame
            examData = examData.substr(1, examData.size() - 2);//tolgo le { } che racchiudono le info degli esami
            splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
            idGroupedCourse = specificYearCourse[9];//id dei vari corsi raggruppati
            idGroupedCourse = idGroupedCourse.substr(1,idGroupedCourse.size() - 2);// tolgo le { } che racchiudono gli id
            idGrouped = Parse::splittedLine(idGroupedCourse, ',');//scissione degli id dei corsi raggruppati
            ///ricerca "anno-semestre" di questo corso
            std::string yy_semester;
            std::vector<int> studyCourse;

            for (int i = 1; i <= _studyCourse.size(); i++) {
                ///se gli study course già ci sono, entra, altrimenti non esegue for
                std::string result = _studyCourse.at(i).isInWhichSemester(newIdCourse);
                if (!result.empty()) {
                    //ho trovato il suo corso di studi
                    auto iterStudyCourse = _studyCourse.find(i); //prendo id del corso di studio associato
                    studyCourse.push_back(iterStudyCourse->first);
                    yy_semester = result;
                    if (!idGrouped.empty()) {
                        try {
                            controlGroupedCoursesDifferentCds_Reciprocy(idGrouped, specificYearCourse[1], line_counter,
                                                                        newIdCourse);
                        }
                        catch (std::exception &err) {
                            error.append(err.what());
                            doDbWrite = false;
                        }
                    }
                }
            }
            ///update: questa parte commentata perchè course può venire prima di studyCourse
            /*if (_studyCourse.empty())
                throw InvalidDbException("Per inserire nuovi corsi, devi prima definire i corsi di studio relativi");
            for (int i = 1; i <= _studyCourse.size(); i++) {
                std::string result = _studyCourse.at(i).isInWhichSemester(newIdCourse);
                if (!result.empty()) {
                    //ho trovato il suo corso di studi

                    auto iterStudyCourse = _studyCourse.find(i); //prendo id del corso di studio associato
                    studyCourse.push_back(iterStudyCourse->first);
                    yy_semester = result;
                    if (!idGrouped.empty()) {
                        fillGroupedCourse(idGrouped, newIdCourse, acYear, line_counter);
                    }
                    ///controllo i corsi raggruppati (genera eccezione se errore)
                    controlGroupedCoursesDifferentCds_Reciprocy(i, idGrouped, specificYearCourse[1], line_counter, newIdCourse);
                }
            }
            if (yy_semester.empty())
                throw InvalidDbException(
                        "un corso deve essere associato ad almeno un corso di studio! Codice del corso non utilizzato:" +
                        newIdCourse);
            */
            try {
                _courses.at(newIdCourse).addSpecificYearCourses(acYear, isActive, num_parallel_courses, profCorsoPar,
                                                                splittedExamData, idGrouped, yy_semester, studyCourse,
                                                                line_counter);
            }
            catch (std::exception &err) {
                error.append(err.what());
                doDbWrite = false;
            }
            ///qui: fill dei corsi raggruppati dell'anno accademico
            if (!idGrouped.empty()) {
                fillGroupedCourse(idGrouped, newIdCourse, acYear, line_counter);
            }
            else {
                if (_coursesGrouped.count(newIdCourse) != 0) {
                    idGrouped = _coursesGrouped.at(acYear+"-"+newIdCourse);
                    SpecificYearCourse &specificYY = _courses.at(newIdCourse).getThisYearCourseReference(Parse::getAcStartYear(acYear));//corso per un anno specifico
                    specificYY.assignGrouped(idGrouped, newIdCourse, newIdCourse);
                }
            }
            ///controllo che i professori di questo corso esistano già in _professors
            try {
                _courses.at(newIdCourse).controlTheExistenceAndHoursOfProfessors(_professors,Parse::getAcStartYear(acYear));
            }catch (std::exception &err){
                error.append(err.what());
                doDbWrite = false;
            }
        }
        line_counter++;
    }
    ///controlli che possono essere fatti solo dopo aver letto tutto il file
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        ///se c'è un gap tra anni prendo il precedente
        iterCourse->second.fillAcYearsEmpty();
        ///se il corso è stato disattivato non può essere riattivato
        try {
            iterCourse->second.notActive();
        }catch (std::exception &err){
            error.append(err.what());
            doDbWrite = false;
        }
        ///controllo se i corsi raggruppati siano dello stesso semestre del principale
        try {
            iterCourse->second.sameSemesterGrouped(_courses);
        }catch (std::exception &err){
            error.append(err.what());
            doDbWrite = false;
        }
    }
    if (doDbWrite) {
        fileIn.close();
        dbCourseWrite();
    }
    else{
        throw std::invalid_argument(error);
    }
    return _errorStringUniversity;
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
    update_name = 1,
    update_surName = 2,
    update_eMail = 3,
    update_birth = 4,
    update_registrationOrEntry = 5,
    update_address = 6
};

void University::updateStuds(const std::string &fin) {
    int i;
    char c;
    int nMatr = 0;
    bool doDbWrite = true;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file per aggiornamento studenti \n");
    }
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::vector<std::string> infoStud;
    std::string error;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoStud = Parse::splittedLine(line, ';');
        bool canBeAnId = Parse::controlItCanBeAnId(infoStud[0], 6);
        std::stringstream ss(infoStud[0]);
        ss >> c >> nMatr;

        if (_version == 2) {
            if (std::count(line.begin(), line.end(), ';') != 6) {
                error.append("errore formato file alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
        } else if (std::count(line.begin(), line.end(), ';') != 3) {
            error.append("errore formato file alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        }
        if (infoStud[0].empty()) {
            error.append("manca la matricola dello studente alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        } else if (canBeAnId == false) {
            error.append("il primo campo alla riga " + std::to_string(line_counter) + " non puo' essere un codice Id di uno studente \n");
            doDbWrite == false;
        } else if (_students.find(nMatr) == _students.end()) { //find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            error.append("Lo studente " + infoStud[0] + " non e' presente nel database. (Riga: " + std::to_string(line_counter) + ") \n");
            doDbWrite = false;
        }
        if (doDbWrite) {
            auto iter = _students.find(nMatr);//prendo la posizione della matricola
            for (i = 0; i < infoStud.size(); i++) {  //analizzo i campi della riga del file passato come parametro che andranno aggiornati
                if (!(infoStud[i].empty())) {//se la stringa raccolta da tokens è vuota l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati
                    switch (i) {
                        case update_name : {//analizzo il nome
                            if (!(iter->second.getName() ==
                                  infoStud[i])) { //se il nome letto dal file è diverso dal nome del database
                                iter->second.updateName(infoStud[i]); //cambio il nome
                            }
                            break;
                        }
                        case update_surName : {//analizzo il cognome
                            if (!(iter->second.getSurname() ==
                                  infoStud[i])) {//se il cognome letto dal file è diverso dal cognome del database
                                iter->second.updateSurnName(infoStud[i]); //cambio il cognome
                            }
                            break;
                        }
                        case update_eMail : { //analizzo l'email
                            if (!(iter->second.getEmail() ==
                                  infoStud[i])) {//se l'email letta dal file è diversa dall'email del database
                                iter->second.updateEmail(infoStud[i]); //cambia l'email
                            }
                            break;
                        }
                        case update_birth: {
                            if (!(iter->second.getBirth() ==
                                  infoStud[i])) {//se la data di nascitaletta dal file è diversa dall'email del database
                                iter->second.updateBirth(infoStud[i]); //cambia Data di nascita
                            }
                            break;
                        }
                        case update_registrationOrEntry: {
                            if (!(iter->second.getRegistrationOrEntry() ==
                                  infoStud[i])) {//se la data di registrazione letta dal file è diversa dall'email del database
                                iter->second.updateRegistration(infoStud[i]); //cambia Data di registrazione
                            }
                            break;
                        }
                        case update_address: {
                            if (!(iter->second.getAddress() ==
                                  infoStud[i])) {//se l'indirizzo letto dal file è diversa dall'email del database
                                iter->second.updateAdress(infoStud[i]); //cambia l'indirizzo
                            }
                            break;
                        }
                    }
                }
            }
        }

        line_counter++;
    }

    fileIn.close();
    if (doDbWrite)
        dbStudsWrite();

}

///aggiorno i professori
void University::updateProfessors(const std::string &fin) {
    int i;
    char c;
    int nMatr = 0;
    bool doDbWrite = true;
    std::string error;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file per aggiornamento professori \n");
    }
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::vector<std::string> infoProf;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoProf = Parse::splittedLine(line, ';'); //splitto il rigo nei vari campi di interesse
        bool canBeAnId = Parse::controlItCanBeAnId(infoProf[0], 6);
        std::stringstream ss(infoProf[0]);
        ss >> c >> nMatr;

        if (_version == 2) {
            if (std::count(line.begin(), line.end(), ';') != 6) {
                _errorStringUniversity.push_back(
                        "errore formato file professori alla riga: " + std::to_string(line_counter));
                doDbWrite = false;
            }
        } else if (std::count(line.begin(), line.end(), ';') != 3) {
            error.append("errore formato file professori alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        }
        if (infoProf[0].empty()) {
            error.append("manca la matricola del professore alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        } else if (canBeAnId == false) {
            error.append("il primo campo alla riga " + std::to_string(line_counter) + " non puo' essere un codice Id di un professore \n");
            doDbWrite == false;
        } else if (_professors.find(nMatr) == _professors.end()) {//find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            error.append("Il professore " + infoProf[0] + " non e' presente nel database \n");
            doDbWrite = false;
        }
        if (doDbWrite) {
            auto iter = _professors.find(nMatr);//prendo la posizione della matricola

            for (i = 0; i < infoProf.size(); i++) {  //analizzo i campi della riga del file passato come parametro che andranno aggiornati
                if (!(infoProf[i].empty())) {//se la stringa raccolta da tokens è vuota l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati
                    switch (i) {
                        case update_name : {//analizzo il nome
                            if (!(iter->second.getName() ==
                                  infoProf[i])) { //se il nome letto dal file è diverso dal nome del database
                                iter->second.updateName(infoProf[i]); //cambio il nome
                            }
                            break;
                        }
                        case update_surName : {//analizzo il cognome
                            if (!(iter->second.getSurname() ==
                                  infoProf[i])) {//se il cognome letto dal file è diverso dal cognome del database
                                iter->second.updateSurnName(infoProf[i]); //cambio il cognome
                            }
                            break;
                        }
                        case update_eMail : { //analizzo l'email
                            if (!(iter->second.getEmail() ==
                                  infoProf[i])) {//se l'email letta dal file è diversa dall'email del database
                                iter->second.updateEmail(infoProf[i]); //cambia l'email
                            }
                            break;
                        }
                        case update_birth: {
                            if (!(iter->second.getBirth() ==
                                  infoProf[i])) {//se la data di nascitaletta dal file è diversa dall'email del database
                                iter->second.updateBirth(infoProf[i]); //cambia Data di nascita
                            }
                            break;
                        }
                        case update_registrationOrEntry: {
                            if (!(iter->second.getRegistrationOrEntry() ==
                                  infoProf[i])) {//se la data di registrazione letta dal file è diversa dall'email del database
                                iter->second.updateRegistration(infoProf[i]); //cambia Data di registrazione
                            }
                            break;
                        }
                        case update_address: {
                            if (!(iter->second.getAddress() ==
                                  infoProf[i])) {//se l'indirizzo letto dal file è diversa dall'email del database
                                iter->second.updateAdress(infoProf[i]); //cambia l'indirizzo
                            }
                            break;
                        }
                    }
                }
            }
        }

        line_counter++;
    }
    fileIn.close();
    if (doDbWrite)
        dbProfsWrite();
    else
        throw std::invalid_argument(error);
}

///aggiornamento aule
enum {
    update_lab = 1,
    update_nameClassroom = 2,
    update_nSeats = 3,
    update_nSeatsExam = 4,
    update_drawingTable = 5,
    update_computer = 6,
    update_projector = 7,
    update_blackBoard = 8
};

///aggiorno le Classroom
void University::updateClassroom(const std::string &fin) {
    int i;
    char c;
    int nMatr = 0;
    bool doDbWrite = true;

    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file per aggiornamento aule \n");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::string error;
    std::vector<std::string> infoClassroom;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoClassroom = Parse::splittedLine(line, ';');
        bool canBeAnId = Parse::controlItCanBeAnId(infoClassroom[0], 3);
        std::stringstream ss(infoClassroom[0]);
        ss >> c >> nMatr;
        if (_version == 3) {
            if (std::count(line.begin(), line.end(), ';') != 7) {
                error.append("errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
        } else if (std::count(line.begin(), line.end(), ';') != 4) {
            error.append("errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        }
        if (infoClassroom[0].empty()) {
            error.append("manca il codice dell' aula alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        } else if (canBeAnId == false) {
            error.append("il primo campo alla riga " + std::to_string(line_counter) + " non puo' essere un codice Id di una aula \n");
            doDbWrite == false;
        } else if (_classroom.find(nMatr) == _classroom.end()) {//se non trovo il codice
            error.append("L'aula" + infoClassroom[0] + "non e' presente nel database \n");
            doDbWrite = false;
        }
        if (doDbWrite) {
            auto iter = _classroom.find(nMatr);//prendo la posizione della matricola
            for (i = 1;
                 i < infoClassroom.size(); i++) {//cerco i campi della riga del file passato che andranno aggiornati
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
                        case update_nSeatsExam : {
                            if (iter->second.getNExamSeats() !=
                                stoi(infoClassroom[i])) {//se la capienza dell'aula per gli esami letta da file non è uguale alla capienza dell'aula per gli esami del database
                                iter->second.updateNExamSeats(stoi(infoClassroom[i])); //cambia l'email
                            }
                            break;
                        }
                        case update_drawingTable: {
                            if (iter->second.getDrawingTable() !=
                                stoi(infoClassroom[i])) {//se il numero di tavoli da disegno è diverso da quello che c'è nel database
                                iter->second.updateDrawingTable(
                                        stoi(infoClassroom[i])); //cambia il numero di tavoli da disegno
                            }
                            break;
                        }
                        case update_computer: {
                            if (iter->second.getComputer() !=
                                stoi(infoClassroom[i])) {//se il numero di tavoli da disegno è diverso da quello che c'è nel database
                                iter->second.updateComputer(stoi(infoClassroom[i])); //cambia il numero di computer
                            }
                            break;
                        }
                        case update_projector: {
                            if (iter->second.getProjector() !=
                                stoi(infoClassroom[i])) {//se il numero di tavoli da disegno è diverso da quello che c'è nel database
                                iter->second.updateProjector(stoi(infoClassroom[i])); //cambia il numero di proiettori
                            }
                            break;
                        }
                        case update_blackBoard: {
                            if (iter->second.getBlackBoard() !=
                                stoi(infoClassroom[i])) {//se il numero di tavoli da disegno è diverso da quello che c'è nel database
                                iter->second.updateBlackBoard(stoi(infoClassroom[i])); //cambia il numero di lavagne
                            }
                            break;
                        }
                    }
                }
            }
        }
        line_counter++;
    }
    fileIn.close();
    if (doDbWrite)
        dbClassRoomWrite();
    else
        throw std::invalid_argument(error);
}

///inserisce un nuovo anno accademico ad un corso già esistente
std::vector<std::string> University::insertCourses(const std::string &fin) {
    std::vector<std::string> specificYearCourse;
    bool doDbWrite = true;
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
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        std::vector<std::string> specificYearCourse = Parse::splittedLine(line, ';');
        if (_courses.find(specificYearCourse[0]) ==
            _courses.end()) {//find mi restituisce literatore alla chiave inserita(IdCorso). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
            throw std::invalid_argument("IdCorso non presente");
        }
        auto course_with_given_id = _courses.find(specificYearCourse[0]);//iteratore al corso da aggiornare
        ///fillSpecificYearCourse mi aggiorna il vettore specificYearCourse aggiungendo le info dell'anno accademico precedente negli spazi vuoti
        if (!(course_with_given_id->second.courseOfTheYearIsEmpty())) {
            course_with_given_id->second.fillSpecificYearCourse(
                    specificYearCourse, line_counter);//passo l'intero vettore di stringhe by reference
        } else {
            //non esistono informazioni riguardo anni precedenti
            if (specificYearCourse.size() != 7) {
                throw DbException(
                        "non esistono informazioni riguardo anni precedenti da poter ereditare; manca info sui corsi raggruppati alla riga: " +
                        std::to_string(line_counter));
            } else {
                for (int i = 0; i < 7; i++) {
                    if (specificYearCourse[i].empty()) {
                        throw DbException(
                                "non esistono informazioni riguardo anni precedenti da poter ereditare; specificare tutti i campi alla riga: " +
                                std::to_string(line_counter));
                    }
                }
            }
        }

        acYear = specificYearCourse[1]; //anno accademico

        ///come per la readCourse, aggiorno la mappa _courses
        num_parallel_courses = stoi(specificYearCourse[3]);//numero di corsi in parallelo
        profSenzaQuadre = specificYearCourse[4].substr(1, specificYearCourse[4].size() -
                                                          2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
        std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre,
                                                                  num_parallel_courses);//divido i vari corsi in parallelo
        examData = specificYearCourse[5];//informazioni sull'esame
        examData = examData.substr(1, examData.size() - 2);//tolgo le { } che racchiudono le info degli esami
        splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
        idGrouped = Parse::SplittedGroupedID(specificYearCourse[6]);

        ///ricerca "anno-semestre" di questo corso
        std::string yy_semester;
        std::vector<int> studyCourse;
        for (int i = 1; i <= _studyCourse.size(); i++) {
            std::string res = _studyCourse.at(i).isInWhichSemester(specificYearCourse[0]);
            if (!res.empty()) {
                //ho trovato il suo corso di studi
                auto iterStudyCourse = _studyCourse.find(i); //prendo id del corso di studio associato
                studyCourse.push_back(iterStudyCourse->first);
                yy_semester = res;

                fillGroupedCourse(idGrouped, specificYearCourse[0], acYear, line_counter);

                ///controllo i corsi raggruppati (genera eccezione se errore)
                if (controlGroupedCoursesDifferentCds_Reciprocy(idGrouped, _courses.at(specificYearCourse[0]).getName(),
                                                                line_counter,
                                                                specificYearCourse[0]) == false)
                    doDbWrite = false;
            }
        }

        if (specificYearCourse[2] == "attivo") //se attivo o meno
            isActive = true;
        else {
            isActive = false;
            //se un corso viene aggiornato diventando "spento" in quell'anno allora per ogni corso di studio eseguo il metodo updateSemestersAndOffCourses
            //con il quale vedo per i diversi semestri se c'è quel determinato corso ora diventato "spento" e in tal caso:
            //- lo tolgo dai corsi attivi
            //- lo aggiungo tra i corsi spenti
            for (auto iterStudyCourse = _studyCourse.begin();
                 iterStudyCourse != _studyCourse.end(); iterStudyCourse++) {
                iterStudyCourse->second.updateSemestersAndOffCourses(specificYearCourse[0], specificYearCourse[1],
                                                                     _tempInfoNotActiveCoursesToWriteInTheDB);
            }
        }

        _courses.at(specificYearCourse[0]).addSpecificYearCourses(acYear, isActive, num_parallel_courses, profCorsoPar,
                                                                  splittedExamData, idGrouped, yy_semester,
                                                                  studyCourse, line_counter);
        line_counter++;
    }
    fileIn.close();

    ///controlli che posso fare solo dopo aver inserito le info da file
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        ///se c'è un gap tra anni prendo il precedente
        iterCourse->second.fillAcYearsEmpty();
        ///se il corso è stato disattivato non può essere riattivato
        iterCourse->second.notActive();
    }
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        ///controllo se i corsi raggruppati siano dello stesso semestre del principale
        iterCourse->second.sameSemesterGrouped(_courses);
    }

    ///tutto ok, posso aggiornare effettivamente db
    if (doDbWrite) {
        dbCourseNotActive();
        dbCourseWrite();
        dbStudyCourseWrite();
        std::cout << "comando -i:c eseguito correttamente" << std::endl;

    }

    return _errorStringUniversity;
}

///riscrive il database degli studenti
void University::dbStudsWrite() {
    std::fstream fout;
    fout.open("db_studenti.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterStud = _students.begin(); iterStud != _students.end(); iterStud++) {
        Student stud = _students.at(
                iterStud->first);//salvo in un oggetto Student temporaneo, l'intero oggetto puntato da iterStud
        if (_version == 2) {
            std::string otherInfo = stud.getOtherInfoString();
            fout << stud << ";" << otherInfo << std::endl;
        } else
            fout << stud
                 << std::endl;//grazie all'overload dell'operatore << scrivo su file l'oggetto stud(si rimanda all'overload dell'operatore in Student.cpp)
    }
    fout.close();

}

///riscrive il database dei professori
void University::dbProfsWrite() {

    std::fstream fout;
    fout.open("db_professori.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterProf = _professors.begin(); iterProf != _professors.end(); iterProf++) {
        Professor prof = _professors.at(iterProf->first);
        if (_version == 2) {
            std::string otherInfo = prof.getOtherInfoString();
            fout << prof << ";" << otherInfo << std::endl;
        } else
            fout << prof << std::endl;
    }
    fout.close();
}

///riscrive il database delle classroom
void University::dbClassRoomWrite() {

    std::fstream fout;
    fout.open("db_aule.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterClassRoom = _classroom.begin(); iterClassRoom != _classroom.end(); iterClassRoom++) {
        Classroom room = _classroom.at(iterClassRoom->first);
        if (_version == 3) {
            std::string otherInfo = room.getOthersInfo();
            fout << room << ";" << otherInfo << std::endl;
        } else
            fout << room << std::endl;
    }
    fout.close();
}

///riscrive il database dei corsi di studio
void University::dbStudyCourseWrite() {
    std::fstream fout;
    fout.open("db_corsi_studio.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterStudyCourse = _studyCourse.begin(); iterStudyCourse != _studyCourse.end(); iterStudyCourse++) {
        StudyCourse sC = _studyCourse.at(iterStudyCourse->first);
        fout << sC << std::endl;
    }
    fout.close();

}

///riscrive il database dei corsi
void University::dbCourseWrite() {

    std::fstream fout;
    fout.open("db_corsi.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        Course generalCourse = _courses.at(
                iterCourse->first);//prendo l'intero oggetto Course generale per scrivere su file la riga che inizia con "c"
        fout << generalCourse;
    }
    fout.close();

}

///legge le date delle sessioni in base all'anno
void University::readSessionAcYear() {
    std::ifstream fileIn("dateSessioni.txt");
    if (!fileIn.is_open()) {
        throw DbException("file dateSessioni.txt non esistente");
    }
    std::string line;
    while (std::getline(fileIn, line)) {
        std::vector<std::string> infoSessions = Parse::splittedLine(line, ';');
        int acStartYear = Parse::getAcStartYear(infoSessions[0]);
        std::string output_file_name = std::string("output_file");
        SessionYear sessionYear(infoSessions[0], infoSessions[1], infoSessions[2], infoSessions[3], output_file_name);
        _acYearSessions.insert(std::pair<int, SessionYear>(acStartYear, sessionYear));
    }
    fileIn.close();
}

///lettura delle indisponibilità dei professori in ogni anno
void University::readProfsNoAvailability() {
    std::ifstream fileIn("tutte_le_indisponibilita.txt");
    if (!fileIn.is_open()) {
        throw DbException("file tutte_le_indisponibilita non esistente");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> profAvailability;
    while (std::getline(fileIn, line)) {
        if (!line.empty()) {
            profAvailability = Parse::splittedLine(line, ';');
            int acYear = Parse::getAcStartYear(profAvailability[0]);
            int nMatr = Parse::getMatr(profAvailability[1]);
            //prima di inserire le indisponibilità devo controllare che abbia il professore
            if (_professors.find(nMatr) != _professors.end()) {
                for (int i = 2;
                     i < profAvailability.size(); i++) {//per il numero di periodi di indisponibilità del singolo prof
                    _professors.at(nMatr).setNoAvaibilities(acYear,
                                                            profAvailability[i]);//vado a settare l'indisponibilità del prof nella map _professor
                }
            }
        }
    }
    fileIn.close();
}

///setta il periodo delle sessioni
void University::setSessionPeriod(std::string &acYear, std::string &winterSession,std::string &summerSession, std::string &autumnSession) {
    std::string ac = acYear;
    int acStartYear = Parse::getAcStartYear(ac);
    std::string error;
    bool doDbWrite = true;
    try {
        controlCoerenceSessionDate(winterSession, summerSession,autumnSession,acStartYear);
    }catch(std::exception& err) {
        error.append(err.what());
        doDbWrite = false;
    }

    if (_acYearSessions.count(acStartYear) != 0) {
        _acYearSessions.erase(acStartYear);
    }
    //creo oggetto session da aggiungere
    std::string output_file_name = std::string("output_file"); //default per il costruttore
    try {
        SessionYear sessionYear(acYear, winterSession, summerSession, autumnSession, output_file_name);
        //popolo mappa in university
        _acYearSessions.insert(std::pair<int, SessionYear>(acStartYear, sessionYear));
    }catch(std::exception& err) {
        error.append(err.what());
        doDbWrite = false;
    }

    if(doDbWrite) {

        dbDateSessionsWrite();
    }else
        throw std::logic_error(error);
}
void University::controlCoerenceSessionDate(std::string winterSession, std::string summerSession,std::string  autumnSession,int acYear) {
    std::string error;
    bool isOk = true;
    try {
        controlOfASingleSessionPeriod("winter",winterSession,acYear);
    }catch(std::exception& err) {
        error.append(err.what());
        isOk = false;
    }
    try {
        controlOfASingleSessionPeriod("summer",summerSession,acYear);
    }catch(std::exception& err) {
        error.append(err.what());
        isOk = false;
    }
    try {
        controlOfASingleSessionPeriod("autumn",autumnSession,acYear);
    }catch(std::exception& err) {
        error.append(err.what());
        isOk = false;
    }
    if(isOk == false)
        throw std::logic_error(error);


}
void University::controlOfASingleSessionPeriod(std::string name , std::string sessionPeriod, int acStartYear) {

      Date startSessionDate(sessionPeriod.substr(0,10));
      Date endSessionDate(sessionPeriod.substr(11,10));
      std::string error;
      bool isOk = true;
      if(acStartYear != stoi(sessionPeriod.substr(0,4))-1){
          if(name == "winter"){
              error.append("la sessione invernale e' impostata per un anno diverso dall'anno chiesto via comando \n");
          } else if (name == "summer")
              error.append("la sessione estiva e' impostata per un anno diverso dall'anno chiesto via comando \n");
          else if (name == "autumn")
              error.append("la sessione autunnale e' impostata per un anno diverso dall'anno chiesto via comando \n");
          isOk = false;
      }
      if(startSessionDate > endSessionDate){
          if(name == "winter"){
              error.append("La data di fine sessione invernale e' inferiore a quella di inizio \n");
          } else if (name == "summer")
              error.append("La data di fine sessione estiva e' inferiore a quella di inizio \n");
          else if (name == "autumn")
              error.append("La data di fine sessione autunnale e' inferiore a quella di inizio \n");
          isOk = false;
      }
      if(isOk == false)
          throw std::logic_error(error);
}


///set indisponibilità professori in base all'anno
void University::setProfsNoAvailability(std::string acYear, const std::string &fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("file indisponibilita' dei professori non esistente \n");
    }
    std::string line;
    std::vector<std::string> profAvailability;
    int nMatr, year;
    bool doDbWrite = true;
    std::string error;
    year = Parse::getAcStartYear(acYear);
    int line_counter = 1;

    while (std::getline(fileIn, line)) {//fino alla fine del file leggo un rigo alla volta
        profAvailability = Parse::splittedLine(line, ';');
        bool canBeAnId = Parse::controlItCanBeAnId(profAvailability[0], 6);

        nMatr = Parse::getMatr(profAvailability[0]);//prendo la matricola
        std::string idProf = Parse::setId('d',6,nMatr);
        auto iterProf = _professors.find(nMatr);//punto al prof con matricola nMatr
        if(profAvailability[0].empty()){
            error.append("Non e' stata dichiarata la matricola del professore alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        }else if(canBeAnId == false){
            error.append("Il primo campo di informazioni alla riga " + std::to_string(line_counter)+ " non puo' essere un Id \n");
            doDbWrite = false;
        }else if(iterProf == _professors.end()){
            error.append("la matricola " + idProf + "alla riga" + std::to_string(line_counter) + " non e' presente nel database \n");
            doDbWrite = false;
        }else {
            //dovrò aggiornare l'indisponibilità di un prof per uno specifico anno accademico quindi cancello le
            //vecchie indisponibilità di uno specifico prof per uno specifico anno

            iterProf->second.noAvailabilityClear(year);
            for (int i = 1; i < profAvailability.size(); i++) {//per ogni campo della riga letta
                try {
                    _professors.at(nMatr).setNoAvaibilities(year,profAvailability[i]);//vado a settare uno dei periodi di indisponibilità del prof nella map _professor
                }catch(std::exception& err) {
                    std::string generalError = err.what();
                    error.append("in almeno uno dei periodi di indisponibilita' del prof con matricola: " + idProf + " alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
            }
        }
        line_counter++;
    }
    fileIn.close();
    if(doDbWrite)
       dbNoAvailabilityWrite();
    else
        throw std::invalid_argument(error);

}

///prende tutte le indisponibilità di tutti i prof
std::vector<std::string> University::allProfsNoAvailabilities() {
    std::vector<std::string> allProfsAvailabilities;
    for (auto iterProfs = _professors.begin(); iterProfs != _professors.end(); iterProfs++) {//per ogni professore
        int id = iterProfs->first;//prendo l'id
        std::vector<std::string> allProfAvailabilities = iterProfs->second.outputNoAvailabilities(
                id); //ritorna un vettore per anno accademico con tutte le indisponibilità per un prof
        for (int i = 0; i <
                        allProfAvailabilities.size(); i++) {//riempio il vettore di tutte le indisponibilità di ogni prof per ogni anno
            allProfsAvailabilities.push_back(allProfAvailabilities[i]);
        }
    }
    return allProfsAvailabilities;
}

///salva le date delle sessioni su file
void University::dbDateSessionsWrite() {

    std::fstream fout;
    fout.open("dateSessioni.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterAcYear = _acYearSessions.begin(); iterAcYear != _acYearSessions.end(); iterAcYear++) {
        SessionYear dateSessions = iterAcYear->second;
        fout << dateSessions << std::endl;
    }

    fout.close();

}

///scrittura indisponibilità professori di tutti gli anni
void University::dbNoAvailabilityWrite() {
    std::fstream fout;
    fout.open("tutte_le_indisponibilita.txt", std::fstream::out | std::fstream::trunc);
    std::vector<std::string> allprofsAvailabilities = allProfsNoAvailabilities();//ritorna le indisponibilità per tutti gli anni per prof già sottoforma di stringhe da salvare sul file.txt
    for (int i = 0;
         i < allprofsAvailabilities.size(); i++) {//scrivo su file le varie stringhe di allProfsAvailabilities
        fout << allprofsAvailabilities[i] << std::endl;
    }
    fout.close();
}

///organizza gli esami per l'anno accademico
void University::setExamDate(std::string acYear, std::string outputNameFile) {
    int startAcYear = Parse::getAcStartYear(acYear);
    int constraintRelaxParameter = 0;
    bool esito = false;
    if (controlDatabase(startAcYear) == false)
        throw DbException("");
    ///il ciclo sarà eseguito se le sessioni non sono ancora generate(result==false) e finchè ci saranno ancora vincoli da poter rilassare
    while (!esito && constraintRelaxParameter < 4) {
        //accedo all'anno accademico passato dal comando e genero le sessioni per un anno
        esito = _acYearSessions.at(startAcYear).generateNewYearSession(outputNameFile, _courses, _professors,
                                                                       _classroom,
                                                                       constraintRelaxParameter);
        constraintRelaxParameter++;
    }
    ///se le sessioni non possono essere generate nonostante i vincoli rilassati
    if (!esito) {
        std::cerr << "non è stato possibile generare le date d'esame nonostante i vincoli rilassati" << std::endl;
    }
    std::cout << "comando -g correttamente eseguito" << std::endl;

}

///controlla se i file sono coerenti
bool University::controlDatabase(int startAcYear) {
    ///controllo che i database non siano vuoti
    dataBaseIsEmpty(startAcYear);
    bool checkIsOK = true;
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        ///controllo che ogni corso in _courses esista in uno StudyCourse
        Course course = iterCourse->second;
        if (whatIsMyStudyCourse(course) == -1) {
            checkIsOK = false;
            std::cerr << "studio corso non esiste, da mod\n";
        }
    }
    for (auto iter = _studyCourse.begin(); iter != _studyCourse.end(); iter++) {
        ///controllo che ogni studyCourse abbia i corsi specificati
        StudyCourse sCourse = iter->second;
        std::vector<std::string> allCoursesOfThis = sCourse.getAllCoursesOfStudyCourse();
        for (int i = 0; i < allCoursesOfThis.size(); i++) {
            int occ = _courses.count(allCoursesOfThis[i]);
            if (occ == 0) {
                checkIsOK = false;
                std::cerr << "studio corso x ha quesyo corso che non esiste\n";
            }
        }
    }
    return checkIsOK;
}

///i database sono vuoti?
bool University::dataBaseIsEmpty(int startAcYear) {
    if (_professors.empty())
        throw InvalidDbException("Il database dei professori e' vuoto");
    else if (_students.empty())
        throw InvalidDbException("Il database degli studenti e' vuoto");
    else if (_courses.empty())
        throw InvalidDbException("Il database dei corsi e' vuoto");
    else if (_studyCourse.empty())
        throw InvalidDbException("Il database dei corsi di studio e' vuoto");
    else if (_acYearSessions.find(startAcYear) == _acYearSessions.end())
        throw InvalidDbException("non ci sono informazioni sulle sessioni per questo anno");
    else if (_acYearSessions.at(startAcYear).sessionsPeriodIsEmpty())
        throw InvalidDbException("Il database dei corsi di studio e' vuoto");
    ///controllo che gli studenti siano iscritti nei corsi di questo anno accademico
    for (auto iter = _courses.begin(); iter != _courses.end(); iter++) {
        int studentsEnrolled = iter->second.getThisYearCourse(startAcYear).getTotStudentsEnrolled();
        if (studentsEnrolled == 0)
            throw InvalidDbException("Il corso " + iter->second.getId() + " non ha studenti iscritti");
    }
    return false;
}

/// controllo che idGrouped NON siano corsi dello stesso CdS
bool University::controlGroupedCoursesDifferentCds_Reciprocy(std::vector<std::string> &idGrouped,
                                                             std::string nameCourse, int line_counter,
                                                             std::string idCourseToAddToIdGrouped){
    bool everIsOk = true;
    std::string error;
    std::vector<std::string> coursesToConsider(idGrouped);
    coursesToConsider.push_back(idCourseToAddToIdGrouped);
    ///devo controllare che i corsi raggruppati non siano dello stesso cds, reciprocamente --> 2 for
    for (int i = 0; i < coursesToConsider.size(); i++){
        int idStudyCourseOfThisCourse = whatIsMyStudyCourse(_courses.at(coursesToConsider[i]));
        if (idStudyCourseOfThisCourse != -1) {
            ///se c'è cDs, prendo tutti i corsi del cds e tutti gli coursesToConsider != questi ultimi
            std::vector<std::string> allCoursesOfCdSOfI = _studyCourse.at(idStudyCourseOfThisCourse).getAllCoursesOfStudyCourse();
            for (int j = 0; j < coursesToConsider.size(); j++) {
                bool same = false;
                if (coursesToConsider[i] != coursesToConsider[j]) {
                    ///se non è lo stesso faccio il controllo
                    ///cerco coursesToConsider[j] all'interno di allCoursesOfCdSOfI
                    auto found = std::find(allCoursesOfCdSOfI.begin(), allCoursesOfCdSOfI.end(), coursesToConsider[j]);
                    if (found != allCoursesOfCdSOfI.end()) {
                        ///se c'è dentro == stesso cds
                        everIsOk = false;
                        std::string settedId = Parse::setId('C', 3, idStudyCourseOfThisCourse);
                        //error.append("stesso corso di studio tra: " + nameCourse + "(corrispondente al corso:" +
                        //             idCourse + ") e " + idGrouped[j] + " alla riga: " + std::to_string(line_counter) +
                        //             ". Corso di studio non coerente: " + settedId + "\n");
                    }
                }
            }
        }
        ///se non c'è cds devo controllare sta cosa quando inserisco i cds
    }
    if(everIsOk == false){
        throw std::invalid_argument(error);
    }
    return everIsOk;
}

///verifica che non ci siano buchi negli id dei corsi nei corsi di studio
void University::thereIsAHoleInTheCoursesCodes() {
    ///controllo non ci siano buchi, se c'è mi ritorna la stringa non consecutiva che crea il buco
    std::vector<std::string> allCoursesCodesFromSemesters;
    std::vector<std::string> codesReturned;
    ///prendo tutti corsi di tutti i semestri di tutti i corsi di studio
    for (int i = 1; i <= _studyCourse.size(); i++) {
        codesReturned = _studyCourse.at(i).getAllCoursesOfStudyCourse();
        allCoursesCodesFromSemesters.insert(allCoursesCodesFromSemesters.end(), codesReturned.begin(),
                                            codesReturned.end());
    }
    ///for per controllare che le stringhe siano sequenziali
    std::sort(allCoursesCodesFromSemesters.begin(), allCoursesCodesFromSemesters.end());
    allCoursesCodesFromSemesters.erase(unique(allCoursesCodesFromSemesters.begin(), allCoursesCodesFromSemesters.end()),
                                       allCoursesCodesFromSemesters.end());
    for (int i = 1; i < allCoursesCodesFromSemesters.size(); i++) {
        checkDistance(allCoursesCodesFromSemesters[i - 1], allCoursesCodesFromSemesters[i]);
    }
}

///verifica la distanza tra due id successivi
void University::checkDistance(std::string &minor, std::string &major) {
    int size_min = minor.size();
    int size_maj = major.size();
    std::string error("File non corretto: le stringhe dei corsi non sono consecutive -> (" + major + ")");
    if (size_maj != size_min)
        throw std::invalid_argument("errore codice corso!");
    bool notEqual = false;
    int posNotEqual = 0;
    ///prendo la posizione in cui i codici iniziano a cambiare
    for (int j = 0; j < size_maj && !notEqual; j++) {
        if (major[j] != minor[j]) {
            notEqual = true;
            posNotEqual = j;
        }
    }
    //prendo le prime lettere diverse per la major e minor
    char lettMaj = major[posNotEqual];
    char lettMin = minor[posNotEqual];
    int distance = lettMaj - lettMin;
    //due possibili scenari:
    // 1) il primo carattere(numero) è uguale per entrambi i codici. Esempio: minor = "01ZZZZZ". major = "02AAAAA" o minor = "01AAABA". major = "01AAABB"
    // In questo caso se la distanza è 1 devo controllare che i successivi caratteri siano tutte A per major e Z per minor
    //2)il primo carattere(numero) è diverso per i due codici. Esempio: minor = "19ZZZZZ". major = "20AAAAA"
    //In questo caso se la distanza è 1 devo controllare che i successivi caratteri siano: 0 per major e 9 per minor, tutte A per major e Z per minor
    if (distance == 1) {
        for (int i = posNotEqual + 1; i < size_maj; i++) {
            if ((major[i] != 'A' || minor[i] != 'Z') && (major[i] != '0' || minor[i] != '9'))
                throw std::invalid_argument(error.c_str());//non consecutivi
        }
    } else {
        //se la distanza non è 1
        throw std::invalid_argument(error.c_str());//non consecutivi
    }
    ///se esco il check è andato bene, sono consecutivi
}


/*controllo la proprietà di reciprocità dei corsi raggruppati: se a ragg con c, allora c con a
///a (b,c,d)
//b (f,a,c,d)
///c (a,b,d)
///d (a,c,b)
void University::controlReciprocyGrouped() {
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        //a
        std::map<int, std::vector<std::string>> allGroupedCourse = iterCourse->second.getGroupedCourseFromAllYear();
        ///per ogni anno accademico del corso
        for (auto iterSpecific = allGroupedCourse.begin(); iterSpecific != allGroupedCourse.end(); iterSpecific++) {

            std::vector<std::string> groupedOfThisYear = allGroupedCourse.at(iterSpecific->first); //(b,c,d)
            groupedOfThisYear.push_back(iterCourse->second.getId()); //(b,c,d,a)
            ///controllo tutti nel vettore, meno l'ultimo che è lui stesso (a)
            for (int j = 0; j < groupedOfThisYear.size() - 1; j++) {
                //b
                std::map<int, std::vector<std::string>> allGroupedCourse2 = _courses.at(
                        groupedOfThisYear[j]).getGroupedCourseFromAllYear();
                std::string name = _courses.at(groupedOfThisYear[j]).getName();
                ///prendo anno accademico considerato
                std::vector<std::string> groupedOfThisYear2 = allGroupedCourse2.at(iterSpecific->first); //(a,c,d)
                groupedOfThisYear2.push_back(groupedOfThisYear[j]); //(a,c,d,b)
                ///rimuoviamo corso per corso -> alla fine dovrà rimanere 0
                for (int k = 0; k < groupedOfThisYear.size(); k++) {
                    auto pos = std::find(groupedOfThisYear2.begin(), groupedOfThisYear2.end(), groupedOfThisYear[k]);
                    if (pos == groupedOfThisYear2.end())
                        throw std::logic_error(
                                "Proprieta' di reciprocita' dei corsi non rispettata! Il seguente corso ha dei corsi raggruppati non in comune con gli altri: " +
                                name + ". Il codice assente e':" + groupedOfThisYear[k]);
                    groupedOfThisYear2.erase(pos);
                }
                if (groupedOfThisYear2.empty() == false) {
                    throw std::logic_error(
                            "Proprieta' di reciprocita' dei corsi non rispettata! Il seguente corso ha dei corsi raggruppati in piu': " +
                            name);
                }
            }
        }
    }
}*/

///tiene traccia delle info di corsi ormai spenti(per sessioni precedenti alla data di disattivazione)
void University::dbCourseNotActive() {
    std::fstream fout;
    fout.open("offCourses_db.txt", std::fstream::out | std::fstream::trunc);
    for (int i = 0; i < _tempInfoNotActiveCoursesToWriteInTheDB.size(); i++) {
        fout << _tempInfoNotActiveCoursesToWriteInTheDB[i] << std::endl;
    }
    fout.close();
}

///lettura delle info di corsi ormai spenti(per sessioni precedenti alla data di disattivazione)
void University::readCourseNotActive() {
    std::ifstream fileIn("offCourses_db.txt");
    if (!fileIn.is_open()) {
        throw DbException("file offCourses_db.txt non esistente");
    }
    std::string line;
    while (std::getline(fileIn, line)) {
        ///salvo l'intera riga del file
        _tempInfoNotActiveCoursesToWriteInTheDB.push_back(line);
        std::vector<std::string> token = Parse::splittedLine(line, ';');

        ///per il corso letto nella riga vado ad assegnare il semestre a cui era associato il corso
        if (_courses.find(token[0]) != _courses.end())
            _courses.at(token[0]).assignYY_Sem(token[1], token[2]);
    }
    fileIn.close();
}

///versione dei database con informazioni aggiuntive
void University::versioning(std::string newVersion) {
    int newVer = stoi(newVersion);
    if (checkVersioningRequest(newVer) == false)
        ///se è tornato false, perchè newVersion == _version, mi devo fermare


    ///se tutto ok, eseguiamo cambiamento
    if (newVer == 2 && _version == 1) {
        renameOldDataBase(newVer);
        //studenti e professori
        dbStudsWrite();
        dbProfsWrite();
        _version = newVer;
    }
    if (newVer == 3 && _version == 2) {
        renameOldDataBase(newVer);
        //aule
        dbClassRoomWrite();
        _version = newVer;
    }
    writeVersion(newVer);

}

///rename del database con _old prima dell'estensione .txt
bool University::renameOldDataBase(int version) {
    if (version == 2) {
        //rename student e professor database
        int result;
        char oldname[] = "db_studenti.txt";
        char newname[] = "db_studenti_old.txt";
        result = rename(oldname, newname);
        if (result != 0)
            throw InvalidDbException("file db_studenti.txt non rinominato adeguatamente");

        //rename student e professor database
        int result2;
        char oldname2[] = "db_professori.txt";
        char newname2[] = "db_professori_old.txt";
        result2 = rename(oldname2, newname2);
        if (result2 != 0)
            throw InvalidDbException("file db_professori.txt non rinominato adeguatamente");
    }
    if (version == 3) {
        //rename student e professor database
        int result;
        char oldname[] = "db_aule.txt";
        char newname[] = "db_aule_old.txt";
        result = rename(oldname, newname);
        if (result != 0)
            throw InvalidDbException("file db_aule.txt non rinominato adeguatamente");
    }
    return false;
}

void University::addStudyPlan(std::string fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento nuovi piani di studio \n");
    }
    std::string line;
    std::string error;
    bool doDbWrite = true;
    int line_counter = 1;
    while (std::getline(fileIn, line)) {
        char c;
        int id = 0;
        bool isOk = true;
        std::vector<int> posSemiColon = Parse::posSemiColon(line);
        ///controlli sul primo campo di informazioni, matricola
        std::string matr = line.substr(0, posSemiColon[0]);
        bool canBeAnId = Parse::controlItCanBeAnId(matr,6);
        if(matr.empty()){
            error.append("Non e' stata dichiarata la matricola dello studente al rigo: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
            isOk = false;
        } else if(canBeAnId == false){
            error.append("Il primo campo di informazioni alla riga " + std::to_string(line_counter) + " non puo' essere una matricola di uno studente \n");
            doDbWrite = false;
            isOk = false;
        } else
        {
            std::stringstream ss(matr);
            ss >> c >> id;
        }
        ///controllo sul secondo campo di informazioni, anno accademico
        std::string acYearRegistration = line.substr(posSemiColon[0]+1, posSemiColon[1]-posSemiColon[0] - 1);
        bool canBeAnAcYear = Parse::controlItCanBeAnAcYear(acYearRegistration);
        if(acYearRegistration.empty()){//se il campo è vuoto
            error.append("Non e' stato dichiarato l'anno accademico al rigo: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
            isOk = false;
        } else if(canBeAnAcYear == false && isOk){ //se non è vuoto controllo che possa essere un anno accademico
            error.append("Anno accademico non valido al rigo: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
            isOk = false;
        }
        if(isOk) {
            std::size_t pos = line.find('}');
            std::string allCourses = line.substr(posSemiColon[1] + 2, pos - (posSemiColon[1]+2));
            Student &stud = _students.at(id);
            std::vector<std::string> courses = Parse::splittedLine(allCourses, ';');
            /// mi assicuro che lo studente non abbia già un piano di studio(posso cambiarlo solo con la update)
            try {
                stud.addStudyPlanPerStudent(acYearRegistration, courses);
            }catch (std::invalid_argument &err) {
                std::string generalError = err.what();
                error.append(generalError + ". Controllare riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            try {
                registerStudentsToSpecificYearCourses(courses, stud, acYearRegistration, line_counter);
            } catch (std::invalid_argument &err) {
                error.append(err.what());
                doDbWrite = false;
            }
        }
        line_counter++;

    }
    fileIn.close();
    if(doDbWrite) {
        dbStudyPlanWrite();
    }else{
        throw std::invalid_argument(error);
    }
}

void University::dbStudyPlanWrite() {
    std::fstream fout;
    fout.open("db_piano_studi.txt", std::fstream::out | std::fstream::trunc);
    for (auto iterStud = _students.begin(); iterStud != _students.end(); iterStud++) {
        Student stud = _students.at(
                iterStud->first);//salvo in un oggetto Student temporaneo, l'intero oggetto puntato da iterStud
        if (stud.studyPlanIsEmpty() == false) {
            std::string otherInfo = stud.getOtherInfoString();
            fout << "s" << std::setfill('0') << std::setw(6) << stud.getId() << ";" << stud.getYearRegistration() << "-"
                 << stud.getYearRegistration() - 1 << ";" << stud.getPlanStudyCourseString() << std::endl;
        }
    }
    fout.close();
}

void University::readStudyPlan() {
    std::ifstream fileIn("db_piano_studi.txt");
    if (!fileIn.is_open()) {
        throw DbException("file db_piano_studi.txt non esistente \n");
    }
    std::string line;
    int line_counter = 1;
    while (std::getline(fileIn, line)) {
        std::string matr = line.substr(0, 7);
        std::string acYearRegistration = line.substr(8, 9);
        std::size_t pos = line.find('}');
        std::string allCourses = line.substr(19, pos - 19);
        std::stringstream ss(matr);
        char c;
        int id;
        ss >> c >> id;
        Student &stud = _students.at(id);
        std::vector<std::string> courses = Parse::splittedLine(allCourses, ';');
        _students.at(id).addStudyPlanPerStudent(acYearRegistration, courses);
        registerStudentsToSpecificYearCourses(courses, stud, acYearRegistration, line_counter);
        line_counter++;
    }
    fileIn.close();
}

///CAPIRE SE L'ANNO SI PUO' AGGIORNARE PURE
void University::updateStudyPlan(const std::string& fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file aggiornamento piani di studio \n");
    }
    std::string line;
    int line_counter = 1;
    std::string error;
    bool doDbWrite = true;
    while (std::getline(fileIn, line)) {
        char c;
        int id = 0;
        bool isOk = true;
        std::vector<int> posSemiColon = Parse::posSemiColon(line);
        ///controlli sul primo campo di informazioni, matricola
        std::string matr = line.substr(0, posSemiColon[0]);
        bool canBeAnId = Parse::controlItCanBeAnId(matr,6);
        if(matr.empty()){
            error.append("Non e' stata dichiarata la matricola dello studente al rigo: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
            isOk = false;
        } else if(canBeAnId == false){
            error.append("Il primo campo di informazioni alla riga " + std::to_string(line_counter) + " non puo' essere una matricola di uno studente \n");
            doDbWrite = false;
            isOk = false;
        } else
        {
            std::stringstream ss(matr);
            ss >> c >> id;
        }
        ///controllo sul secondo campo di informazioni, anno accademico
        std::string acYearRegistration = line.substr(posSemiColon[0]+1, posSemiColon[1]-posSemiColon[0] - 1);
        bool canBeAnAcYear = Parse::controlItCanBeAnAcYear(acYearRegistration);
        if(acYearRegistration.empty()){//se il campo è vuoto
            error.append("Non e' stato dichiarato l'anno accademico al rigo: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
            isOk = false;
        } else if(canBeAnAcYear == false && isOk){ //se non è vuoto controllo che possa essere un anno accademico
            error.append("Anno accademico non valido al rigo: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
            isOk = false;
        }
        if(isOk) {
            std::size_t pos = line.find('}');
            std::string allCourses = line.substr(posSemiColon[1] + 2, pos - (posSemiColon[1]+2));
            Student &stud = _students.at(id);
            stud.clearStudyPlan();
            std::vector<std::string> courses = Parse::splittedLine(allCourses, ';');
            /// mi assicuro che lo studente non abbia già un piano di studio(posso cambiarlo solo con la update)
            try {
                stud.addStudyPlanPerStudent(acYearRegistration, courses);
            }catch (std::invalid_argument &err) {
                std::string generalError = err.what();
                error.append(generalError + ". Controllare riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            try {
                registerStudentsToSpecificYearCourses(courses, stud, acYearRegistration, line_counter);
            } catch (std::invalid_argument &err) {
                error.append(err.what());
                doDbWrite = false;
            }
        }
        line_counter++;
    }
    fileIn.close();
    if(doDbWrite)
        dbStudyPlanWrite();
    else{
        throw std::invalid_argument(error);
    }
}

std::vector<std::string> University::insertStudentsGrades(std::string fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento voti");
    }
    ///folder/<id_corso>_<data_appello>(_[*]).csv
    std::string base_filename = fin.substr(fin.find_last_of("/\\") + 1);
    std::string idCorso = base_filename.substr(0, 7);
    std::string appealDate = base_filename.substr(8, 10);
    //controllo che il corso esista
    if (_courses.find(idCorso) == _courses.end())
        throw std::invalid_argument("il seguente corso non esiste" + idCorso + "impossibile assegnare i voti");
    Course &corso = _courses.at(idCorso);
    //controllo che la data esista
    corso.controlAppeal(appealDate);
    int appealYear = stoi(appealDate.substr(0, 4));
    std::string line;
    while (std::getline(fileIn, line)) {
        std::string idmatr = line.substr(0, 7);
        int matr = Parse::getMatr(idmatr);
        int mark = stoi(line.substr(8, 9));
        //controllo che lo studente esista nel database
        if (_students.find(matr) == _students.end())
            throw InvalidDbException("lo studente " + idmatr + "non esiste nel database");
        Student &stud = _students.at(matr); //preso l'istanza dello studente di cui si parla
        int acYear = stud.getYearRegistration();
        _courses.at(idCorso).modifyStudentAsPassedToSpecYearCourse(acYear, stud, appealYear, mark, appealDate);

    }
    fileIn.close();
    dbAppealsWrite();
    return _errorStringUniversity;
}

void University::registerStudentsToSpecificYearCourses(std::vector<std::string> &courses, Student &stud,std::string acYearRegistration, int line_counter) {
    int acStartYear = Parse::getAcStartYear(acYearRegistration);
    bool thereIsNotAnError = true;
    std::string error;

    try {
        controlUnicity(courses, line_counter);
    }catch (std::logic_error &err) {
        error.append(err.what());
        thereIsNotAnError = false;
    }
    for (int i = 0; i < courses.size(); i++) {
        if (_courses.find(courses[i]) == _courses.end()) {
            error.append("Alla riga " + std::to_string(line_counter) + " il corso con codice: " + courses[i] + " non esiste\n");
            thereIsNotAnError = false;
        }else{
            try {
                _courses.at(courses[i]).registerStudentsToSpecificYear(acStartYear, stud);
            } catch (InvalidDbException &err) {
                std::string generalError = err.what();
                error.append(generalError + ". Controllare alla riga: " + std::to_string(line_counter) + "\n");
                thereIsNotAnError = false;
            }
        }
    }
    if (thereIsNotAnError == false) {
        throw std::invalid_argument(error);
    }
}
void University::controlUnicity(std::vector<std::string>& controlUnicityCourses, int line_counter) {
      std::string error;
      bool isOk = true;
      bool pop = false;
      for(int i = 0; i<controlUnicityCourses.size(); i++) {
          if(pop) {
              pop = false;
              i = 0;
          }
          int numCourse = count(controlUnicityCourses.begin(), controlUnicityCourses.end(), controlUnicityCourses[i]);
          ///se presente più volte
          if (numCourse != 1){
              error.append("Il corso con codice: " + controlUnicityCourses[i] + " e' presente piu' volte alla riga: " + std::to_string(line_counter)+ "\n");
              isOk = false;
              ///se questo appello è presente piu' volte non lo dobbiamo più considerare nel controllo!!
              ///per il numero di volte che è stato trovato lo vado a togliere, lo lascio una sola volta
              std::string courseToPop = controlUnicityCourses[i];
              for(int j = 0; j<numCourse-1; j++){
                  auto pos = find(controlUnicityCourses.begin(), controlUnicityCourses.end(), courseToPop);
                  controlUnicityCourses.erase(pos);
              }
              pop = true;
          }
      }
      if(isOk == false)
          throw std::logic_error(error);
}


void University::dbAppealsWrite() {
    std::fstream fout;
    fout.open("db_appelli.txt", std::fstream::out | std::fstream::trunc);
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        std::vector<std::string> acYearAppeals = iterCourse->second.getAcYearStudExam();
        for (int i = 0; i < acYearAppeals.size(); i++) {
            fout << acYearAppeals[i] << std::endl;
        }
    }
    fout.close();
}

void University::readPassedAppeals() {
    std::ifstream fileIn("db_appelli.txt");
    if (!fileIn.is_open()) {
        throw DbException("file db_appelli.txt non esistente");
    }
    std::string line;

    while (std::getline(fileIn, line)) {
        std::vector<std::string> appeal = Parse::splittedLine(line, ';');
        std::string idCorso = appeal[0];
        std::string acYear = appeal[1];
        std::string appealDate = appeal[2];
        int pos = appeal[3].find(']');
        std::string allStudPassedExam = appeal[3].substr(1, pos - 1);

        _courses.at(idCorso).assignStudToAppealPerYear(acYear, appealDate,
                                                       allStudPassedExam);///cambio _grade, _passed;appealPassed
    }
}

void University::readAllExamAppeals() {
    std::ifstream fileIn("allExamAppealsDb.txt");
    if (!fileIn.is_open()) {
        throw DbException("file allExamAppealsDb.txt non esistente");
    }
    std::string line;
    while (std::getline(fileIn, line)) {
        std::vector<std::string> appeals = Parse::splittedLine(line, ';');
        std::string idCorso = appeals[0];
        std::string acYear = appeals[1];
        std::string appealsSession = appeals[2];
        std::vector<std::string> infoEachSession = Parse::splittedLine(appeals[2], '%');
        //per ogni sessione
        for (int i = 0; i < infoEachSession.size(); i++) {
            //tolgo le quadre
            std::vector<int> posSquareBrackets = Parse::posSquareBrackets(infoEachSession[i]);
            //prendo la sessione
            std::string session = infoEachSession[i].substr(0, posSquareBrackets[0]);
            //prendo le info di tutti gli appelli
            std::string appealsPerSessionString = infoEachSession[i].substr(posSquareBrackets[0] + 1,
                                                                            posSquareBrackets[1] -
                                                                            posSquareBrackets[0] - 1);
            std::vector<int> posCurlyBrackets = Parse::posCurlyBrackets(appealsPerSessionString);
            std::vector<std::string> appealInfo;
            //prendo le info per ogni appello per ogni sessione
            for (int j = 0; j < posCurlyBrackets.size(); j = j + 2) {
                appealInfo.push_back(appealsPerSessionString.substr(posCurlyBrackets[j] + 1,
                                                                    posCurlyBrackets[j + 1] - posCurlyBrackets[j] - 1));
            }
            assignInfoAppealPerSession(acYear, idCorso, session, appealInfo);
        }
    }
    fileIn.close();
}

void University::writeVersion(int version) {
    std::fstream fout;
    fout.open("versione.txt", std::fstream::out | std::fstream::trunc);
    fout << version;
    fout.close();
}

void University::readAllMinDistanceRequest() {
    std::ifstream fileIn("tutte_le_richieste.txt");
    if (!fileIn.is_open()) {
        throw DbException(
                "file tutte_le_richieste.txt non esistente. Non e' ancora stato usato il comando set_min_distance");
    }
    std::string line;
    while (std::getline(fileIn, line)) {
        std::vector<std::string> infoRequest = Parse::splittedLine(line, ';');
        int acStartYear = Parse::getAcStartYear(infoRequest[0]);
        std::string matrString = infoRequest[1];
        std::string idCorso = infoRequest[2];
        int distance = stoi(infoRequest[3]);
        std::string matr_idC(matrString + "_" + idCorso);
        _acYearSessions.at(acStartYear).addProfGap(matr_idC, distance);
    }
}

void University::setMinDistance(std::string acYear, std::string name) {
    int year = Parse::getAcStartYear(acYear);
    std::fstream fin;
    fin.open(name, std::ios::in);
    if (!fin.is_open()) {
        std::string base_filename = name.substr(name.find_last_of("/\\") + 1);
        throw std::invalid_argument("Il file" + base_filename + "non esiste");
    }
    std::string line;
    std::string error;
    bool doDbWrite = true;
    int line_counter = 0;
    while (std::getline(fin, line)) {
        std::vector<std::string> infoRequest = Parse::splittedLine(line, ';');
        int matr = Parse::getMatr(infoRequest[0]);
        std::string matrString = infoRequest[0];
        std::string idCorso = infoRequest[1];
        int distance = stoi(infoRequest[2]);
        if (distance >= 14) {
            //controllo che il corso esista
            if (_courses.find(idCorso) == _courses.end()) {
                error.append("il corso " + idCorso + "alla riga: " + std::to_string(line_counter) + " non esiste nel database dei corsi \n");
                doDbWrite = false;
            }
            //controllo che quel prof esista
            if (_professors.find(matr) == _professors.end()) {
                error.append("il professore " + matrString + "alla riga: "+ std::to_string(line_counter) + " non esiste nel database dei professori \n");
                doDbWrite = false;
            }
            //controllo che quel prof abbia quel corso in quell'anno accademico
            if(doDbWrite) {
                if (_courses.at(idCorso).profHaveThisCourse(matr, year) == false) {
                    error.append("il professore " + matrString + " alla riga: " + std::to_string(line_counter) + " non è stato assegnato al corso " + idCorso + " nel " +  acYear + "\n");
                    doDbWrite = false;
                }
            }
            //se la distanza è minore di 14 ignoro la riga del file
            std::string matr_idC(matrString + "_" + idCorso);
            if (_acYearSessions.count(year) == 0) {
                error.append("Bisogna prima settare i periodi delle sessioni per il " + acYear + "\n");
                doDbWrite = false;
            }
            if(doDbWrite)
                _acYearSessions.at(year).addProfGap(matr_idC, distance);
        }
        line_counter++;
    }
    if(doDbWrite)
        minDistanceRequestWrite();
    else
        throw std::invalid_argument(error);
}

void University::minDistanceRequestWrite() {
    std::fstream fout;
    //anno accademico;idProf;codiceCorso;distanza
    fout.open("tutte_le_richieste.txt", std::fstream::out | std::fstream::trunc);
    for (auto iterSession = _acYearSessions.begin(); iterSession != _acYearSessions.end(); iterSession++) {
        std::vector<std::string> profsOfSpecificSessionYear = iterSession->second.getProfsOfGapProfsString();
        for (int i = 0; i < profsOfSpecificSessionYear.size(); i++)
            fout << profsOfSpecificSessionYear[i] << std::endl;
    }
    fout.close();
}

bool University::checkVersioningRequest(int newVer) {
    ///controllo input
    if (newVer >= 3 || newVer < 1) {
        throw std::invalid_argument("versioning non valido!");
    } else if (newVer == _version) {
        ///se uguali non faccio nulla, mi devo fermare
        return false;
    } else if (newVer - _version > 1) {
        ///salto NON incrementale in AVANTI!!!
        for (int i = _version + 1; i < newVer; i++) {
            versioning(std::to_string(i));
        }
        ///ora tutto ok, si può proseguire
        return true;
    } else if (newVer < _version) {
        ///salto all'INDIETRO!!!
        revertChanges(newVer);
        return true;
    }
    return true;
}

void University::revertChanges(int newVersion) {
    ///in pratica oldDatabase deve tornare a essere current Database!
    if (_version - newVersion > 1) {
        ///salto NON incrementale!!!
        /// devo portare prima 3 a 2 e poi 2 a 1 (unico caso possibile!)
        revertChanges3to2();
        revertChanges2to1();
    } else {
        ///salto all'indietro incrementale
        /// 2 casi: "2 -> 1" oppure "3->2"
        if (_version == 2) {
            ///caso 2->1
            revertChanges2to1();
        }
        if (_version == 3) {
            ///caso 3->2
            revertChanges3to2();
        }
    }
}

void University::revertChanges2to1() {
    ///rimuove il nuovo db degli studenti
    if (remove("db_studenti.txt") != 0)
        perror("Error deleting file");
    else
        puts("File successfully deleted");

    ///rimuove il nuovo db dei professori
    if (remove("db_professori.txt") != 0)
        perror("Error deleting file");
    else
        puts("File successfully deleted");

    int result;
    char oldname[] = "db_studenti_old.txt";
    char newname[] = "db_studenti.txt";
    result = rename(oldname, newname);
    if (result != 0)
        throw InvalidDbException("file db_studenti_old.txt non rinominato adeguatamente");

    char oldname2[] = "db_professori_old.txt";
    char newname2[] = "db_professori.txt";
    result = rename(oldname2, newname2);
    if (result != 0)
        throw InvalidDbException("file db_studenti_old.txt non rinominato adeguatamente");
}

void University::revertChanges3to2() {
    ///rimuove il nuovo db delle aule
    if (remove("db_aule.txt") != 0)
        perror("Error deleting file");
    else
        puts("File successfully deleted");

    int result;
    char oldname2[] = "db_aule_old.txt";
    char newname2[] = "db_aule.txt";
    result = rename(oldname2, newname2);
    if (result != 0)
        throw InvalidDbException("file db_studenti_old.txt non rinominato adeguatamente");

}

void University::fillGroupedCourse(std::vector<std::string> &idGroupedLetti, std::string &idCourse, std::string &acYear,
                                   int line) {
    for (int i = 0; i < idGroupedLetti.size(); i++) {
        if (_courses.count(idGroupedLetti[i]) != 0 && _coursesGrouped.count(acYear+"-"+idGroupedLetti[i])==0){
            ///se il corso raggruppato già esiste ed è vuoto, POSSO QUINDI FILLARLO
            SpecificYearCourse &specificYY = _courses.at(idGroupedLetti[i]).getThisYearCourseReference(
                    Parse::getAcStartYear(acYear));//corso per un anno specifico
            specificYY.assignGrouped(idGroupedLetti, idCourse, idGroupedLetti[i]);
        }
        if(_coursesGrouped.count(acYear+"-"+idGroupedLetti[i])!=0){
            ///se si sa già quali sono i raggruppati suoi, le info devono essere coerenti
            std::vector<std::string> groupedSicuri = _coursesGrouped.at(acYear+"-"+idGroupedLetti[i]);
            //1)devono avere lo stesso numero di grouped
            if(groupedSicuri.size()!=idGroupedLetti.size())
                throw std::exception();
            //2)devono essere li stessi
            for(int i=0; i<groupedSicuri.size(); i++){
                auto pos = std::find(idGroupedLetti.begin(), idGroupedLetti.end(), groupedSicuri[i]);
                if(pos == idGroupedLetti.end())
                    throw std::exception();
            }
        }
        ///riempio la mappa, per aiutarmi nei prossimi corsi inseriti DA FILLARE (che ancora non sono stati dichiarati)
        std::vector<std::string> grouped(idGroupedLetti);
        grouped.push_back(idCourse); ///!!!!!
        _coursesGrouped.insert(std::pair<std::string, std::vector<std::string>>(acYear+"-"+idGroupedLetti[i], grouped));
    }
    ///riempiola mappa anche per il corso che sto leggendo,per completezza
    std::vector<std::string> grouped(idGroupedLetti);
    _coursesGrouped.insert(std::pair<std::string, std::vector<std::string>>(acYear+"-"+idCourse, idGroupedLetti));
}

void University::assignInfoAppealPerSession(std::string acYear, std::string idCorso, std::string session,
                                            std::vector<std::string> appealInfo) {
    std::vector<Date> appealDate;
    int numSlot = _courses.at(idCorso).getExamSlotPerYear(acYear);
    int startAcYear = Parse::getAcStartYear(acYear);
    std::vector<int> allProfsPerYearCourse = _courses.at(idCorso).getProfsPerYear(acYear);
    for (int i = 0; i < appealInfo.size(); i++) {
        std::vector<std::string> infoOfSingleAppeal = Parse::splittedLine(appealInfo[i], ',');
        ///prendo le date degli appelli per quella sessione
        appealDate.emplace_back(infoOfSingleAppeal[0]);
        ///segno l'esame nei professori del corso(appello,ora di inizio,numero di slot)
        assignAppealsToProf(idCorso, infoOfSingleAppeal[0], stoi(infoOfSingleAppeal[1]), numSlot,
                            allProfsPerYearCourse);
        ///segno l'esame nelle aule (appello, ora di inizio, numero di slot)
        assignAppealsToClassroom(infoOfSingleAppeal[0], stoi(infoOfSingleAppeal[1]), infoOfSingleAppeal[2], numSlot);
        ///segno l'esame nel calendario (appello, ora di inizio, numero di slot, corso)
        Course &course = _courses.at(idCorso);
        _acYearSessions.at(startAcYear).assignAppealsToCalendar(infoOfSingleAppeal[0], stoi(infoOfSingleAppeal[1]),
                                                                course, numSlot);
    }
    ///salvo le date dell'appello in  _howManyTimesIAmAssignedInASession
    _courses.at(idCorso).assignAppealToSpecificYear(acYear, session, appealDate);
}

void University::assignAppealsToProf(std::string idCorso, std::string appeal, int startHour, int numSlots,
                                     std::vector<int> allProfsPerYearCourse) {
    for (int i = 0; i < allProfsPerYearCourse.size(); i++) {
        for (int num_slot = 0; num_slot < numSlots; num_slot++) {
            _professors.at(allProfsPerYearCourse[i]).addNewExam(appeal, startHour + (num_slot * 2), idCorso);
        }
    }
}

void University::assignAppealsToClassroom(std::string appeal, int startSlotHour, std::string classrooms, int numSlot) {
    std::vector<std::string> allClassrooms = Parse::splittedLine(classrooms, '|');
    Date appealDate(appeal);
    for (int i = 0; i < allClassrooms.size(); i++) {
        _classroom.at(stoi(allClassrooms[i])).setDisavailability(appealDate, startSlotHour, numSlot);
    }
}

void University::requestChanges(std::string acYear, std::string fin) {
    //<id_corso>;<numero_sessione>;<numero_appello>;<direzione_spostamento>;<num_settimane>
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw DbException("Errore apertura del file per le richieste del cambio data esami");
    }
    std::string line;
    std::string error;
    while (std::getline(fileIn, line)) {
        std::vector<std::string> infoChanges = Parse::splittedLine(line, ';');
        std::string idCourse = infoChanges[0];
        int numSession = stoi(infoChanges[1]);
        int numAppeal = stoi(infoChanges[2]);
        char shift = infoChanges[3][0];
        int numWeeks = stoi(infoChanges[4]);
    }

}

void University::ifThereAreAlreadyCoursesFillYYSemesterVar(StudyCourse &sCourse) {
    std::vector<std::string> allCoursesOfThis = sCourse.getAllCoursesOfStudyCourse();
    for (int i = 0; i < allCoursesOfThis.size(); i++) {
        ///i corsi ancora non sono stati letti oppure quel corso non esiste ancora se == 0
        if (_courses.count(allCoursesOfThis[i]) != 0) {
            ///il corso già esiste
            Course courseToConsider = _courses.at(allCoursesOfThis[i]);
            std::string yy_semester = sCourse.isInWhichSemester(courseToConsider.getId());
            ///update di yy_sem in tutti gli anni accademici...
            courseToConsider.updateYYSemesterInAllSpecYearCourse(yy_semester);
        }
    }
}

int University::whatIsMyStudyCourse(Course &courseToConsider) {
    std::string yy_semester;
    for (auto iter = _studyCourse.begin(); iter != _studyCourse.end(); iter++) {
        StudyCourse sCourse = iter->second;
        yy_semester = sCourse.isInWhichSemester(courseToConsider.getId());
        if (!yy_semester.empty())
            return iter->first;
    }
    return -1;
}

bool University::controlAGAINGroupedCoursesDifferentCds_Reciprocy() {
    for(auto iter = _coursesGrouped.begin(); iter!=_coursesGrouped.end(); iter++){
        if(!controlGroupedCoursesDifferentCds_Reciprocy(iter->second,"",0,iter->first))
            return false;
    }
    return true;
}




































































