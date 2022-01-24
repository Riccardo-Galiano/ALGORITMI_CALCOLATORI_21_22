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

        if (_version == 1) {
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
    _version = std::stoi(version);
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

        if (_version == 2) {
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

        if (_version == 2) {
            _classrooms.insert(std::pair<int, Classroom>(nCod, Classroom(nCod, InteraClasse[1], InteraClasse[2],
                                                                         std::stoi(InteraClasse[3]),
                                                                         std::stoi(InteraClasse[4]),
                                                                         std::stoi(InteraClasse[5]),
                                                                         std::stoi(InteraClasse[6]),
                                                                         std::stoi(InteraClasse[7]),
                                                                         std::stoi(InteraClasse[8]))));

        } else
            _classrooms.insert(std::pair<int, Classroom>(nCod, Classroom(nCod, InteraClasse[1], InteraClasse[2],
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
                                                           Course(interoCorso[1], interoCorso[2], Parse::checkedStoi(interoCorso[3],"Errore numero cfu."),
                                                                  Parse::checkedStoi(interoCorso[4],"Errore ore di lezione."), Parse::checkedStoi(interoCorso[5],"Errore ora di esercitazione."),
                                                                  Parse::checkedStoi(interoCorso[6],"Errore ore di laboratorio."))));

        } else if (interoCorso[0] == "a") {//se la riga letta inizia con "a" devo inserire le info di un anno accademico per un corso generale letto
            specificYearCourse = interoCorso;//per una maggiore comprensione nella lettura del codice
            acYear = specificYearCourse[1]; //anno accademico
            if (specificYearCourse[2] == "attivo") //se attivo o meno
                isActive = true;
            else
                isActive = false;
            ///numero di corsi in parallelo
            num_parallel_courses = Parse::checkedStoi(specificYearCourse[3],"Errore numero di corsi parallelo.");
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

        for (i = 0; i < posSem.size() - 1; i = i + 2) {//metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
            int posStart = posSem[i] + 1, len = posSem[i + 1] - posSem[i] - 1;
            semestri.push_back(InteroCorsoDiStudi[2].substr(posStart,
                                                            len));//salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato
        }
        ///leggo i semestri spenti.
        std::string corsiSpentiSenzaQuadre;
        ///è possibile leggere i corsi spenti solo se questi, leggendo da database, sono presenti
        ///e quindi solo se il vettore InteroCorsoDiStudi presenta un quarto campo
        if (InteroCorsoDiStudi.size() == 4) {
            corsiSpentiSenzaQuadre = InteroCorsoDiStudi[3].substr(1, InteroCorsoDiStudi[3].size() - 2);    //salvo la stringa senza le quadre
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
    //apro il file in lettura
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("Errore apertura file di inserimento nuovi studenti \n");
    }
    bool doDbWrite = true;
    std::string error;
    int line_counter = 1;
    std::string line; //stringa di appoggio per l'intera riga del file
    std::vector<std::string> infoStud; //vettore di stringhe che accoglierà il ritorno della funzione splittedLine
    while (std::getline(fIn, line)) {
        ///se la riga letta è vuota passo alla successiva, altrimenti la uso
        if(line.empty() == false) {
            infoStud = Parse::splittedLine(line, ';');
            //calcolo la matricola del nuovo studente
            int matr = getNewStudentId();
            //controllo che il formato file sia corretto: 6 campi per la versione aggiuntiva
            if (_version == 2) {
                if (infoStud.size() != 6) {
                    error.append("Errore formato file studenti alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
                if (Parse::controlFieldsVectorAreEmpty(infoStud)) {
                    error.append("Uno dei campi di informazione per l'aggiunta degli studenti e' vuoto alla riga: " +
                                 std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                } else if (infoStud.size() == 6) {
                    //Se non ci sono campi vuoti e il formato è esatto inserisco le informazioni nella struttura dati
                    _students.insert(std::pair<int, Student>(matr, Student(matr, infoStud[0], infoStud[1], infoStud[2],
                                                                           infoStud[3], infoStud[4],
                                                                           infoStud[5]))); //inserisco il nuovo studente nella mappatura interna
                }
            } else {
                //controllo che il formato file sia corretto: 3 campi per la versione base
                if (infoStud.size() != 3) {
                    error.append("Errore formato file studenti alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
                if (Parse::controlFieldsVectorAreEmpty(infoStud)) {
                    //controllo che non ci siano campi vuoti
                    error.append("Uno o piu campi di informazione per l'aggiunta degli studenti e' vuoto alla riga: " +
                                 std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                } else if (infoStud.size() == 3) {
                    //Se non ci sono campi vuoti e il formato è esatto inserisco le informazioni nella struttura dati
                    _students.insert(std::pair<int, Student>(matr, Student(matr, infoStud[0], infoStud[1],
                                                                           infoStud[2]))); //inserisco il nuovo studente nella mappatura interna
                }
            }
        }
        line_counter++;
    }
    fIn.close();
    ///se doDbWrite dovesse essere false, almeno una riga del file presenta degli errori per cui
    ///non si procede con la vera scrittura nel database degli studenti
    ///piuttosto vengono passati gli errori al main che verranno presentati all'utente
    if (doDbWrite)
        dbStudsWrite();
    else
        throw std::invalid_argument(error);
}

///identico alla addStuds(); si evita di commentare per non sporcare il codice
void University::addProfessors(const std::string &fileIn) {
    //addprofessor
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("Errore apertura file di inserimento nuovi professori \n");
    }
    bool doDbWrite = true;
    std::string line;
    int line_counter = 1;
    std::vector<std::string> infoProf;
    std::string error;
    while (std::getline(fIn, line)) {
        if(line.empty() == false) {
            int matr = getNewProfessorId();
            infoProf = Parse::splittedLine(line, ';');
            if (_version == 2) {
                //controllo che formato file sia corretto: 3 campi
                if (infoProf.size() != 6) {
                    error.append("Errore formato file professori alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
                if (Parse::controlFieldsVectorAreEmpty(infoProf)) {
                    error.append("Uno o piu' campi di informazione per l'aggiunta dei professori e' vuoto alla riga: " +
                                 std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                } else if (infoProf.size() == 6) {
                    _professors.insert(std::pair<int, Professor>(matr,
                                                                 Professor(matr, infoProf[0], infoProf[1], infoProf[2],
                                                                           infoProf[3], infoProf[4], infoProf[5])));
                }
            } else {
                //controllo che formato file sia corretto: 3 campi
                if (infoProf.size() != 3) {
                    error.append("Errore formato file professori alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
                if (Parse::controlFieldsVectorAreEmpty(infoProf)) {
                    error.append("Uno o piu' campi di informazione per l'aggiunta dei professori e' vuoto alla riga: " +
                                 std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                } else if (infoProf.size() == 3)
                    _professors.insert(
                            std::pair<int, Professor>(matr, Professor(matr, infoProf[0], infoProf[1], infoProf[2])));
            }
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
        throw std::invalid_argument("Errore apertura file di inserimento nuove aule");
    }
    std::string line;
    bool doDbWrite = true;
    int line_counter = 1;
    std::string error;
    std::vector<std::string> infoClassroom;
    while (std::getline(fIn, line)) {
        if(line.empty()== false) {
            infoClassroom = Parse::splittedLine(line, ';');
            int id = getNewClassroomId();
            if (_version == 3) {
                //controllo che il formato del file sia corretto: 7 campi
                if (infoClassroom.size() != 7) {
                    error.append("Errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
                if (Parse::controlFieldsVectorAreEmpty(infoClassroom)) {
                    error.append("Uno o piu' campi di informazione per l'aggiunta delle aule e' vuoto alla riga: " +
                                 std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                } else if (infoClassroom.size() == 7) {
                    try {
                        _classrooms.insert(
                                std::pair<int, Classroom>(id, Classroom(id, infoClassroom[0], infoClassroom[1],
                                                                        Parse::checkedStoi(infoClassroom[2]," della capienza massima dell'aula "),
                                                                        Parse::checkedStoi(infoClassroom[3],
                                                                                           " della capienza per un esame dell'aula "),
                                                                        Parse::checkedStoi(infoClassroom[4],
                                                                                           " del numero di tavoli da disegno "),
                                                                        Parse::checkedStoi(infoClassroom[5],
                                                                                           " del numero di computer "),
                                                                        Parse::checkedStoi(infoClassroom[6],
                                                                                           " del numero di proiettori "),
                                                                        Parse::checkedStoi(infoClassroom[7],
                                                                                           " del numero di lavagne "))));

                    } catch (std::exception &err) {
                        std::string generalError = err.what();
                        error.append(generalError + " alla riga: " + std::to_string(line_counter)+"\n");
                        doDbWrite = false;
                    }
                }
            } else {
                //controllo che il formato del file sia corretto: 4 campi
                if (infoClassroom.size() != 4) {
                    error.append("Errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
                if (Parse::controlFieldsVectorAreEmpty(infoClassroom)) {
                    error.append("Uno o piu' campi di informazione per l'aggiunta delle aule e' vuoto alla riga: " +
                                 std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }else if (infoClassroom.size() == 4) {
                    try {
                        _classrooms.insert(
                                std::pair<int, Classroom>(id, Classroom(id, infoClassroom[0], infoClassroom[1],
                                                                        Parse::checkedStoi(infoClassroom[2],
                                                                                           " della capienza massima dell'aula "),
                                                                        Parse::checkedStoi(infoClassroom[3],
                                                                                           " della capienza esame dell'aula "))));
                    } catch (std::exception &err) {
                        std::string generalError = err.what();
                        error.append(generalError + " alla riga: " + std::to_string(line_counter) + "\n");
                        doDbWrite = false;
                    }
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
void University::addCourses(const std::string &fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento nuovi corsi");
    }
    std::string line;
    int line_counter = 1;
    std::vector<std::string> specificYearCourse;
    std::string acYear;
    std::string examData;
    std::string idGroupedCourse;
    bool isActive = true;
    int num_parallel_courses = 0;
    std::string profSenzaQuadre;
    std::vector<std::string> idGrouped;
    std::vector<std::string> splittedExamData;
    bool doDbWrite = true;
    int cfu = 0;
    int lessonHours = 0;
    int exerciseHours = 0;
    int labHours = 0;
    std::string error;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        specificYearCourse = Parse::splittedLine(line, ';');
        //controllo che il formato file sia corretto: 10 campi
        if (specificYearCourse.size() != 10) {
            error.append("formato file corsi non valido alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        }
        //controllo che tutti campi siano specificati
        for (int i = 0; i < specificYearCourse.size(); i++) {
            if (specificYearCourse[i].empty()) {
                error.append("uno o più campi sono vuoti alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
        }
        //controllo che l'esame non sia già presente in base dati
        //cerco nel DB un esame con stesso titolo e cfu
        for (auto iterCourses = _courses.begin(); iterCourses != _courses.end(); iterCourses++) {
            if (iterCourses->second.getName() == specificYearCourse[1] &&
                iterCourses->second.getCfu() == Parse::checkedStoi(specificYearCourse[2]," dei cfu")) {
                error.append("c'è un corso già presente in base dati alla riga: " + std::to_string(line_counter)+"\n");
                doDbWrite = false;
            }
        }

        if (doDbWrite) {
            std::string newIdCourse = getNewCourseId();
            try {
                cfu = Parse::checkedStoi(specificYearCourse[2], " dei cfu ");
                lessonHours = Parse::checkedStoi(specificYearCourse[3], " delle ore di lezione ");
                exerciseHours = Parse::checkedStoi(specificYearCourse[4], " delle ore di esercitazione");
                labHours = Parse::checkedStoi(specificYearCourse[5], " delle ore di laboratorio");
                num_parallel_courses = Parse::checkedStoi(specificYearCourse[6]," del numero di corsi in parallelo");//numero di corsi in parallelo
            } catch (std::invalid_argument &err) {
                std::string genericError = err.what();
                error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            ///se non ho problemi di conversione dei cfu,ore lezione, esercitazione e laboratorio posso andare avanti altrimenti passo al prossimo rigo del file
            if (doDbWrite) {
                _courses.insert(std::pair<std::string, Course>(newIdCourse, Course(newIdCourse, specificYearCourse[1],
                                                                                   cfu, lessonHours, exerciseHours,
                                                                                   labHours)));

                //anno accademico
                if (Parse::controlItCanBeAnAcYear(specificYearCourse[0]))
                    acYear = specificYearCourse[0];


                profSenzaQuadre = specificYearCourse[7].substr(1, specificYearCourse[7].size() - 2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
                std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre,num_parallel_courses);//divido i vari corsi in parallelo
                examData = specificYearCourse[8];//informazioni sull'esame
                examData = examData.substr(1, examData.size() - 2);//tolgo le { } che racchiudono le info degli esami
                splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
                idGroupedCourse = specificYearCourse[9];//id dei vari corsi raggruppati
                idGroupedCourse = idGroupedCourse.substr(1, idGroupedCourse.size() -
                                                            2);// tolgo le { } che racchiudono gli id
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
                                controlGroupedCoursesDifferentCds_Reciprocy(idGrouped, specificYearCourse[1],
                                                                            line_counter,
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
                    _courses.at(newIdCourse).addSpecificYearCourses(acYear, isActive, num_parallel_courses,
                                                                    profCorsoPar,
                                                                    splittedExamData, idGrouped, yy_semester,
                                                                    studyCourse,
                                                                    line_counter);
                }
                catch (std::exception &err) {
                    error.append(err.what());
                    doDbWrite = false;
                }
                ///qui: fill dei corsi raggruppati dell'anno accademico
                if (!idGrouped.empty()) {
                    fillGroupedCourse(idGrouped, newIdCourse, acYear, line_counter);
                } else {
                    if (_coursesGrouped.count(newIdCourse) != 0) {
                        idGrouped = _coursesGrouped.at(acYear + "-" + newIdCourse);
                        SpecificYearCourse &specificYY = _courses.at(newIdCourse).getThisYearCourseReference(
                                Parse::getAcStartYear(acYear));//corso per un anno specifico
                        specificYY.assignGrouped(idGrouped, newIdCourse, newIdCourse);
                    }
                }
                ///controllo che i professori di questo corso esistano già in _professors
                try {
                    _courses.at(newIdCourse).controlTheExistenceAndHoursOfProfessors(_professors,
                                                                                     Parse::getAcStartYear(acYear));
                } catch (std::exception &err) {
                    error.append(err.what());
                    doDbWrite = false;
                }
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
    fileIn.close();
    if (doDbWrite) {
        dbCourseWrite();
    }
    else{
        throw std::invalid_argument(error);
    }
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
    if (_classrooms.empty())
        return 1;
    auto last = _classrooms.rbegin();  //
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
    bool doDbWrite = true;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("Errore apertura file per aggiornamento studenti \n");
    }
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::vector<std::string> infoStud;
    std::string error;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        if(line.empty()==false) {
            int nMatr = 0;
            infoStud = Parse::splittedLine(line, ';');
            ///controllo se può essere una matricola(6 numeri)
            bool canBeAnId = Parse::controlItCanBeAnId(infoStud[0], 6,'s');

            if(canBeAnId)
                //se un numero a 6 cifre e la lettera iniziale è una 'd' può essere una matricola
                nMatr = Parse::getMatr(infoStud[0]);

            if (_version == 2) {
                //se versione aggiuntiva controllo che il suo formato sia di 6 campi
                if (std::count(line.begin(), line.end(), ';') != 6) {
                    error.append("Errore formato file alla riga: " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                }
            } else if (std::count(line.begin(), line.end(), ';') != 3) {
                //se versione aggiuntiva controllo che il suo formato sia di 3 campi
                error.append("Errore formato file alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            //controllo che il primo campo non sia vuoto: la matricola deve esserci !!
            if (infoStud[0].empty()) {
                error.append("Manca la matricola dello studente alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            } else if (canBeAnId == false) {
                //se il primo campo non è vuoto va comunque considerato il caso in cui questo non sia una matricola
                error.append("Il primo campo alla riga " + std::to_string(line_counter) +
                             " non puo' essere un codice Id di uno studente \n");
                doDbWrite == false;
            } else if (_students.find(nMatr) == _students.end()) { //find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
                //se il primo campo è una matricola devo assicurarmi che esista nel database
                error.append("Lo studente " + infoStud[0] + " alla riga " + std::to_string(line_counter) +  " non e' presente nel database.\n");
                doDbWrite = false;
            }
            if (doDbWrite) {
                //se il primo campo è una matricola ed è presente nel database degli studenti procedo con le operazioni
                auto iter = _students.find(nMatr);//prendo la posizione della matricola
                for (i = 1; i < infoStud.size(); i++) {
                    ///ciclo su tutti i campi di informazioni
                    ///se ci sono dei campi vuoti vuol dire che l'utente ha considerato la possbilità
                    /// di non cambiare le informazioni dello studente lasciando vuoto quello specifico campo
                    if (!(infoStud[i].empty())) {
                        switch (i) {
                            case update_name : {//analizzo il nome(secondo campo della riga del file)
                                if (!(iter->second.getName() == infoStud[i])) {
                                    //se il nome letto dal file è diverso dal nome del database lo cambio con quello nuovo
                                    iter->second.setName(infoStud[i]);
                                }
                                break;
                            }
                            case update_surName : {//analizzo il cognome(secondo campo della riga del file)
                                if (!(iter->second.getSurname() ==  infoStud[i])) {
                                    //se il cognome letto dal file è diverso dal cognome del database
                                    iter->second.setSurnName(infoStud[i]);
                                }
                                break;
                            }
                            case update_eMail : { //analizzo l'email(terzo campo della riga del file)
                                if (!(iter->second.getEmail() == infoStud[i])) {
                                    //se l'email letta dal file è diversa dall'email del database la cambio
                                    iter->second.setEmail(infoStud[i]);
                                }
                                break;
                            }
                            ///i campi da qui in poi sono raggiungibili solo in caso di versione aggiuntiva
                            case update_birth: {//analizzo la data di nascita(quarto campo della riga del file)
                                try {
                                    Date date = Parse::controlItCanBeADate(infoStud[i]);
                                    if (!(iter->second.getBirth() ==  date.toString())) {
                                        //se la data di nascita letta dal file è diversa dalla data del database la cambio
                                        iter->second.setBirth(infoStud[i]);
                                        }
                                    }catch(std::invalid_argument & err){
                                        std::string genericError = err.what();
                                        error.append(genericError + " alla riga " + std::to_string(line_counter)+ "\n");
                                        doDbWrite = false;
                                    }
                                break;
                            }
                            case update_registrationOrEntry: {//analizzo la data di registrazione(quinto campo della riga del file)
                                try {
                                    Date date = Parse::controlItCanBeADate(infoStud[i]);
                                    if (!(iter->second.getRegistrationOrEntry() ==  date.toString())) {
                                        //se la data di registrazione letta dal file è diversa da quella del database la cambio
                                        iter->second.setRegistration(infoStud[i]);
                                    }
                                }catch(std::invalid_argument & err){
                                    std::string genericError = err.what();
                                    error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                                    doDbWrite = false;
                                }


                                break;
                            }
                            case update_address: {//analizzo l'indirizzo(sesto campo della riga del file)
                                if (!(iter->second.getAddress() ==  infoStud[i])) {
                                    //se l'indirizzo letto dal file è diverso da quello del database lo cambio
                                    iter->second.setAdress(infoStud[i]);
                                }
                                break;
                            }
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
    else
        throw std::invalid_argument(error);

}

///aggiorno i professori(la maggior parte dei commenti viene evitata per evitare di sporcre il codice poichè è uguale all'updatestuds)
void University::updateProfessors(const std::string &fin) {
    int i;
    int nMatr = 0;
    bool doDbWrite = true;
    std::string error;
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("Errore apertura file per aggiornamento professori \n");
    }
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::vector<std::string> infoProf;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        if(line.empty()== false) {
            infoProf = Parse::splittedLine(line, ';'); //splitto il rigo nei vari campi di interesse
            bool canBeAnId = Parse::controlItCanBeAnId(infoProf[0], 6,'d');

            if(canBeAnId)
                //se un numero a 6 cifre e la lettera iniziale è una 's' può essere una matricola
                nMatr = Parse::getMatr(infoProf[0]);

            if (_version == 2) {
                if (std::count(line.begin(), line.end(), ';') != 6) {
                    error.append("Errore formato file professori alla riga: " + std::to_string(line_counter));
                    doDbWrite = false;
                }
            } else if (std::count(line.begin(), line.end(), ';') != 3) {
                error.append("Errore formato file professori alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
            if (infoProf[0].empty()) {
                error.append("Manca la matricola del professore alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            } else if (canBeAnId == false) {
                error.append("Il primo campo alla riga " + std::to_string(line_counter) + " non puo' essere un codice Id di un professore \n");
                doDbWrite = false;
            } else if (_professors.find(nMatr) == _professors.end()) {//find mi restituisce literatore alla chiave inserita(nMatr). se non lo trova mi ritorna l'iteratore dell'elemento successivo all'ultimo
                error.append("Il professore " + infoProf[0] + " alla riga " + std::to_string(line_counter) +  " non e' presente nel database.\n");
                doDbWrite = false;
            }
            if (doDbWrite) {
                //se il primo campo è una matricola ed è presente nel database degli studenti procedo con le operazioni
                auto iter = _professors.find(nMatr);//prendo la posizione della matricola
                ///ciclo su tutti i campi di informazioni
                ///se ci sono dei campi vuoti vuol dire che l'utente ha considerato la possbilità
                /// di non cambiare le informazioni dello studente lasciando vuoto quello specifico campo
                for (i = 1; i < infoProf.size(); i++) {
                    if (!(infoProf[i].empty())) {
                        switch (i) {
                            case update_name : {//analizzo il nome
                                if (!(iter->second.getName() == infoProf[i])) {
                                    iter->second.setName(infoProf[i]);
                                }
                                break;
                            }
                            case update_surName : {//analizzo il cognome
                                if (!(iter->second.getSurname() == infoProf[i])) {
                                    iter->second.setSurnName(infoProf[i]);
                                }
                                break;
                            }
                            case update_eMail : { //analizzo l'email
                                if (!(iter->second.getEmail() == infoProf[i])) {
                                    iter->second.setEmail(infoProf[i]);
                                }
                                break;
                            }
                            ///i campi da qui in poi sono raggiungibili solo in caso di versione aggiuntiva
                            case update_birth: {//analizzo la data di nascita
                                if (!(iter->second.getBirth() == infoProf[i])) {
                                    iter->second.setBirth(infoProf[i]);
                                }
                                break;
                            }
                            case update_registrationOrEntry: {//analizzo la data di registrazione
                                if (!(iter->second.getRegistrationOrEntry() == infoProf[i])) {
                                    iter->second.setRegistration(infoProf[i]);
                                }
                                break;
                            }
                            case update_address: {//analizzo l'indirizzo
                                if (!(iter->second.getAddress() ==  infoProf[i])) {
                                    iter->second.setAdress(infoProf[i]);
                                }
                                break;
                            }
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
    update_nExamSeats = 4,
    update_drawingTable = 5,
    update_computer = 6,
    update_projector = 7,
    update_blackBoard = 8
};

///aggiorno le Classroom
void University::updateClassroom(const std::string &fin) {
    int i;
    int nMatr = 0;
    bool doDbWrite = true;

    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("Errore apertura file per aggiornamento aule \n");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::string error;
    std::vector<std::string> infoClassroom;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoClassroom = Parse::splittedLine(line, ';');
        bool canBeAnId = Parse::controlItCanBeAnId(infoClassroom[0], 3,'A');
        if(canBeAnId)
            //se un numero a 3 cifre e la lettera iniziale è una 'A' può essere il codice di un'aula
            nMatr = Parse::getMatr(infoClassroom[0]);

        if (_version == 3) {
            //se versione aggiuntiva controllo che il suo formato sia di 8 campi
            if (std::count(line.begin(), line.end(), ';') != 7) {
                error.append("Errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
            }
        } else if (std::count(line.begin(), line.end(), ';') != 4) {
            //se versione base controllo che il suo formato sia di 5 campi
            error.append("Errore formato file aule alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        }
        if (infoClassroom[0].empty()) {
            error.append("Manca il codice dell' aula alla riga: " + std::to_string(line_counter) + "\n");
            doDbWrite = false;
        } else if (canBeAnId == false) {
            error.append("Il primo campo alla riga " + std::to_string(line_counter) + " non puo' essere un codice Id di una aula \n");
            doDbWrite == false;
        } else if (_classrooms.find(nMatr) == _classrooms.end()) {//se non trovo il codice
            error.append("L'aula " + infoClassroom[0] + " alla riga "+ std::to_string(line_counter) + " non e' presente nel database \n");
            doDbWrite = false;
        }
        if (doDbWrite) {
            auto iter = _classrooms.find(nMatr);//prendo la posizione della matricola
            for (i = 1; i < infoClassroom.size(); i++) {
                //se la stringa raccolta da infoClassroom è vuota vuol dire che l'utente ha scelto di caricare i dati con la possibilità di saltare i campi che non verranno cambiati
                if (!(infoClassroom[i].empty())) {
                    switch (i) {
                        case update_lab : {//analizzo il tipo di classroom: aula o lab
                            bool lab;
                            bool isOk = true;
                            if (infoClassroom[i] == "L") //se il carattere intercettato è L allora l'aula da aggiungee sarà un lab
                                lab = true;
                            else if(infoClassroom[i] == "A")//altrimenti sarà un'aula
                                lab = false;
                            else {
                                error.append("La lettera identificativa del tipo di classe alla riga " +
                                             std::to_string(line_counter) + " non e' una 'L' e non e' una 'A'\n");
                                isOk = false;
                            }
                            //se la lettera è una A o una L e se è cambiata da quella che c'è nel database la cambio
                            if ( isOk && iter->second.isThisLab() != lab) {
                                iter->second.setType(lab);
                            }
                            break;
                        }
                        case update_nameClassroom : {//analizzo il nome della classroom
                            //se il nome della classe è cambiata da quella che c'è nel database la cambio
                            if (!(iter->second.getName() == infoClassroom[i])) {
                                iter->second.setName(infoClassroom[i]);
                            }
                            break;
                        }
                        case update_nSeats : {//analizzo la capienza
                            try {
                                //se il numero di posti è cambiata da quello che c'è nel database lo cambio
                                int numSeats = Parse::checkedStoi(infoClassroom[i]," della capienza massima dell'aula");
                                if (iter->second.getNSeats() != numSeats) {
                                    iter->second.setNSeats(numSeats);
                                }
                            }catch (std::invalid_argument& err){
                                // se non è un numero intero positivo non va bene, lo segnalo
                                std::string genericError = err.what();
                                error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                                doDbWrite = false;
                            }
                            break;
                        }
                        case update_nExamSeats : {//analizzo la capienza massima per un esame
                            try {
                                //se il numero di posti è cambiata da quello che c'è nel database lo cambio
                                int numnExamSeats = Parse::checkedStoi(infoClassroom[i]," della capienza per un esame dell'aula ");
                                if (iter->second.getNExamSeats() != numnExamSeats) {//se la capienza dell'aula per gli esami letta da file non è uguale alla capienza dell'aula per gli esami del database
                                    iter->second.setNExamSeats(numnExamSeats); //cambia l'email
                                }
                            }catch (std::invalid_argument& err){
                                // se non è un numero intero positivo non va bene, lo segnalo
                                std::string genericError = err.what();
                                error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                                doDbWrite = false;
                            }
                            break;
                        }
                        ///da qui in poi sarà raggiungibile solo in caso di versione aggiuntiva
                        case update_drawingTable: {//analizzo il numero di tavole da disegno
                            try {
                                //se il numero di tavoli da disegno è diverso da quello che c'è nel database lo cambio
                                int numDrawingTable = Parse::checkedStoi(infoClassroom[i], " del numero di tavoli da disegno ");
                                if (iter->second.getDrawingTable() != numDrawingTable) {
                                    iter->second.setDrawingTable(numDrawingTable);
                                }
                            }catch (std::invalid_argument& err){
                                // se non è un numero intero positivo non va bene, lo segnalo
                                std::string genericError = err.what();
                                error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                                doDbWrite = false;
                            }

                            break;
                        }
                        case update_computer: {//analizzo il numero di computer
                            try {
                                //se il numero di computer è diverso da quello che c'è nel database lo cambio
                                int numComputer = Parse::checkedStoi(infoClassroom[i]," del numero di computer");
                                if (iter->second.getComputer() != numComputer) {
                                    iter->second.setComputer(numComputer);
                                }
                            }catch (std::invalid_argument& err){
                                // se non è un numero intero positivo non va bene, lo segnalo
                                std::string genericError = err.what();
                                error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                                doDbWrite = false;
                            }
                            break;
                        }
                        case update_projector: {//analizzo il numero di proiettori
                            try {
                                //se il numero di proiettori è diverso da quello che c'è nel database lo cambio
                                int numProjector = Parse::checkedStoi(infoClassroom[i]," del numero di proiettori");
                                if (iter->second.getProjector() != numProjector) {
                                    iter->second.setProjector(numProjector);
                                }
                            }catch (std::invalid_argument& err){
                                // se non è un numero intero positivo non va bene, lo segnalo
                                std::string genericError = err.what();
                                error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                                doDbWrite = false;
                            }
                            break;
                        }
                        case update_blackBoard: {//analizzo il numero di lavagne
                            try {
                                //se il numero di lavagne è diverso da quello che c'è nel database lo cambio
                                int numBlackBoard = Parse::checkedStoi(infoClassroom[i], " del numero di lavagne");
                                if (iter->second.getBlackBoard() != numBlackBoard) {
                                    iter->second.setBlackBoard(numBlackBoard);
                                }
                            }catch (std::invalid_argument& err){
                                // se non è un numero intero positivo non va bene, lo segnalo
                                std::string genericError = err.what();
                                error.append(genericError + " alla riga " + std::to_string(line_counter) + "\n");
                                doDbWrite = false;
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
        num_parallel_courses = Parse::checkedStoi(specificYearCourse[3],"Errore numero di corsi in parallelo");//numero di corsi in parallelo
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

    for (auto iterClassRoom = _classrooms.begin(); iterClassRoom != _classrooms.end(); iterClassRoom++) {
        Classroom room = _classrooms.at(iterClassRoom->first);
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
    int acStartYear = 0;
    std::string error;
    bool doDbWrite = true;

    //controllo che possa essere un anno accademico e in quel caso prendo il primo anno
    //altrimenti blocco il programma
    if(Parse::controlItCanBeAnAcYear(acYear)){
        acStartYear = Parse::getAcStartYear(acYear);
    }

    ///se il controllo sull'anno è andato a buon fine faccio il controllo sulle date per ogni sessione
    controlCoerenceSessionDate(winterSession, summerSession,autumnSession,acStartYear);
    //se non ci sono stati errori di formato, coerenza tra anno accademico e periodi delle sessioni
    //controllo se esistono già dei periodi per le sessioni di quell'anno; in questo caso le devo cancellare per aggiungere le nuove
    if (_acYearSessions.count(acStartYear) != 0) {
        _acYearSessions.erase(acStartYear);
    }
    //creo oggetto session da aggiungere
    try {
        SessionYear sessionYear(acYear, winterSession, summerSession, autumnSession);
        //popolo mappa in university
        _acYearSessions.insert(std::pair<int, SessionYear>(acStartYear, sessionYear));
    }catch(std::exception& err) {
        error.append(err.what());
        doDbWrite = false;
    }
    if(doDbWrite) {
        //controllo che la prima data della sessione estiva sia successiva all'ultima della sessione invernale
        //che la prima data della sessione autunnale sia successiva all'ultima della sessione invernale e all'ultima della sessione estiva
        try {
            _acYearSessions.at(acStartYear).controlSuccessivitySessionPeriod();
            dbDateSessionsWrite();
        }catch (std::exception& err) {
            error.append(err.what());
            throw  std::invalid_argument(error);
        }
    }else
        throw std::logic_error(error);
}

///controlli sulle sessioni
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

///per ogni sessione controllo che il formato dei periodi sia corretto, le date siano corrette e che ci sia coerenza tra le date e l'anno accademico scelto
void University::controlOfASingleSessionPeriod(std::string name , std::string sessionPeriod, int acStartYear) {
    std::vector<std::string> dateSession = Parse::splittedLine(sessionPeriod, '_');
    std::string error;
    std::string session;
    Date startSessionDate;
    Date endSessionDate;
    bool isOk = true;

    if (name == "winter")
        session = "invernale";
    else if (name == "summer")
        session = "estiva";
    else if (name == "autumn")
        session = "autunnale";

    //controllo che il formato del periodo sia rispettato
    if (dateSession.size() != 2 || sessionPeriod.size() != 21)//AAAA-MM-GG_AAAA-MM-GG (21 caratteri)
        throw std::invalid_argument("Il periodo della sessione " + session + " non rispetta il formato richiesto\n");

    ///controllo che il formato delle date sia rispettato
    try {
        startSessionDate = Parse::controlItCanBeADate(sessionPeriod.substr(0, 10));
    } catch (std::invalid_argument &err) {
        std::string genericError = err.what();
        error.append("La prima data della sessione " + session + " non conforme con le specifiche." + genericError);
        isOk = false;
    }
    try {
        endSessionDate = Parse::controlItCanBeADate(sessionPeriod.substr(11, 10));
    } catch (std::invalid_argument &err) {
        std::string genericError = err.what();
        error.append("La seconda data della sessione " + session + " non conforme con le specifiche." + genericError);
        isOk = false;
    }
    //adesso controllo che ci sia coerenza tra l'anno richiesto da comando e i periodi passati da comando
    if (isOk) {
        //se arrivo fin qui vuol dire che abbiamo effettivamente due date
        ///AAA SI ASSUME, SECONDO LA LEGISLAZIONE ITALIANA, CHE UN ANNO ACCADEMICO INIZI IL 01/11/aaaa E FINISCA IL 31/10/aaaB
        Date acStartDate = Date(acStartYear,11,01);
        Date acEndDate = Date(acStartYear+1,10,31);
        if (startSessionDate < acStartDate || endSessionDate > acEndDate) {
            throw std::invalid_argument("La sessione " + session + " non rientra nell'anno accademico richiesto via comando \n");
        } else if (startSessionDate > endSessionDate) {
            throw std::invalid_argument("La data di fine sessione " + session + " e' inferiore a quella di inizio \n");
        }

    } else
        throw std::invalid_argument(error);
}

///set indisponibilità professori in base all'anno
void University::setProfsNoAvailability(std::string acYear, const std::string &fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("file indisponibilita' dei professori non esistente \n");
    }
    std::string line;
    std::vector<std::string> profAvailability;
    int year = 0;
    int line_counter = 1;
    bool doDbWrite = true;
    std::string error;

    ///controllo possa essere un anno accademico

    bool canBeAnAcYear = Parse::controlItCanBeAnAcYear(acYear);
    if(canBeAnAcYear == false){
        error.append("Il primo comando non puo' essere un anno accademico\n");
        doDbWrite = false;
    } else{
        year = Parse::getAcStartYear(acYear);
    }

    while (std::getline(fileIn, line)) {//fino alla fine del file leggo un rigo alla volta
        //se il rigo appena letto è vuoto passo al successivo
        if(line.empty()==false) {
            profAvailability = Parse::splittedLine(line, ';');
            int nMatr = 0;

            ///se il primo campo è vuoto passo al prossimo rigo
            if (profAvailability[0].empty()) {
                error.append("Non e' stata dichiarata la matricola del professore alla riga: " +
                             std::to_string(line_counter) + "\n");
                doDbWrite = false;
            } else if (Parse::controlItCanBeAnId(profAvailability[0], 6,'d')) {///controllo che possa essere effettivamente la matricola di un professore
                //se un numero a 6 cifre e la lettera iniziale è una 'd' può essere una matricola
                nMatr = Parse::getMatr(profAvailability[0]);
                auto iterProf = _professors.find(nMatr);//punto al prof con matricola nMatr
                if (iterProf == _professors.end()) {
                    error.append("La matricola " + profAvailability[0] + " alla riga" + std::to_string(line_counter) +
                                 " non e' presente nel database \n");
                    doDbWrite = false;
                } else {
                    //dovrò aggiornare l'indisponibilità di un prof per uno specifico anno accademico quindi cancello le
                    //vecchie indisponibilità di uno specifico prof per uno specifico anno
                    iterProf->second.noAvailabilityClear(year);
                    for (int i = 1; i < profAvailability.size(); i++) {//per ogni campo della riga letta
                        try {
                            _professors.at(nMatr).setNoAvaibilities(year,profAvailability[i]);//vado a settare uno dei periodi di indisponibilità del prof nella map _professor
                        } catch (std::exception &err ) {
                            std::string generalError = err.what();
                            error.append(generalError + std::to_string(i) + " del professore con matricola " +
                                         profAvailability[0] + " alla riga: " + std::to_string(line_counter) + "\n");
                            doDbWrite = false;
                        }
                    }
                }
            } else {
                error.append("Il primo campo alla riga " + std::to_string(line_counter) +
                             " non puo' essere la matricola di un professore\n");
                doDbWrite = false;
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
    //riempio i corsi fino alla sessione voluta (può essere che un corso non venga aggiornata ma ciò non significa che non possa avere quel corso in quell'anno)
    for(auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++){
        int lastYear = iterCourse->second.getLastSpecificYearCourse().getStartYear();
        iterCourse->second.fillAcYearsUntilStartAcYear(startAcYear,lastYear);
    }
    ///faccio dei controlli di coerenza dei dataBase
    controlDatabase(startAcYear);
    ///il ciclo sarà eseguito se le sessioni non sono ancora generate(result==false) e finchè ci saranno ancora vincoli da poter rilassare
    while (!esito && constraintRelaxParameter < 4) {
        //accedo all'anno accademico passato dal comando e genero le sessioni per un anno
        try {
            esito = _acYearSessions.at(startAcYear).generateNewYearSession(outputNameFile, 0,*this);
        }catch (std::logic_error& err){
            ///questo throw sarà attivo solo se non posso trovare disponibilità di aule in nessun giorno e in nessuno slot per almeno un corso
            throw std::invalid_argument(err.what());
        }
        constraintRelaxParameter++;
    }
    ///se le sessioni non possono essere generate nonostante i vincoli rilassati
    if (!esito) {
        throw std::invalid_argument("non è possibile generare la sessione");
    }else{
        generateOutputFileName();
    }
}

void University::generateOutputFileName() {
    std::fstream outputFile;
    outputFile.open("allFileNamePerSessionYear.txt", std::fstream::out | std::fstream::trunc);
    if (!outputFile.is_open()){
        throw std::exception();
    }
    for(auto iterSession = _acYearSessions.begin(); iterSession != _acYearSessions.end(); iterSession++){
        int acStart = iterSession->first;
        if(_acYearSessions.at(acStart).fileNameIsEmpty() == false){
            std::vector<std::string> sessionAndFileName = iterSession->second.getSessionAndFileName();
            for(int i = 0; i<sessionAndFileName.size();i++) {
                outputFile << std::to_string(acStart) + "-" + sessionAndFileName[i] + "\n";
            }
        }
    }
    outputFile.close();
}

void University::readOutputFileName(){
    std::ifstream fileIn("allFileNamePerSessionYear.txt");
    if (!fileIn.is_open()) {
        throw DbException("file allFileNamePerSessionYear.txt non esistente");
    }
    std::string line;
    while (std::getline(fileIn, line)) {
        std::vector<std::string> infoName = Parse::splittedLine(line,';');
        std::string acStartString = infoName[0].substr(0,4);
        std::string numSessionString = infoName[0].substr(5,1);
        int acStart = Parse::checkedStoi(acStartString,"Errore anno accademico");
        int numSession = Parse::checkedStoi(numSessionString,"Errore anno accademico");
        std::string fileName = infoName[1];
        _acYearSessions.at(acStart).setFileNamePerSession(numSession,fileName);
    }
}
///controlla se i file sono coerenti
bool University::controlDatabase(int startAcYear) {
    ///controllo che i database non siano vuoti
    dataBaseIsEmpty(startAcYear);
    bool checkIsOK = true;
    std::string error;
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        ///controllo che ogni corso in _courses esista in uno StudyCourse
        Course course = iterCourse->second;
        if (whatIsMyStudyCourse(course,startAcYear) == -1) {
            error.append( "Il corso " + iterCourse->first + " non esiste in alcun corso di studio. Inserire nuovi corsi di studio che lo contengano.\n");
            checkIsOK = false;
        }
    }
    ///COMMENTATO PERCHE' NON MI ANDAVA DI CARICARE ALTRI CORSI PER DEBUGGARE LA GESTIONE ESAMI
    /*
    for (auto iter = _studyCourse.begin(); iter != _studyCourse.end(); iter++) {
        ///controllo che ogni studyCourse abbia i corsi specificati
        StudyCourse sCourse = iter->second;
        std::vector<std::string> allCoursesOfThis = sCourse.getAllCoursesOfStudyCourse();
        for (int i = 0; i < allCoursesOfThis.size(); i++) {
            int occ = _courses.count(allCoursesOfThis[i]);
            if (occ == 0) {
                std::string settedId = Parse::setId('C',3,iter->first);
                error.append("Il corso "+ allCoursesOfThis[i]+ " presente nel corso di studio " + settedId +" non e' ancora stato attivato. Inserirlo tra i corsi \n");
                checkIsOK = false;
            }
        }
    }*/

    if(checkIsOK == false)
        throw std::invalid_argument(error);

    return checkIsOK;
}

///i database sono vuoti?
bool University::dataBaseIsEmpty(int startAcYear) {
    std::string error;
    bool isOk = true;
    if (_professors.empty()) {
        error.append("Il database dei professori e' vuoto \n");
        isOk = false;
    }
    if (_students.empty()) {
        error.append("Il database degli studenti e' vuoto \n");
        isOk = false;
    }
    if (_courses.empty()) {
        error.append("Il database dei corsi e' vuoto \n");
        isOk = false;
    }
    if (_studyCourse.empty()) {
        error.append("Il database dei corsi di studio e' vuoto \n");
        isOk = false;
    }
    if (_acYearSessions.find(startAcYear) == _acYearSessions.end()) {
        error.append("Non ci sono informazioni sulle sessioni per questo anno: " + std::to_string(startAcYear) + "-" +std::to_string(startAcYear +1) + "\n");
        isOk = false;
    }else if (_acYearSessions.at(startAcYear).sessionsPeriodIsEmpty()) {
        error.append("I periodi delle sessioni non sono stati definiti per l'anno: " + std::to_string(startAcYear) + "-" +
                std::to_string(startAcYear + 1) + "\n");
                isOk = false;
    }
    ///controllo che gli studenti siano iscritti nei corsi di questo anno accademico
    for (auto iter = _courses.begin(); iter != _courses.end(); iter++) {
        int firstYear = iter->second.getFirstYearOfActivity();
        ///se il corso è stato attivato prima della sessione rischiesta lo considero, altrimenti no
        if (firstYear <= startAcYear) {
            int studentsEnrolled = iter->second.getThisYearCourse(startAcYear).getTotStudentsEnrolled();
            if (studentsEnrolled == 0) {
                error.append("Il corso " + iter->second.getId() + " non ha studenti iscritti in questo anno: "+ std::to_string(startAcYear) + "-" + std::to_string(startAcYear + 1) + "\n");
                isOk = false;
            }
        }
    }
    if(isOk == false)
        throw std::invalid_argument(error);

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
        int firstYear = _courses.at(coursesToConsider[i]).getFirstYearOfActivity();
        int idStudyCourseOfThisCourse = whatIsMyStudyCourse(_courses.at(coursesToConsider[i]),firstYear);
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
    int newVer = Parse::checkedStoi(newVersion,"Errore numero versione");
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
        if(line.empty() == false) {
            int id = 0;
            bool isOk = true;
            std::vector<int> posSemiColon = Parse::posSemiColon(line);
            std::string acYearRegistration;
            bool canBeAnAcYear = true;

            ///controlli sul primo campo di informazioni, matricola
            std::string matr = line.substr(0, posSemiColon[0]);
            bool canBeAnId = Parse::controlItCanBeAnId(matr, 6, 's');

            if (matr.empty()) {
                error.append(
                        "Non e' stata dichiarata la matricola dello studente al rigo: " + std::to_string(line_counter) +
                        "\n");
                doDbWrite = false;
                isOk = false;
            } else if (canBeAnId == false) {
                ///se non è un numero a 6 cifre e la prima lettera non inizia con 's'
                error.append("Il primo campo di informazioni alla riga " + std::to_string(line_counter) +
                             " non puo' essere una matricola di uno studente \n");
                doDbWrite = false;
                isOk = false;
            } else {
                id = Parse::getMatr(matr);
            }

            ///se la matricola è ok faccio il controllo sul secondo campo di informazioni: anno accademico
            if(isOk) {
                acYearRegistration = line.substr(posSemiColon[0] + 1, posSemiColon[1] - posSemiColon[0] - 1);
                if (acYearRegistration.empty()) {
                    //se il campo dell'anno accademico è vuoto
                    error.append("Non e' stato dichiarato l'anno accademico alla riga " + std::to_string(line_counter) +
                                 "\n");
                    doDbWrite = false;
                    isOk = false;
                } else {
                    try {
                        canBeAnAcYear = Parse::controlItCanBeAnAcYear(acYearRegistration);
                    } catch (std::invalid_argument &err) {
                        std::string genericError = err.what();
                        error.append("Controllare riga: " + std::to_string(line_counter) + ". " + genericError);
                        doDbWrite = false;
                        isOk = false;
                    }
                    if (canBeAnAcYear == false) {
                        error.append("Anno accademico non valido al rigo: " + std::to_string(line_counter) + "\n");
                        doDbWrite = false;
                        isOk = false;
                    }
                }
            }

            /// se anno e matricola sono ok continuo con i controlli sul piano di studio
            if (isOk) {
                std::vector<int> pos = Parse::posCurlyBrackets(line);
                //mi assicuro che le parentesi graffe siano messe al posto giusto
                try {
                    Parse::controlStudyPlanFormat(pos, line);
                } catch (std::invalid_argument &err) {
                    std::string genericError = err.what();
                    error.append("Al rigo " + std::to_string(line_counter) + genericError);
                    doDbWrite = false;
                    isOk = false;
                }

                if (isOk) {
                    //se anche le parentesi graffe sono al posto giusto controllo i corsi
                    std::string allCourses = line.substr(posSemiColon[1] + 2, pos[1] - (posSemiColon[1] + 2));
                    if (_students.find(id) == _students.end()) {
                        error.append("Lo studente con matricola " + matr + " al rigo " + std::to_string(line_counter) +
                                     " non e' presente nel database \n");
                        doDbWrite = false;
                    } else {
                        Student &stud = _students.at(id);
                        std::vector<std::string> courses = Parse::splittedLine(allCourses, ';');
                        //controllo il formato dei corsi
                        if (allCourses.size() == 0) {
                            //piano di studio vuoto
                            ///se non ho inserito alcun piano di studio da aggiungere
                            error.append("Al rigo " + std::to_string(line_counter) +
                                         " non e' stato dichiarato alcun corso da aggiungere \n");
                            doDbWrite = false;
                            isOk = false;
                        } else {
                            //controllo che non ci siano caratteri non voluti tra i corsi
                            bool continueLoop = true;
                            for (int i = 0; i < allCourses.size() && continueLoop; i++) {
                                if (!((allCourses[i] >= 48 && allCourses[i] <= 57) || allCourses[i] == 59 ||
                                    (allCourses[i] >= 65 && allCourses[i] <= 90))) {
                                    error.append("Al rigo " + std::to_string(line_counter) +
                                                 " sono presenti caratteri non conformi con il formato del piano di studio \n");
                                    doDbWrite = false;
                                    isOk = false;
                                    continueLoop = false;
                                }
                            }
                            if (isOk) {
                                for (int i = 0; i < courses.size(); i++) {
                                    try {
                                        Parse::controlItCanBeACourseId(courses[i]);
                                    } catch (std::invalid_argument &err) {
                                        std::string genericError = err.what();
                                        error.append(genericError + ". Il codice " + courses[i] + " al rigo " +
                                                     std::to_string(line_counter) +
                                                     " non puo' essere il codice di un corso \n");
                                        doDbWrite = false;
                                        isOk = false;
                                    }
                                }
                            }
                        }
                        /// mi assicuro che lo studente non abbia già un piano di studio(posso cambiarlo solo con la update)
                        if (isOk) {
                            try {
                                stud.addStudyPlanPerStudent(acYearRegistration, courses, true);
                            } catch (std::invalid_argument &err) {
                                std::string generalError = err.what();
                                error.append(generalError + "Controllare riga: " + std::to_string(line_counter) + "\n");
                                doDbWrite = false;
                            }
                            ///registro lo studente per la prima volta nell'anno specifico dei corsi del suo piano di studio
                            try {
                                registerStudentsToSpecificYearCourses(courses, stud, acYearRegistration, line_counter);
                            } catch (std::invalid_argument &err) {
                                error.append(err.what());
                                doDbWrite = false;
                            }
                        }
                    }
                }
            }
        }
        line_counter++;
    }
    fileIn.close();
    if(doDbWrite) {
        dbStudyPlanWrite();
        dbCourseWrite();//BISOGNA FARLO??
    }else{
        throw std::invalid_argument(error);
    }
}

void University::dbStudyPlanWrite() {
    std::fstream fout;
    fout.open("db_piano_studi.txt", std::fstream::out | std::fstream::trunc);
    for (auto iterStud = _students.begin(); iterStud != _students.end(); iterStud++) {
        Student stud = _students.at(iterStud->first);//salvo in un oggetto Student temporaneo, l'intero oggetto puntato da iterStud
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
        _students.at(id).readStudyPlanPerStudent(acYearRegistration, courses);
        registerStudentsToSpecificYearCourses(courses, stud, acYearRegistration, line_counter);
        line_counter++;
    }
    fileIn.close();
}

///aggiornamento dei piani di studio
void University::updateStudyPlan(const std::string& fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento nuovi piani di studio \n");
    }
    std::string line;
    std::string error;
    bool doDbWrite = true;
    int line_counter = 1;
    while (std::getline(fileIn, line)) {
        if(line.empty() == false) {
            int id = 0;
            bool isOk = true;
            std::vector<int> posSemiColon = Parse::posSemiColon(line);
            std::string acYearRegistration;
            bool canBeAnAcYear = true;

            ///controlli sul primo campo di informazioni, matricola
            std::string matr = line.substr(0, posSemiColon[0]);
            bool canBeAnId = Parse::controlItCanBeAnId(matr, 6, 's');

            if (matr.empty()) {
                error.append("Non e' stata dichiarata la matricola dello studente al rigo: " + std::to_string(line_counter) + "\n");
                doDbWrite = false;
                isOk = false;
            } else if (canBeAnId == false) {
                ///se non è un numero a 6 cifre e la prima lettera non inizia con 's'
                error.append("Il primo campo di informazioni alla riga " + std::to_string(line_counter) +  " non puo' essere una matricola di uno studente \n");
                doDbWrite = false;
                isOk = false;
            } else {
                id = Parse::getMatr(matr);
            }

            ///se la matricola è ok faccio il controllo sul secondo campo di informazioni: anno accademico
            if(isOk) {
                acYearRegistration = line.substr(posSemiColon[0] + 1, posSemiColon[1] - posSemiColon[0] - 1);
                if (acYearRegistration.empty()) {
                    //se il campo dell'anno accademico è vuoto
                    error.append("Non e' stato dichiarato l'anno accademico alla riga " + std::to_string(line_counter) + "\n");
                    doDbWrite = false;
                    isOk = false;
                } else {
                    try {
                        canBeAnAcYear = Parse::controlItCanBeAnAcYear(acYearRegistration);
                    } catch (std::invalid_argument &err) {
                        std::string genericError = err.what();
                        error.append("Controllare riga: " + std::to_string(line_counter) + ". " + genericError);
                        doDbWrite = false;
                        isOk = false;
                    }
                    if (canBeAnAcYear == false) {
                        error.append("Anno accademico non valido al rigo: " + std::to_string(line_counter) + "\n");
                        doDbWrite = false;
                        isOk = false;
                    }
                }
            }

            /// se anno e matricola sono ok continuo con i controlli sul piano di studio
            if (isOk) {
                std::vector<int> pos = Parse::posCurlyBrackets(line);
                //mi assicuro che le parentesi graffe siano messe al posto giusto
                try {
                    Parse::controlStudyPlanFormat(pos, line);
                } catch (std::invalid_argument &err) {
                    std::string genericError = err.what();
                    error.append("Al rigo " + std::to_string(line_counter) + genericError);
                    doDbWrite = false;
                    isOk = false;
                }

                if (isOk) {
                    //se anche le parentesi graffe sono al posto giusto controllo i corsi
                    std::string allCourses = line.substr(posSemiColon[1] + 2, pos[1] - (posSemiColon[1] + 2));
                    if (_students.find(id) == _students.end()) {
                        error.append("Lo studente con matricola " + matr + " al rigo " + std::to_string(line_counter) +
                                     " non e' presente nel database \n");
                        doDbWrite = false;
                    } else {
                        Student &stud = _students.at(id);
                        std::vector<std::string> courses = Parse::splittedLine(allCourses, ';');
                        if (allCourses.size() == 0) {
                            ///se non ho inserito alcun piano di studio da aggiornare
                            error.append("Al rigo " + std::to_string(line_counter) +
                                         "non e' stato dichiarato alcun corso da aggiornare \n");
                            doDbWrite = false;
                            isOk = false;
                        } else {
                            //controllo il formato dei corsi
                            //controllo che non ci siano caratteri non voluti tra i corsi
                            bool continueLoop = true;
                            for (int i = 0; i < allCourses.size() && continueLoop; i++) {
                                if (!((allCourses[i] >= 48 && allCourses[i] <= 57) || allCourses[i] == 59 ||
                                      (allCourses[i] >= 65 && allCourses[i] <= 90))) {
                                    error.append("Al rigo " + std::to_string(line_counter) +
                                                 " sono presenti caratteri non conformi con il formato del piano di studio \n");
                                    doDbWrite = false;
                                    isOk = false;
                                    continueLoop = false;
                                }
                            }
                            if (isOk) {
                                for (int i = 0; i < courses.size(); i++) {
                                    try {
                                        Parse::controlItCanBeACourseId(courses[i]);
                                    } catch (std::invalid_argument &err) {
                                        std::string genericError = err.what();
                                        error.append(genericError + ". Il codice " + courses[i] + " al rigo " +
                                                     std::to_string(line_counter) +
                                                     " non puo' essere il codice di un corso \n");
                                        doDbWrite = false;
                                        isOk = false;
                                    }
                                }
                            }
                        }
                        /// mi assicuro che lo studente non abbia già un piano di studio(posso cambiarlo solo con la update)
                        if (isOk) {
                            try {
                                stud.addStudyPlanPerStudent(acYearRegistration, courses, false);
                            } catch (std::invalid_argument &err) {
                                std::string generalError = err.what();
                                error.append("Lo studente con matricola " + matr + " alla riga " +
                                             std::to_string(line_counter) + generalError);
                                doDbWrite = false;
                            }
                            if(isOk) {
                                ///registro lo studente per la prima volta nell'anno specifico dei corsi del suo piano di studio
                                try {
                                    registerStudentsToSpecificYearCourses(courses, stud, acYearRegistration,
                                                                          line_counter);
                                } catch (std::invalid_argument &err) {
                                    error.append(err.what());
                                    doDbWrite = false;
                                }
                            }
                        }
                    }
                }
            }
        }
        line_counter++;
    }
    fileIn.close();
    if(doDbWrite) {
        dbStudyPlanWrite();
        dbCourseWrite();//BISOGNA FARLO??
    }else{
        throw std::invalid_argument(error);
    }
}

void University::insertStudentsGrades(std::string fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento voti. \n");
    }
    ///folder/<id_corso>_<data_appello>(_[*]).csv
    std::string base_filename = fin.substr(fin.find_last_of("/\\") + 1);
    std::string idCorso = base_filename.substr(0, 7);
    std::string appealDate = base_filename.substr(8, 10);
    std::string error;
    int doDbWrite = true;
    //controllo che il corso esista
    if (_courses.find(idCorso) == _courses.end()) {
        throw std::invalid_argument("il seguente corso non esiste" + idCorso + "impossibile assegnare i voti \n");
    }
    Course &corso = _courses.at(idCorso);
    //controllo che la data esista
    corso.controlAppeal(appealDate);
    std::string year(appealDate.substr(0, 4));
    int appealYear;
    std::string line;
    int line_counter = 1;
    while (std::getline(fileIn, line)) {
        if(line.empty() == false) {
            std::vector<std::string> infoStud = Parse::splittedLine(line, ';');
            if (infoStud.size() != 2) {
                error.append("Errore formato file al rigo: " + std::to_string(line_counter) + "\n");
            } else {
                std::string idStud = infoStud[0];
                int id;
                bool matrIsOk = Parse::controlItCanBeAnId(idStud, 6, 's');
                bool markIsOk = true;
                ///se il primo campo è vuoto ho dimenticato a scrivere la matricola nel file
                if (idStud.empty()) {
                    error.append(
                            "Non e' stata dichiarata la matricola dello studente al rigo: " +
                            std::to_string(line_counter) +
                            "\n");
                    doDbWrite = false;
                    matrIsOk = false;
                } else if (matrIsOk == false) {
                    /// se il primo campo non è vuoto devo capire se è una matricola o no
                    error.append(" Il primo campo alla riga " + std::to_string(line_counter) +
                                 " non puo' essere l'id di uno studente \n");
                    doDbWrite = false;
                    matrIsOk = false;
                } else {
                    ///se sono arrivato qui vuol dire che il primo campo puo' essere una matricola(tutti int e formato esatto per gli studenti)
                    id = Parse::getMatr(idStud);
                }
                ///controllo il voto
                std::string markString = infoStud[1];
                int mark;
                if (markString.empty()) {
                    error.append(
                            "Non e' stata dichiarato il voto dello studente al rigo: " + std::to_string(line_counter) +
                            "\n");
                    doDbWrite = false;
                    markIsOk = false;
                } else {/// se il primo campo non è vuoto devo capire se è un voto o no
                    try {
                        mark = Parse::checkedStoi(markString, "Errore voto studente.");
                        if (mark < 0 || mark > 32) {
                            error.append("Il voto alla riga: " + std::to_string(line_counter) +
                                         " non e' compreso in un intervallo tra 0 e 32 \n");
                            markIsOk = false;
                        }
                    } catch (std::invalid_argument &err) {
                        error.append(" Il secondo campo alla riga: " + std::to_string(line_counter) +
                                     " non puo' essere il voto di un esame\n");
                        doDbWrite = false;
                        markIsOk = false;
                    }
                }
                //controllo che lo studente esista nel database
                if (matrIsOk && markIsOk) {
                    if (_students.find(id) == _students.end()) {
                        error.append(
                                "lo studente alla riga " + std::to_string(line_counter) + " con matricola " + idStud +
                                " non esiste nel database\n");
                        doDbWrite = false;
                    } else {
                        Student &stud = _students.at(id); //preso l'istanza dello studente di cui si parla
                        //prendiamo l'anno di iscrizione dello studente
                        //(perchè le info sull'esame fatto dallo studente sono salvate nello specifico anno di ogni corso a cui è iscritto il cui specifico anno è proprio l'anno di iscrizione dello studente)
                        int acYear = stud.getYearRegistration();
                        try {
                            _courses.at(idCorso).modifyStudentAsPassedToSpecYearCourse(acYear, stud, appealYear, mark,
                                                                                       appealDate);
                        } catch (std::invalid_argument &err) {
                            std::string generalError = err.what();
                            error.append(
                                    "Lo studente alla riga " + std::to_string(line_counter) + " con matricola " +
                                    idStud +
                                    generalError);
                            doDbWrite = false;
                        }
                    }
                }
            }
        }
     line_counter++;
    }
    fileIn.close();
    if(doDbWrite)
        dbAppealsWrite();
    else
        throw std::invalid_argument(error);
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
        int distance = Parse::checkedStoi(infoRequest[3],"Errore distanza");
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
        int distance = Parse::checkedStoi(infoRequest[2],"Errore distanza.");
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
    std::vector<int> startSlotPerAppeal;
    std::vector<std::string> classroomsPerAppeal;
    int numSlot = _courses.at(idCorso).getExamSlotPerYear(acYear);
    int startAcYear = Parse::getAcStartYear(acYear);
    std::vector<int> allProfsPerYearCourse = _courses.at(idCorso).getProfsPerYear(acYear);


    for (int i = 0; i < appealInfo.size(); i++) {
        std::vector<std::string> infoOfSingleAppeal = Parse::splittedLine(appealInfo[i], ',');
        ///prendo le date degli appelli per quella sessione
        appealDate.emplace_back(infoOfSingleAppeal[0]);
        startSlotPerAppeal.emplace_back(Parse::checkedStoi(infoOfSingleAppeal[1],"Errore ora di inizio."));
        classroomsPerAppeal.emplace_back(infoOfSingleAppeal[2]);
        ///segno l'esame nei professori del corso(appello,ora di inizio,numero di slot)
        assignAppealsToProf(idCorso, infoOfSingleAppeal[0], Parse::checkedStoi(infoOfSingleAppeal[1],"Errore ora di inizio."), numSlot,
                            allProfsPerYearCourse);
        ///segno l'esame nelle aule (appello, ora di inizio, numero di slot)
        std::vector<std::string> allClassroomsString = Parse::splittedLine(infoOfSingleAppeal[2], '|');
        std::vector<int> allClassrooms;
        //converto il vettore di stringhe in un vettore di interi
        for(int j = 0; j<allClassroomsString.size();j++){
            allClassrooms.push_back(Parse::checkedStoi(allClassroomsString[j],"Errore aula."));
        }
        assignAppealsToClassroom(infoOfSingleAppeal[0], Parse::checkedStoi(infoOfSingleAppeal[1],"Errore ora di inizio."), allClassrooms, numSlot);
        ///segno l'esame nel calendario (appello, ora di inizio, numero di slot, corso)
        Course &course = _courses.at(idCorso);
        _acYearSessions.at(startAcYear).assignAppealsToCalendar(infoOfSingleAppeal[0], Parse::checkedStoi(infoOfSingleAppeal[1],"Errore ora di inizio"),
                                                                course, numSlot);
    }
    ///salvo le date dell'appello in  _howManyTimesIAmAssignedInASession
    _courses.at(idCorso).assignAppealToSpecificYear(acYear, session, appealDate,startSlotPerAppeal,classroomsPerAppeal);
    Course course = _courses.at(idCorso);
    SpecificYearCourse sp = course.getThisYearCourse(startAcYear);
    std::vector<Date> allAppealPerCourse = sp.getAllAppeals();
    _acYearSessions.at(startAcYear).updateExamDayCourse(course,allAppealPerCourse);
}

void University::assignAppealsToProf(std::string idCorso, std::string appeal, int startHour, int numSlots,
                                     std::vector<int> allProfsPerYearCourse) {
    for (int i = 0; i < allProfsPerYearCourse.size(); i++) {
        for (int num_slot = 0; num_slot < numSlots; num_slot++) {
            _professors.at(allProfsPerYearCourse[i]).addNewExam(appeal, startHour + (num_slot * 2), idCorso);
        }
    }
}

void University::assignAppealsToClassroom(std::string appeal, int startSlotHour, std::vector<int> classrooms, int numSlot) {
    Date appealDate(appeal);
    for (int i = 0; i < classrooms.size(); i++) {
        _classrooms.at(classrooms[i]).setDisavailability(appealDate, startSlotHour, numSlot);
    }
}

void University::requestChanges(std::string acYear, std::string fin) {
    //<id_corso>;<numero_sessione>;<numero_appello>;<direzione_spostamento>;<num_settimane>
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw DbException("Errore apertura del file per le richieste del cambio data esami \n");
    }
    std::string ac = acYear.substr(0,4);
    int acStart = Parse::checkedStoi(ac,"Errore anno accademico.");
    SessionYear& thisSession = _acYearSessions.at(acStart);
    std::string line;
    int successfulChanges = 0; //se >1 alla fine dovrò riscrivere tutto il file sessioni
    bool allChangesArePossible = true;
    int numSession;

    std::string error;
    bool appealsAreAlreadyLoaded = false;
    ///tutti gli appelli delle sessioni (tutti anni accademici indistintamente) sono già caricati in memoria
    while (std::getline(fileIn, line)){
        std::vector<std::string> infoChanges = Parse::splittedLine(line, ';');
        std::string idCourse = infoChanges[0];
        numSession = Parse::checkedStoi(infoChanges[1],"Errore numero sessione.");
        int numAppeal = Parse::checkedStoi(infoChanges[2],"Errore numero appello");
        char directionOfSfift = infoChanges[3][0];
        int numWeeks = Parse::checkedStoi(infoChanges[4],"Errore numero di setimane");

        ///2) cancello le informazioni dell'appello selezionato in questa riga + raggruppati e le salvo
        ///3) cerco di soddisfare il cambiamento
        ///4) se riesco, aggiungo appello alla sessione di questo anno accademico è successfulChanges++
        ///5) se non riesco, devo ripristinare info appello + emit warning
        Date oldDate;
        int startSlot;
        std::map<std::string,std::vector<int>> classrooms;
        //2 -> rimozione + salvataggio
        removeThisAppealAndGroupedInfo(acStart, idCourse, numSession, numAppeal, oldDate, startSlot, classrooms);
        //3
        Course& courseToConsider = _courses.at(idCourse);
        Date tryDate;
        if(directionOfSfift == 'd'){
            tryDate = oldDate.subtract(numWeeks * 7);
        }
        else{
            tryDate = oldDate.add(numWeeks * 7);
        }
        try {
            thisSession.tryToSetThisExamInThisSession(_courses, _professors, _classrooms, courseToConsider, numSession, numAppeal,tryDate);
            //4
            successfulChanges++;
        }
        catch(std::exception& err){
            //5
            ///warning
            ///ripristino le info iniziali
            reassignThisAppealInfo(acStart,idCourse,numSession,numAppeal,oldDate,startSlot,classrooms);
            //segnalo l'errore da lanciare successivamente con il throw
            error.append(err.what());
            allChangesArePossible = false;
        }
    }
    if(allChangesArePossible) {
        //aggiorno il calendario
        for(auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++){
            Course course = _courses.at(iterCourse->second.getId());
            SpecificYearCourse sp = course.getThisYearCourse(acStart);
            std::vector<Date> allAppealPerCourse = sp.getAllAppeals();
            course.getThisYearCourseReference(acStart).eraseNumAppeal();
            _acYearSessions.at(acStart).updateExamDayCourse(course,allAppealPerCourse);
        }
        //leggo e riempio la struttura con i nomi dei file stampati per le sessioni
        readOutputFileName();
        //ristampo la sessione
        bool requestChanges = true;
        //prendo il nome del file per quell'anno e per quella sessione
        std::string fileName = _acYearSessions.at(acStart).getFileName(numSession);
        //rigenero i file di output
        //prima della generazione tutti i _numAppeal devono essere 0 per tutti gli specifcyearCourse

        _acYearSessions.at(acStart).generateOutputFilesSession(fileName, numSession, _courses, requestChanges);
        _acYearSessions.at(acStart).allExamAppealsWrite(_courses);
    } else
        throw std::invalid_argument(error);
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

int University::whatIsMyStudyCourse(Course &courseToConsider, int acStartYear) {
    int sem = 0;
    for (auto iter = _studyCourse.begin(); iter != _studyCourse.end(); iter++) {
        std::vector<std::string> allCoursesOfStudyCourse = iter->second.getAllCoursesOfStudyCourse();
        auto iterCourse = find(allCoursesOfStudyCourse.begin(), allCoursesOfStudyCourse.end(), courseToConsider.getId());
        if(iterCourse != allCoursesOfStudyCourse.end())
            return sem = courseToConsider.getSemesterAtYear(acStartYear,courseToConsider.getId());
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

void University::removeThisAppealAndGroupedInfo(int acYear, std::string idCourse, int numSession, int numAppeal, Date& date, int& startSlot, std::map<std::string,std::vector<int>>& classrooms) {
    SpecificYearCourse& sp = _courses.at(idCourse).getThisYearCourseReference(acYear);
    ///salvo le informazioni per poterle riusare dopo
    //salvo la data(sarà la data per tutti i corsi raggruppati)
    date = sp.dateAssignationInGivenSession(numSession,numAppeal);
    //savo lo slot iniziale(sarà lo slot iniziale per tutti i corsi raggruppati)
    startSlot = sp.startSlotAssignationInGivenSession(numSession,numAppeal);
    //devo prendere le aule per ogni corso raggruppato
    std::vector<std::string> idGroupedCourse = _courses.at(idCourse).getIdGroupedCourseFromYear(acYear);
    idGroupedCourse.push_back(idCourse);
    for(int i = 0; i<idGroupedCourse.size(); i++){
        SpecificYearCourse& spGrouped = _courses.at(idGroupedCourse[i]).getThisYearCourseReference(acYear);
        std::vector<int> classroomPerCourse = spGrouped.classroomsAssignedInGivenSession(numSession,numAppeal);
        classrooms.insert(std::pair<std::string,std::vector<int>>(idGroupedCourse[i],classroomPerCourse));
    }
    ///RIMOZIONE
    ///deve rimuovere le info da SpecificYearCourse, professori relativi, Classroom selezionate, slots occupati
    ///rimuovo dal corso indicato nel file
    ///per fare un unico ciclo aggiungo l'idCourse letto nel file ai suoi raggruppati e ciclo su tutti i corsi da togliere, letto da file e i suoi raggruppati

    for(int i = 0; i<idGroupedCourse.size(); i++){
        SpecificYearCourse& spGrouped = _courses.at(idGroupedCourse[i]).getThisYearCourseReference(acYear);
        Exam examInfo = spGrouped.getExam();
        int numSlots = examInfo.howManySlots();

        spGrouped.removeInfoThisAppealFromSpecificYear(numSession, numAppeal);
        ///professori relativi da SpecificYearCourse trovato
        std::vector<int> profs = spGrouped.getAllProfMatr();
        for(int j=0; j<profs.size();j++){
            Professor& prof = _professors.at(profs[j]);
            prof.eraseThisAppealFromProfs(date, startSlot, numSlots);
        }
        ///Classroom selezionate da SpecificYearCourse trovato
        std::vector<int> rooms = classrooms.at(idGroupedCourse[i]);
        for(int j=0; j<rooms.size();j++){
            Classroom& room = _classrooms.at(rooms[j]);
            room.eraseThisAppealFromClassrooms(date, startSlot, numSlots);
        }
        ///slots occupati in ExamDay in SessionYear (dalla data di quell'appello in SpecificYearCourse)
        _acYearSessions.at(acYear).removeThisAppealInfoFromCalendar(numSlots, date, startSlot, idGroupedCourse[i]);
    }

}

void University::reassignThisAppealInfo(int acYear, std::string idCourse, int numSession, int numAppeal, Date date, int startSlot, std::map<std::string, std::vector<int>> classrooms) {
    SessionYear& thisSession = _acYearSessions.at(acYear);
    SpecificYearCourse& sp = _courses.at(idCourse).getThisYearCourseReference(acYear);
    std::vector<std::string> idGroupedCoursePerThisLoop = sp.getIdGroupedCourses();
    ///corsi a cui riassegnare le info
    idGroupedCoursePerThisLoop.push_back(idCourse);
    ///numero di slot
    int numSlot = _courses.at(idCourse).getExamSlotPerYear(std::to_string(acYear) + "-" + std::to_string(acYear + 1));
    for(int i = 0 ; i<idGroupedCoursePerThisLoop.size(); i++) {
        //prendo i prof a cui assegnare la data e segno per tutti i prof
        std::vector<int> allProfsPerYearCourse = _courses.at(idGroupedCoursePerThisLoop[i]).getProfsPerYear(std::to_string(acYear) + "-" + std::to_string(acYear + 1));
        assignAppealsToProf(idGroupedCoursePerThisLoop[i], date.toString(), startSlot, numSlot, allProfsPerYearCourse);

        ///segno l'esame nelle aule (appello, ora di inizio, numero di slot)
        std::vector<int> classroomsPerCourse = classrooms.at(idGroupedCoursePerThisLoop[i]);
        assignAppealsToClassroom(date.toString(), startSlot,classroomsPerCourse, numSlot);

        ///salvo le date dell'appello in  _howManyTimesIAmAssignedInASession
        _courses.at(idGroupedCoursePerThisLoop[i]).reassignAppealToSpecificYear(acYear,numAppeal,numSession,date,startSlot,classroomsPerCourse);

        ///segno l'esame nel calendario (appello, ora di inizio, numero di slot, corso)
        Course &course = _courses.at(idGroupedCoursePerThisLoop[i]);
        _acYearSessions.at(acYear).assignAppealsToCalendar(date.toString(),startSlot,course, numSlot);
    }
}

std::map<int, Professor> University::getProfessors() const {
    return _professors;
}

std::map<int, Classroom> University::getClassrooms() const {
    return _classrooms;
}

std::map<std::string, Course> University::getCourses() const {
    return _courses;
}

void University::setProfessors(const std::map<int, Professor> &professors) {
    _professors = professors;
}

void University::setClassrooms(const std::map<int, Classroom> &classrooms) {
    _classrooms = classrooms;
}

void University::setCourses(const std::map<std::string, Course> &courses) {
    _courses = courses;
}









































































