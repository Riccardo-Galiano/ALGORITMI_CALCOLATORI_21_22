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
bool StudyCourse::addSemesterCourses(const int year, const int semester, const std::string &SemesterCourses,const std::map<int, StudyCourse>& studyCourse, std::map<std::string, Course>& universityCourses,int posFile) {
    ///key
    std::stringstream ss;
    ss << year << "-" << semester;
    std::string key = ss.str(); //creo la chiave con anno e semestre

    ///create values
    std::vector<std::string> courses;
    courses = Parse::splittedLine(SemesterCourses,',');//adesso ho i corsi del semestre passati alla funzione che non erano divisi

    ///analizzo tutti i componenti del vettore courses
    for (auto iterCourses = courses.begin(); iterCourses != courses.end(); iterCourses++) {
        ///dobbiamo controllare che questo corso non esista già all'interno di questo study course
        if(_semesters.empty() == false) {//se sono ancora all'inserimento del primo corso del primo semestre la mappa è vuota
            std::vector<std::string> allCoursesSoFar = getAllCoursesOfStudyCourse();
            int num_occ = std::count(allCoursesSoFar.begin(), allCoursesSoFar.end(), *iterCourses);
            if (num_occ != 0) {
                ///il corso esiste già
                throw InvalidDbException("Avere due corsi uguali all'interno dello stesso corso di studi non e' consentito! ",*iterCourses,posFile);
            }
        }
        ///controllo che abbia lo stesso semestre se presente in altri corsi di studio
        sameSemester(*iterCourses,studyCourse,semester);
        ///
        if (!_semesters.count(key)) {//se la chiave non esiste(non ho aggiunto ancora coris per quel semestre di quell'anno)
            std::vector<std::string> vect;
            vect.push_back(courses[0]);//salvo il primo corso del semestre
            _semesters.insert(std::pair<std::string, std::vector<std::string>>(key, vect));//inserisco nella mappa il primo corso del semestre
        } else {//se la chiave esiste
            _semesters.at(key).push_back(*iterCourses);//aggiungo nel semestre già esistenete i corsi successivi al primo
        }
    }
    return true;
}

///prende tutti i corsi del corso di studio
std::vector<std::string> StudyCourse::getAllCoursesOfStudyCourse() {
    std::vector<std::string> allCourses;
    for(auto iterSemester = _semesters.begin();iterSemester != _semesters.end();iterSemester++) {
        std::vector<std::string> sem = iterSemester->second;
        allCourses.insert(allCourses.end(),sem.begin(),sem.end());
    }
    for(auto iterOffCourses = _offCourses.begin(); iterOffCourses != _offCourses.end();iterOffCourses++){
        allCourses.push_back(*iterOffCourses);
    }
    return allCourses;
}

///aggiunge un corso spento
bool StudyCourse::addOffCourses(const std::vector<std::string> &offCourses) {
    //associo ogni corso spento a _offCourses
    for (int i=0; i<offCourses.size(); i++) {
        _offCourses.push_back(offCourses[i]);
    }
    _offCourses.sort();//se li vogliamo in ordine crescente;
    return true;
}

///controlla se ci sono corsi spenti: corsiSpenti è vuoto?
bool StudyCourse::offCoursesEmpty() const {
    return _offCourses.empty();
}

///aggiorna semestri e corsi spenti. Se un corso diventa spento dovrò toglierlo dal semestre a cui era assegnato
bool StudyCourse::updateSemestersAndOffCourses(const std::string &idCourse) {
for(auto iterSemesters = _semesters.begin(); iterSemesters != _semesters.end(); iterSemesters++){
     //prendo il vettore con i codici del corso
     std::vector<std::string> &corsi = iterSemesters->second;
     //cerco il codice che deve diventare spento
     auto found = std::find(corsi.begin(),corsi.end(),idCourse);
        if(found != corsi.end()) {
             corsi.erase(found);
            _offCourses.push_back(idCourse);
        }
    }
    return true;
}

///prende l'_id
const int StudyCourse::getId() const {
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

    output<<"[";
    for(int i = 0 ; i<_semesters.size() ; i++){//per ogni semestre
        output <<"{";

        int numCoursesSemester = iterSemester->second.size();//numero di corsi del semestre
        ///streammo su output ogni corso del semestre
        for(int j = 0;j< numCoursesSemester; j++){
            output<< iterSemester->second[j] ;//l'iteratore punterà al j-esimo elemento del vettore di stringhe, value di ogni _semester della mappa, contenente i codici dei corsi
            if(j< numCoursesSemester-1) //se è l'ultimo corso del semestre non va messa la virgola finale
                output<<",";
        }
        iterSemester++;
        output <<"}";
        if(i<_semesters.size()-1)//se l'ultimo semestre non va messa la virgola
            output <<",";
    }
    output <<"]";

    return output.str();//ritorno la stringstream sottoforma di stringa
}

///prende i corsi spenti
std::string StudyCourse::getOffCoursesString() const {
    std::stringstream output;
    int countCourses = 0;


    output <<"[";
    for( auto iterOffCourses = _offCourses.begin(); iterOffCourses != _offCourses.end() ; iterOffCourses++){//per ogni corso spento
        output<< *iterOffCourses;//dereferenzio l'iteratore: prendo il value puntato dall'iteratore
        if( countCourses< _offCourses.size() - 1)//se l'ultimo corso non va messa la virgola
            output <<",";
        countCourses ++;
    }
    output <<"]";
    return output.str();//ritorno la stringstream sottoforma di stringa
}

///setta il codice del cosro si studi aggiungendo 0 dove necessario
std::string StudyCourse::setCod(int nCod) const {
    std::stringstream output;
    output<<std::setfill('0')<<std::setw(3)<<nCod;
    return output.str();
}

///controlla se un corso presente in altri corsi di studio è posto sempre allo stesso semestre
bool StudyCourse::sameSemester(std::string idCourse, const std::map<int, StudyCourse> & studyCourse,int semester) {
    int sem = 0;
    for(auto iterStudyCourse = studyCourse.begin();iterStudyCourse != studyCourse.end();iterStudyCourse++){
       std::string result = iterStudyCourse->second.isInWhichSemester(idCourse);
       if(result != "") {
           sem = stoi(result.substr(2, 1));
           if(semester != sem)
           throw InvalidDbException("I corsi in parallelo devono appartenere allo stesso semestre.",idCourse);
       }
    }
    return true;
}

///prendo semestre e anno di un corso associato ad un corso di studio
std::string StudyCourse::isInWhichSemester(std::string codCourse) const {
    //controllo tutti i semestri del corso di studio
    for (auto iterSemester = _semesters.begin(); iterSemester != _semesters.end(); iterSemester++){
        auto iterCourse = find(iterSemester->second.begin(),iterSemester->second.end(),codCourse);//punto ai corsi di un semestre e cerco tra questi il corso passato e di cui dovrò assegnare l'esame
        if(iterCourse != iterSemester->second.end()) {//se il corso è presente in uno dei semestri
            return iterSemester->first; //ne prendo il semestre e anno (primo o secondo)
        }
    }
    return "";
}

///overload operatore <<
std::ostream &operator<<(std::ostream &studC, const StudyCourse &s){
    int nCod = s.getId();
    studC << "C"<<s.setCod(nCod)<<";";
    if(s.getIsBachelor())
        studC << "BS;";
    else
        studC << "MS;";
   studC << s.getSemestersString(); //stringa semestri
   if(!s.offCoursesEmpty()) //ci sono corsi spenti?
         studC <<";"<< s.getOffCoursesString();//se ci sono corsi spenti allora prendili
    return studC;
}

