//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include <algorithm>
#include "StudyCourse.h"
#include "Parse.hpp"
#include "DbException.h"
#include "InvalidDbException.h"

StudyCourse::StudyCourse(const int id, const bool &isBachelor) : _id{id}, _isBachelor{isBachelor} {}

///aggiunge un semestre con i relativi corsi al corso di studio
void StudyCourse::addSemesterCourses(const int year, const int semester, const std::string &semesterCourses,
                                     const std::map<int, StudyCourse> &studyCourse, int posFile) {

    ///key
    std::stringstream ss;
    ss << year << "-" << semester;
    std::string key = ss.str(); //creo la chiave con anno e semestre
    std::string error;
    ///create values
    std::vector<std::string> courses;
    courses = Parse::splittedLine(semesterCourses,',');//adesso ho i corsi del semestre passati alla funzione che non erano divisi

    ///analizzo tutti i componenti del vettore courses
    for (auto iterCourses = courses.begin(); iterCourses != courses.end(); iterCourses++) {
        ///dobbiamo controllare che questo corso non esista già all'interno di questo study course
        if (_semesters.empty() ==  false) {//se sono ancora all'inserimento del primo corso del primo semestre la mappa è vuota
            std::vector<std::string> allCoursesSoFar = getAllCoursesOfStudyCourse();
            int myCount = std::count(allCoursesSoFar.begin(), allCoursesSoFar.end(), *iterCourses);
            if (myCount != 0) {
                ///il corso esiste già
                error.append("Il corso " + *iterCourses + " e' gia' presente nel corso di studio alla riga: " +
                        std::to_string(posFile));
            }
        }
        ///se abbiamo uno stesso corso in più cds, devo controllare che sia presente allo stesse semestre tra tutti i cds
        sameSemester(*iterCourses, studyCourse, semester, posFile);

        if (!_semesters.count( key)) {//se la chiave non esiste(non ho aggiunto ancora corsi per quel semestre di quell'anno)
            std::vector<std::string> vect;
            vect.push_back(courses[0]);//salvo il primo corso del semestre
            _semesters.insert(std::pair<std::string, std::vector<std::string>>(key,
                                                                               vect));//inserisco nella mappa il primo corso del semestre
        } else {//se la chiave esiste
            _semesters.at(key).push_back(
                    *iterCourses);//aggiungo nel semestre già esistenete i corsi successivi al primo
        }
    }

}

///prende tutti i corsi del corso di studio
std::vector<std::string> StudyCourse::getAllCoursesOfStudyCourse() const {
    std::vector<std::string> allCourses;
    for (auto iterSemester = _semesters.begin(); iterSemester != _semesters.end(); iterSemester++) {
        std::vector<std::string> sem = iterSemester->second;
        allCourses.insert(allCourses.end(), sem.begin(), sem.end());
    }
    for (auto iterOffCourses = _offCourses.begin(); iterOffCourses != _offCourses.end(); iterOffCourses++) {
        allCourses.push_back(*iterOffCourses);
    }
    return allCourses;
}

///aggiunge un corso spento
void StudyCourse::addOffCourses(const std::vector<std::string> &offCourses) {
    //associo ogni corso spento a _offCourses
    for (int i = 0; i < offCourses.size(); i++) {
        _offCourses.push_back(offCourses[i]);
    }
    std::sort(_offCourses.begin(), _offCourses.end());//se li vogliamo in ordine crescente;

}

///controlla se ci sono corsi spenti: corsiSpenti è vuoto?
bool StudyCourse::offCoursesEmpty() const {
    return _offCourses.empty();
}

///aggiorna semestri e corsi spenti. Se un corso diventa spento dovrò toglierlo dal semestre a cui era assegnato
void StudyCourse::updateSemestersAndOffCourses(const std::string &idCourse, std::string &acYY,
                                               std::vector<std::string> &temp) {
    for (auto iterSemesters = _semesters.begin(); iterSemesters != _semesters.end(); iterSemesters++) {
        //prendo il vettore con i codici del corso
        std::vector<std::string> &corsi = iterSemesters->second;
        //cerco il codice che deve diventare spento
        auto found = std::find(corsi.begin(), corsi.end(), idCourse);
        if (found != corsi.end()) {
            std::stringstream ss;
            ss << idCourse << ";" << acYY << ";" << iterSemesters->first;
            ///aggiorno temp ---> aggiungo "<codOffCourse>;<acYear(in cui è stato spento)>;<year>-<semester>"
            temp.push_back(ss.str());
            ///tolgo il corso dai semestri e lo aggiungo a offCourses
            corsi.erase(found);
            _offCourses.push_back(idCourse);
        }
    }
}

///prende l'_id
int StudyCourse::getId() const {
    return _id;
}

///prende il tipo di corso di studio: triennale o magistrale
bool StudyCourse::getIsBachelor() const {
    return _isBachelor;
}

///prende l'intero semestre sottoforma di stringa
std::string StudyCourse::getSemestersString() const {
    std::stringstream output;
    auto iterSemester = _semesters.begin();

    output << "[";
    for (int i = 0; i < _semesters.size(); i++) {//per ogni semestre
        output << "{";

        int numCoursesSemester = iterSemester->second.size();//numero di corsi del semestre
        ///streammo su output ogni corso del semestre
        for (int j = 0; j < numCoursesSemester; j++) {
            output
                    << iterSemester->second[j];//l'iteratore punterà al j-esimo elemento del vettore di stringhe, value di ogni _semester della mappa, contenente i codici dei corsi
            if (j < numCoursesSemester - 1) //se è l'ultimo corso del semestre non va messa la virgola finale
                output << ",";
        }
        iterSemester++;
        output << "}";
        if (i < _semesters.size() - 1)//se l'ultimo semestre non va messa la virgola
            output << ",";
    }
    output << "];";

    return output.str();//ritorno la stringstream sottoforma di stringa
}

///prende i corsi spenti
std::string StudyCourse::getOffCoursesString() const {
    std::stringstream output;
    int countCourses = 0;
    output << "[";
    for (auto iterOffCourses = _offCourses.begin();
         iterOffCourses != _offCourses.end(); iterOffCourses++) {//per ogni corso spento
        output << *iterOffCourses;//dereferenzio l'iteratore: prendo il value puntato dall'iteratore
        if (countCourses < _offCourses.size() - 1)//se l'ultimo corso non va messa la virgola
            output << ",";
        countCourses++;
    }
    output << "]";
    return output.str();//ritorno la stringstream sottoforma di stringa
}

///controlla se un corso presente in altri corsi di studio è posto sempre allo stesso semestre
void StudyCourse::sameSemester(std::string idCourse, const std::map<int, StudyCourse> &studyCourse, int semester,
                               int posFile) {
    std::string error;
    bool same = true;
    int sem = 0;
    for (auto iterStudyCourse = studyCourse.begin(); iterStudyCourse != studyCourse.end(); iterStudyCourse++) {
        std::string result = iterStudyCourse->second.isInWhichSemester(idCourse);
        if (result != "") {
            sem = stoi(result.substr(2, 1));
            if (semester != sem) {
                error.append("Errore corso " + idCourse + " presente alla riga " + std::to_string(posFile) + ".Stessi corsi in corsi di studio differenti devono appartenere ad uno stesso semestre\n");
                same = false;
            }
        }
    }
    if(same == false){
        throw std::logic_error(error);
    }
}

///prendo semestre e anno di un corso associato ad un corso di studio
std::string StudyCourse::isInWhichSemester(std::string codCourse) const {
    //controllo tutti i semestri del corso di studio
    for (auto iterSemester = _semesters.begin(); iterSemester != _semesters.end(); iterSemester++) {
        auto iterCourse = find(iterSemester->second.begin(), iterSemester->second.end(),
                               codCourse);//punto ai corsi di un semestre e cerco tra questi il corso passato e di cui dovrò assegnare l'esame
        if (iterCourse != iterSemester->second.end()) {//se il corso è presente in uno dei semestri
            return iterSemester->first; //ne prendo il semestre e anno (primo o secondo)
        }
    }
    return "";
}

bool StudyCourse::assignStudyCourse(std::string course) {
    std::vector<std::string> allCourses = getAllCoursesOfStudyCourse();
    return std::find(allCourses.begin(), allCourses.end(), course) != allCourses.end();
}



///overload operatore <<
std::ostream &operator<<(std::ostream &studC, const StudyCourse &s) {
    std::string settedId = Parse::setId('C', 3, s.getId());
    studC << settedId << ";";
    if (s.getIsBachelor())
        studC << "BS;";
    else
        studC << "MS;";
    studC << s.getSemestersString(); //stringa semestri
    if (!s.offCoursesEmpty()) //ci sono corsi spenti?
        studC << s.getOffCoursesString();//se ci sono corsi spenti allora prendili
    return studC;
}

