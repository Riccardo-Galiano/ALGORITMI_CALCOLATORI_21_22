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
        readProfsNoAvailability();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
    try {
        readCourseNotActive();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }try {
        readStudyPlan();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }try {
        readPassedAppeals();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }try {
        readAllExamAppeals();
    }
    catch (DbException &exc) {
        std::cerr << exc.what() << std::endl;
    }
}

///leggo il database degli studenti
void University::readStudents() {
    std::ifstream fileIn("db_studenti.txt");
    ///leggi version
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
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
            throw std::logic_error("due matricole uguali");
        else
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, InteroStudente[1], InteroStudente[2],
                                                                    InteroStudente[3])));//la chiave intera è la matricola; ad ogni chiave/matricola è associato uno studente
    }

    fileIn.close();

}

///identico alla readStudents(); si evita di commentare per non sporcare il codice
void University::readProfessor() {
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
        else
            _professors.insert(std::pair<int, Professor>(nMatr,
                                                         Professor(nMatr, InteroProfessore[1], InteroProfessore[2],
                                                                   InteroProfessore[3])));
    }
    fileIn.close();
}

///lettura delle aule dal database
void University::readClassroom() {
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
        else {
            _classroom.insert(std::pair<int, Classroom>(nCod, Classroom(nCod, InteraClasse[1], InteraClasse[2],
                                                                        std::stoi(InteraClasse[3]),
                                                                        std::stoi(InteraClasse[4]))));
        }
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
bool University::addStuds(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in); //apro il file in lettura
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura file di inserimento nuovi studenti");
        return false;
    }
    int line_counter = 1;
    std::string line; //stringa di appoggio per l'intera riga del file
    std::vector<std::string> tokens; //vettore di stringhe che accoglierà il ritorno della funzione splittedLine
    while (std::getline(fIn, line)) {//fino alla fine del file leggo un rigo alla volta
        tokens = Parse::splittedLine(line, ';');
        //controllo che formato file sia corretto: 3 campi
        if (tokens.size() != 3) {
            throw DbException("errore formato file studenti alla riga: ", line_counter);
        }
        int matr = getNewStudentId(); //calcolo la matricola del nuovo studente
        _students.insert(std::pair<int, Student>(matr, Student(matr, tokens[0], tokens[1],
                                                               tokens[2]))); //inserisco il nuovo studente nella mappatura interna
        line_counter++;
    }
    fIn.close();

    dbStudsWrite();
    std::cout << "comando -a:s correttamente eseguito" << std::endl;
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
    int line_counter = 1;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = Parse::splittedLine(line, ';');
        //controllo che formato file sia corretto: 3 campi
        if (tokens.size() != 3) {
            throw DbException("errore formato file professori alla riga: ", line_counter);
        }
        int matr = getNewProfessorId();
        _professors.insert(std::pair<int, Professor>(matr, Professor(matr, tokens[0], tokens[1], tokens[2])));
        line_counter++;
    }
    fIn.close();

    dbProfsWrite();
    std::cout << "comando -a:d correttamente eseguito" << std::endl;

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
    int line_counter = 1;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = Parse::splittedLine(line, ';');
        //controllo che formato file sia corretto: 4 campi
        if (tokens.size() != 4) {
            throw DbException("errore formato file aule alla riga: ", line_counter);
        }
        int id = getNewClassroomId();
        _classroom.insert(std::pair<int, Classroom>(id, Classroom(id, tokens[0], tokens[1], std::stoi(tokens[2]),
                                                                  std::stoi(tokens[3]))));
        line_counter++;
    }
    fIn.close();

    dbClassRoomWrite();
    std::cout << "comando -a:a correttamente eseguito" << std::endl;

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
    int line_counter = 1;
    std::vector<std::string> tokens;    //accoglierà il vettore con la riga del file scissa
    while (std::getline(fileIn, line)) {
        ///codice, livello
        tokens = Parse::splittedLine(line, ';');//inserisco i vari campi delimitati dal ;
        if (tokens.size() != 2) {
            throw DbException("errore formato file corsi di studio alla riga: ", line_counter);
        }
        int codCorso = getNewStudyCourseId();

        std::string levelCourse = tokens[0];//triennale o magistrale
        ///devo leggere i semestri
        std::vector<int> posSem;
        ///cerco nella stringa se ci sono i due caratteri inseriti nella find_first_of
        std::size_t found = tokens[1].find_first_of("{}");
        while (found !=
               std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            posSem.push_back(
                    found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = tokens[1].find_first_of("{}", found + 1);//continuo a controllare la stringa
        } //alla fine di questo while posSem conterrà le posizioni in corrispondenza delle quali nel vettore tokens[1] sono presenti: { o }

        std::vector<std::string> semestri;
        for (i = 0; i < posSem.size() - 1; i = i +
                                               2) { //metto +2 perchè, devo andare da una parentesi graffa che apre ad una che chiude
            int posStart = posSem[i] + 1;// tolgo la graffa
            int len = posSem[i + 1] - posSem[i] - 1; //pos(}) - pos({) -1
            semestri.push_back(tokens[1].substr(posStart,
                                                len));   //salvo la sottostringa dal valore successivo al carattere cercato dalla find_first_of fino al valore precedente alla posizione del successivo carattere trovato
        } //alla fine di questo for il vector "semestre" conterrà i corsi di ogni semestre disposti al suo interno in modo che ogni "cella" di "semestre" contiene tutti i corsi di un certo semestre
        ///semestre[0] = tutti i corsi di anno1_semestre1, semestre[1] = tutti i  corsi anno1_semestre2, semestre[2] = tutti i  corsi anno2_semestre1, ...
        ///controllo che formato file sia corretto:
        ///se L3 -> 6 semestri, se LM -> 4 semestri
        if ((levelCourse == "BS" && semestri.size() != 6) || (levelCourse == "MS" && semestri.size() != 4)) {
            throw DbException(
                    "formato file corsi di studio non valido: ci sono semestri senza corsi o numero semestri incompatibile con tipo di laurea alla riga:",
                    line_counter);
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
            SCourse.addSemesterCourses(year, numSemester, semestri[i], _studyCourse, _courses,
                                       line_counter);//passo: l'anno, primo o secondo semestre,tutta la stringa di corsi del semestre
        }
        _studyCourse.insert(std::pair<int, StudyCourse>(codCorso, SCourse));
        line_counter++;
    }
    ///controllo non ci siano buchi, se c'è mi ritorna eccezione
    thereIsAHoleInTheCoursesCodes();
    fileIn.close();
    dbStudyCourseWrite();
    std::cout << "comando -a:f correttamente eseguito" << std::endl;
    return true;
}

///inserisco dei nuovi corsi
bool University::addCourses(const std::string &fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {

        throw std::invalid_argument("errore apertura file inserimento nuovi corsi");
        return false;
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

    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        specificYearCourse = Parse::splittedLine(line, ';');
        //controllo che il formato file sia corretto: 10 campi
        if (specificYearCourse.size() != 10) {
            throw DbException("formato file corsi non valido alla riga: ", line_counter);
        }
        //controllo che tuttii campi siano specificati
        for (int i = 0; i < specificYearCourse.size(); i++) {
            if (specificYearCourse[i].empty()) {
                throw DbException("uno o più campi sono vuoti alla riga: ", line_counter);
            }
        }
        //controllo che l'esame non sia già presente in base dati
        //cerco nel DB un esame con stesso titolo e cfu
        for (auto iterCours = _courses.begin(); iterCours != _courses.end(); iterCours++) {
            if (iterCours->second.getName() == specificYearCourse[1] &&
                iterCours->second.getCfu() == stoi(specificYearCourse[2])) {
                throw DbException("c'è un corso già presente in base dati alla riga: ", line_counter);
            }
        }
        std::string newIdCourse = getNewCourseId();
        _courses.insert(std::pair<std::string, Course>(newIdCourse, Course(newIdCourse, specificYearCourse[1],
                                                                           stoi(specificYearCourse[2]),
                                                                           stoi(specificYearCourse[3]),
                                                                           stoi(specificYearCourse[4]),
                                                                           stoi(specificYearCourse[5]))));

        acYear = specificYearCourse[0]; //anno accademico
        int startAcYear = Parse::getAcStartYear(specificYearCourse[0]);

        num_parallel_courses = stoi(specificYearCourse[6]);//numero di corsi in parallelo
        profSenzaQuadre = specificYearCourse[7].substr(1, specificYearCourse[7].size() -
                                                          2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
        std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre,
                                                                  num_parallel_courses);//divido i vari corsi in parallelo
        examData = specificYearCourse[8];//informazioni sull'esame
        examData = examData.substr(1, examData.size() - 2);//tolgo le { } che racchiudono le info degli esami
        splittedExamData = Parse::splittedLine(examData, ',');//scissione info esami
        idGroupedCourse = specificYearCourse[9];//id dei vari corsi raggruppati
        idGroupedCourse = idGroupedCourse.substr(1, idGroupedCourse.size() - 2);// tolgo le { } che racchiudono gli id
        idGrouped = Parse::splittedLine(idGroupedCourse, ',');//scissione degli id dei corsi raggruppati
        ///ricerca "anno-semestre" di questo corso
        std::string yy_semester;
        std::vector<int> studyCourse;
        if (_studyCourse.empty())
            throw InvalidDbException("Per inserire nuovi corsi, devi prima definire i corsi di studio relativi");
        for (int i = 1; i <= _studyCourse.size(); i++) {
            std::string result = _studyCourse.at(i).isInWhichSemester(newIdCourse);
            if (!result.empty()) {
                //ho trovato il suo corso di studi

                auto iterStudyCourse = _studyCourse.find(i); //prendo id del corso di studio associato
                studyCourse.push_back(iterStudyCourse->first);
                yy_semester = result;
                ///controllo i corsi raggruppati (genera eccezione se errore)
                controlGroupedCourses(i, idGrouped, specificYearCourse[1], line_counter, newIdCourse);
            }
        }
        if (yy_semester.empty())
            throw InvalidDbException(
                    "un corso deve essere associato ad almeno un corso di studio! Codice del corso non utilizzato:",
                    newIdCourse);
        _courses.at(newIdCourse).addSpecificYearCourses(acYear, isActive, num_parallel_courses, profCorsoPar,
                                                        splittedExamData, idGrouped, yy_semester, studyCourse,
                                                        line_counter);
        ///controllo che i professori di questo corso esistano già in _professors
        _courses.at(newIdCourse).controlTheExistenceAndHoursOfProfessors(_professors, Parse::getAcStartYear(acYear));

        line_counter++;
    }
    ///controlli che possono essere fatti solo dopo aver letto tutto il file
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        ///se c'è un gap tra anni prendo il precedente
        iterCourse->second.fillAcYearsEmpty();
        ///se il corso è stato disattivato non può essere riattivato
        iterCourse->second.notActive();
        ///controllo se i corsi raggruppati siano dello stesso semestre del principale
        iterCourse->second.sameSemesterGrouped(_courses);
    }
    ///controllo la proprietà di reciprocità dei corsi raggruppati: se a ragg con c, allora c con a
    controlReciprocyGrouped();
    fileIn.close();
    dbCourseWrite();
    std::cout << "comando -a:c eseguito correttamente" << std::endl;
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
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::vector<std::string> infoStud;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoStud = Parse::splittedLine(line, ';');

        if (infoStud[0].empty()) {
            throw DbException("manca la matricola dello studente alla riga: ", line_counter);
        }
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
        line_counter++;
    }

    fileIn.close();
    dbStudsWrite();
    std::cout << "comando -u:s correttamente eseguito" << std::endl;
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
    std::string line;//stringa di appoggio in cui mettere l'intero rigo
    int line_counter = 1;
    std::vector<std::string> infoProf;
    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoProf = Parse::splittedLine(line, ';'); //splitto il rigo nei vari campi di interesse
        if (infoProf[0].empty()) {
            throw DbException("manca la matricola del professore alla riga: ", line_counter);
        }
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
        line_counter++;
    }

    fileIn.close();
    dbProfsWrite();
    std::cout << "comando -u:d correttamente eseguito" << std::endl;
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
    int line_counter = 1;
    std::vector<std::string> infoClassroom;

    while (std::getline(fileIn, line)) {//finchè il file non sarà finito

        infoClassroom = Parse::splittedLine(line, ';');
        if (infoClassroom[0].empty()) {
            throw DbException("manca il codice dell' aula alla riga: ", line_counter);
        }
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
        line_counter++;
    }
    fileIn.close();
    dbClassRoomWrite();
    std::cout << "comando -u:a correttamente eseguito" << std::endl;

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
                        "non esistono informazioni riguardo anni precedenti da poter ereditare; manca info sui corsi raggruppati alla riga: ",
                        line_counter);
            } else {
                for (int i = 0; i < 7; i++) {
                    if (specificYearCourse[i].empty()) {
                        throw DbException(
                                "non esistono informazioni riguardo anni precedenti da poter ereditare; specificare tutti i campi alla riga: ",
                                line_counter);
                    }
                }
            }
        }

        acYear = specificYearCourse[1]; //anno accademico
        int acStartYear = stoi(specificYearCourse[1].substr(2, 1));

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
                ///controllo i corsi raggruppati (genera eccezione se errore)
                controlGroupedCourses(i, idGrouped, _courses.at(specificYearCourse[0]).getName(), line_counter,
                                      specificYearCourse[0]);
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
                iterStudyCourse->second.updateSemestersAndOffCourses(specificYearCourse[0],specificYearCourse[1],
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
    ///controllo la proprietà di reciprocità dei corsi raggruppati: se a ragg con c, allora c con a
    controlReciprocyGrouped();

    ///tutto ok, posso aggiornare effettivamente db
    dbCourseNotActive();
    dbCourseWrite();
    dbStudyCourseWrite();
    std::cout << "comando -i:c eseguito correttamente" << std::endl;
    return true;
}

///riscrive il database degli studenti
void University::dbStudsWrite() {
    std::fstream fout;
    fout.open("db_studenti.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterStud = _students.begin(); iterStud != _students.end(); iterStud++) {
        Student stud = _students.at(iterStud->first);//salvo in un oggetto Student temporaneo, l'intero oggetto puntato da iterStud
        fout << stud << std::endl;//grazie all'overload dell'operatore << scrivo su file l'oggetto stud(si rimanda all'overload dell'operatore in Student.cpp)
    }
    fout.close();

}

///riscrive il database dei professori
void University::dbProfsWrite() {

    std::fstream fout;
    fout.open("db_professori.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterProf = _professors.begin(); iterProf != _professors.end(); iterProf++) {
        Professor prof = _professors.at(iterProf->first);

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
        std::stringstream tok;
        tok << sC;
        fout << tok.str() << std::endl;
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
bool University::setSessionPeriod(std::string &acYear, std::string &winterSession,
                                  std::string &summerSession, std::string &autumnSession) {
    std::string prova = acYear;
    int acStartYear = Parse::getAcStartYear(prova);
    if (_acYearSessions.count(acStartYear) != 0) {
        _acYearSessions.erase(acStartYear);
    }
    //creo oggetto session da aggiungere
    std::string output_file_name = std::string("output_file");
    SessionYear sessionYear(acYear, winterSession, summerSession, autumnSession, output_file_name);
    //popolo mappa in university
    _acYearSessions.insert(std::pair<int, SessionYear>(acStartYear, sessionYear));
    dbDateSessionsWrite();
    std::cout << "comando -s current_a correttamente eseguito" << std::endl;
    return true;
}

///set indisponibilità professori in base all'anno
bool University::setProfsNoAvailability(std::string acYear, const std::string &fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw DbException("file availabilities non esistente");
    }
    std::string line;
    std::vector<std::string> profAvailability;
    int nMatr, year;
    year = Parse::getAcStartYear(acYear);

    while (std::getline(fileIn, line)) {//fino alla fine del file leggo un rigo alla volta
        profAvailability = Parse::splittedLine(line, ';');

        nMatr = Parse::getMatr(profAvailability[0]);//prendo la matricola
        auto iterProf = _professors.find(nMatr);//punto al prof con matricola nMatr

        //dovrò aggiornare l'indisponibilità di un prof per uno specifico anno accademico quindi cancello le
        //vecchie indisponibilità di uno specifico prof per uno specifico anno
        iterProf->second.noAvailabilityClear(year);//facciamo il controllo se vuoto????
        for (int i = 1; i < profAvailability.size(); i++) {//per ogni campo della riga letta

            _professors.at(nMatr).setNoAvaibilities(year, profAvailability[i]);//vado a settare uno dei periodi di indisponibilità del prof nella map _professor
        }
    }
    fileIn.close();
    dbNoAvailabilityWrite();
    std::cout << "comando -s set_availability correttamente eseguito" << std::endl;

    return true;
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
bool University::setExamDate(std::string acYear, std::string outputNameFile) {
    int startAcYear = Parse::getAcStartYear(acYear);
    int constraintRelaxParameter = 0;
    bool esito = false;
    controlDatabase(startAcYear);
    ///il ciclo sarà eseguito se le sessioni non sono ancora generate(result==false) e finchè ci saranno ancora vincoli da poter rilassare
    while (!esito && constraintRelaxParameter < 4) {
        //accedo all'anno accademico passato dal comando e genero le sessioni per un anno
        esito = _acYearSessions.at(startAcYear).generateNewYearSession(outputNameFile, _courses, _professors,_classroom,
                                                                       constraintRelaxParameter);
        constraintRelaxParameter++;
    }
    ///se le sessioni non possono essere generate nonostante i vincoli rilassati
    if (!esito) {
        std::cerr << "non è stato possibile generare le date d'esame nonostante i vincoli rilassati" << std::endl;
    }

    std::cout << "comando -g correttamente eseguito" << std::endl;
    return esito;
}

///controlla se i file sono coerenti
bool University::controlDatabase(int startAcYear) {
    ///controllo che i database non siano vuoti
    dataBaseIsEmpty(startAcYear);
    for (auto iterCourse = _courses.begin(); iterCourse != _courses.end(); iterCourse++) {
        ///controllo che il corso abbia info relativamente all'anno accademico richiesto
        iterCourse->second.controlExistenceSpecificYear(iterCourse->first, startAcYear);
    }
    return true;
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
    for(auto iter = _courses.begin(); iter != _courses.end(); iter++){
        int studentsEnrolled = iter->second.getThisYearCourse(startAcYear).getTotStudentsEnrolled();
        if(studentsEnrolled == 0)
            throw InvalidDbException("Il corso " + iter->second.getId() + " non ha studenti iscritti");
    }
    return false;
}

/// controllo che idGrouped NON siano corsi dello stesso CdS
bool University::controlGroupedCourses(int idStudyCourse, std::vector<std::string> &idGrouped,
                                       std::string nameCourse, int line_counter,
                                       std::string idCourse) {
    std::vector<std::string> allCoursesOfCdS = _studyCourse.at(idStudyCourse).getAllCoursesOfStudyCourse();

    for (int j = 0; j < idGrouped.size(); j++) {
        auto found = std::find(allCoursesOfCdS.begin(), allCoursesOfCdS.end(), idGrouped[j]);
        if (found != allCoursesOfCdS.end())
            throw InvalidDbException("stesso corso di studio tra: ", nameCourse, "(corrispondente al corso:",
                                     idCourse, ") e ", idGrouped[j], " alla riga: ", line_counter,
                                     ". Corso di studio non coerente: ", idStudyCourse);

    }
    return true;
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
    if (size_maj != size_min)
        throw std::invalid_argument("errore codice corso!");
    for (int i = size_maj; i >= 0; i--) {
        char lettMaj = major[size_maj - 1]; //caso1: 01AAAAA, caso2: 01AAAAZ
        char lettMin = minor[size_min - 1]; //caso1: 01AAAAB, caso2: 01AAABA
        char penultimaLettMaj = major[size_maj - 2];
        char penultimaLettMin = minor[size_min - 2];
        bool other_equal = true;
        for (int j = 0; j < size_maj - 2; j++) {
            char lettMaj = major[j];
            char lettMin = minor[j];
            if (lettMaj != lettMin)
                other_equal = false;
        }
        int distance = lettMaj - lettMin;
        if (distance == 1 && other_equal && penultimaLettMaj == penultimaLettMin) {
            ///OK
            return;
        } else if (lettMaj == 'A' && lettMin == 'Z' && penultimaLettMaj != penultimaLettMin && other_equal) {
            ///penul diverse, devo controllare che penultima_maj > pen_min
            distance = penultimaLettMaj - penultimaLettMin;
            if (distance == 1)
                ///OK
                return;
        }
    }
    ///se esco devo lanciare eccezione
    std::string error("File non corretto: le stringhe dei corsi non sono consecutive -> (");
    error = error + major;
    error = error + ")";
    throw std::invalid_argument(error.c_str());
}


///controllo la proprietà di reciprocità dei corsi raggruppati: se a ragg con c, allora c con a
//a (b,c,d)
//b (f,a,c,d)
//c (a,b,d)
//d (a,c,b)
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
                        throw InvalidDbException(
                                "Proprieta' di reciprocita' dei corsi non rispettata! Il seguente corso ha dei corsi raggruppati non in comune con gli altri: ",
                                name, ". Il codice assente e':", groupedOfThisYear[k]);
                    groupedOfThisYear2.erase(pos);
                }
                if (groupedOfThisYear2.empty() == false) {
                    throw InvalidDbException(
                            "Proprieta' di reciprocita' dei corsi non rispettata! Il seguente corso ha dei corsi raggruppati in piu': ",
                            name);
                }
            }
        }
    }
}

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
            _courses.at(token[0]).assignYY_Sem(token[1],token[2]);
    }
    fileIn.close();
}

///versione dei database con informazioni aggiuntive
bool University::versioning(std::string v) {
    int version = std::stoi(v);
    renameOldDataBase(version);
    if(version == 2) {
        //studenti e professori
        dbNewStudentWrite();
        dbNewProfessorWrite();
    }
    if(version == 3) {
        //aule
        dbNewAuleWrite();
    }
    return false;
}

///rename del database con _old prima dell'estensione .txt
bool University::renameOldDataBase(int version) {
    if(version == 2) {
        //rename student e professor database
        int result;
        char oldname[] = "db_studenti.txt";
        char newname[] = "db_studenti_old.txt";
        result = rename(oldname, newname);
        if(result != 0)
            throw InvalidDbException("file db_studenti.txt non rinominato adeguatamente");

        //rename student e professor database
        int result2;
        char oldname2[] = "db_professori.txt";
        char newname2[] = "db_professori_old.txt";
        result2 = rename(oldname2, newname2);
        if(result2 != 0)
            throw InvalidDbException("file db_professori.txt non rinominato adeguatamente");
    }
    if(version == 3) {
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

///scrittura nuovo database studenti
void University::dbNewStudentWrite() {
    std::fstream fout;
    fout.open("db_studenti.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterStud = _students.begin(); iterStud != _students.end(); iterStud++) {
        Student stud = _students.at(iterStud->first);//salvo in un oggetto Student temporaneo, l'intero oggetto puntato da iterStud
        std::string otherInfo = stud.getOtherInfoString();
        fout << stud <<";"<< otherInfo << std::endl;
    }
    fout.close();
}

///scrittura nuovo database professori
void University::dbNewProfessorWrite() {
    std::fstream fout;
    fout.open("db_professori.txt", std::fstream::out | std::fstream::trunc);
    for (auto iterProf = _professors.begin(); iterProf != _professors.end(); iterProf++) {
        Professor prof = _professors.at(iterProf->first);
        std::string otherInfo = prof.getOtherInfoString();

        fout << prof << ";"<<otherInfo<< std::endl;
    }
    fout.close();
}

///scrittura nuovo database aule
void University::dbNewAuleWrite() {
    std::fstream fout;
    fout.open("db_aule.txt", std::fstream::out | std::fstream::trunc);

    for (auto iterClassRoom = _classroom.begin(); iterClassRoom != _classroom.end(); iterClassRoom++) {
        Classroom room = _classroom.at(iterClassRoom->first);
        std::string otherInfo = room.getOthersInfo();
        fout << room <<";"<<otherInfo<< std::endl;
    }
    fout.close();
}

bool University::addStudyPlan(std::string fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento nuovi piani di studio");
    }
    std::string line;
    while (std::getline(fileIn, line)){
        std::string matr = line.substr(0,7);
        std::string acYearRegistration = line.substr(8,9);
        std::size_t pos = line.find('}');
        std::string allCourses = line.substr(19,pos-19);
        std::stringstream ss(matr);
        char c;
        int id;
        ss>>c>>id;
        Student& stud = _students.at(id);
        std::vector<std::string> courses = Parse::splittedLine(allCourses,';');
        stud.addStudyPlanPerStudent(acYearRegistration,courses);
        registerStudentsToSpecificYearCourses(courses,stud,stoi(acYearRegistration));
    }
    fileIn.close();
    dbStudyPlanWrite();
    return false;
}

void University::dbStudyPlanWrite() {
    std::fstream fout;
    fout.open("db_piano_studi.txt", std::fstream::out | std::fstream::trunc);
    for (auto iterStud = _students.begin(); iterStud != _students.end(); iterStud++) {
        Student stud = _students.at(iterStud->first);//salvo in un oggetto Student temporaneo, l'intero oggetto puntato da iterStud
        if(stud.studyPlanIsEmpty() == false) {
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
        throw DbException("file db_piano_studi.txt non esistente");
    }
    std::string line;
    while (std::getline(fileIn, line)){
        std::string matr = line.substr(0,7);
        std::string acYearRegistration = line.substr(8,9);
        std::size_t pos = line.find('}');
        std::string allCourses = line.substr(19,pos-19);
        std::stringstream ss(matr);
        char c;
        int id;
        ss>>c>>id;
        Student& stud = _students.at(id);
        std::vector<std::string> courses = Parse::splittedLine(allCourses,';');
        _students.at(id).addStudyPlanPerStudent(acYearRegistration,courses);
        registerStudentsToSpecificYearCourses(courses,stud,stoi(acYearRegistration));
    }
    fileIn.close();
}

void University::updateStudyPlan(std::string fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file aggiornamento piani di studio");
    }
    std::string line;
    while (std::getline(fileIn, line)){
        std::string matr = line.substr(0,7);
        std::string acYearRegistration = line.substr(8,9);
        std::size_t pos = line.find('}');
        std::string allCourses = line.substr(19,pos-19);
        std::stringstream ss(matr);
        char c;
        int id;
        ss>>c>>id;
        std::vector<std::string> courses = Parse::splittedLine(allCourses,';');
        Student& stud = _students.at(id);
        stud.clearStudyPlan();
        stud.addStudyPlanPerStudent(acYearRegistration,courses);
        registerStudentsToSpecificYearCourses(courses,stud,stoi(acYearRegistration));
    }
    fileIn.close();
    dbStudyPlanWrite();
}

bool University::insertStudentsGrades(std::string fin) {
    std::ifstream fileIn(fin);
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura file inserimento voti");
    }
    ///<id_corso>_<data_appello>(_[*]).csv
    std::string idCorso = fin.substr(0,7);
    std::string appealDate = fin.substr(8,12);
    //controllo che il corso esista
    if(_courses.find(idCorso)==_courses.end())
        throw std::invalid_argument("il seguente corso non esiste" + idCorso + "impossibile assegnare i voti");
    Course& corso = _courses.at(idCorso);
    //controllo che la data esista
    corso.controlAppeal(appealDate);
    int appealYear = stoi(appealDate.substr(0,4));
    std::string line;
    while (std::getline(fileIn, line)){
        std::string idmatr = line.substr(0,7);
        int matr = Parse::getMatr(idmatr);
        int mark = stoi(line.substr(8,9));
        //controllo che lo studente esista nel database
        if(_students.find(matr) == _students.end())
            throw InvalidDbException("lo studente "+ idmatr +"non esiste nel database");
        Student& stud = _students.at(matr); //preso l'istanza dello studente di cui si parla
        int acYear = stud.getYearRegistration();
        if(mark>=18) {
            _courses.at(idCorso).modifyStudentAsPassedToSpecYearCourse(acYear, stud, appealYear, mark,appealDate);
        }
    }
    fileIn.close();
    dbAppealsWrite();
    return true;
}

void University::registerStudentsToSpecificYearCourses(std::vector<std::string>& courses, Student& stud, int acYearRegistration) {
    for(int i=0; i<courses.size(); i++) {
        if(_courses.find(courses[i]) == _courses.end()){
            throw std::invalid_argument("questo corso non esiste:" + courses[i]);
        }
        _courses.at(courses[i]).registerStudentsToSpecificYear(acYearRegistration,stud);
    }
}

void University::dbAppealsWrite() {
    std::fstream fout;
    fout.open("db_appelli.txt", std::fstream::out | std::fstream::trunc);
    for(auto iterCourse = _courses.begin(); iterCourse != _courses.end();iterCourse++){
        std::vector<std::string> acYearAppeals = iterCourse->second.getAcYearStudExam();
        for(int i = 0; i<acYearAppeals.size();i++){
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

    while (std::getline(fileIn, line)){
        std::vector<std::string> appeal = Parse::splittedLine(line,';');
        std::string idCorso = appeal[0];
        std::string acYear = appeal[1];
        std::string appealDate = appeal[2];
        int pos = appeal[3].find(']');
        std::string allStudPassedExam = appeal[3].substr(1,pos-1);

        _courses.at(idCorso).assignStudToAppealPerYear(acYear,appealDate,allStudPassedExam);///cambio _grade, _passed;appealPassed
    }
}

void University::readAllExamAppeals() {
    std::ifstream fileIn("allExamAppealsDb.txt");
    if (!fileIn.is_open()) {
        throw DbException("file allExamAppealsDb.txt non esistente");
    }
    std::string line;
    while (std::getline(fileIn, line)){
        std::vector<std::string> appeals = Parse::splittedLine(line, ';');
        std::string idCorso = appeals[0];
        std::string acYear = appeals[1];
        std::string appealsSession = appeals[2];
        _courses.at(idCorso).assignAppealsToSpecificyear(acYear,appealsSession);
        }
}























































