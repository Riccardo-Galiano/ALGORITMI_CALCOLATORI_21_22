//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include <algorithm>
#include "StudyCourse.h"
#include "Parse.hpp"

StudyCourse::StudyCourse(const int id, const bool &isBachelor) : _id{id}, _isBachelor{isBachelor} {}

///aggiunge un semsetre con i relativi corsi al corso di studio
bool StudyCourse::addSemesterCourses(const int year, const int semester, const std::string &SemesterCourses) {
    ///key
    int i = 0;
    std::stringstream ss;
    ss << year << "-" << semester;
    std::string key = ss.str(); //creo la chiave con anno e semestre

    ///create values
    std::vector<std::string> courses;
    courses = Parse::splittedLine(SemesterCourses,',');//adesso ho i corsi del semestre passati alla funzione che non erano divisi

    for (auto iter = courses.begin(); iter != courses.end(); iter++) {//analizzo tutti i componenti del vettore corsi
        if (!_semesters.count(key)) {//se la chiave non esiste
            std::vector<std::string> vect;

            vect.push_back(courses[0]);//salvo il primo corso del semestre
            _semesters.insert(std::pair<std::string, std::vector<std::string>>(key, vect));//inserisco nella mappa il primo corso del semestre
        } else {//se la chiave esiste
            i++;
            _semesters.at(key).push_back(courses[i]);//aggiungo nel semestre già esistenete i corsi successivi al primo
        }
    }
    return true;

}

///aggiunge corso spento
bool StudyCourse::addOffCourses(const std::vector<std::string> &corsiSpenti) {
    for (int i=0; i<corsiSpenti.size(); i++) {
        _corsiSpenti.push_back(corsiSpenti[i]);//per ogni codice del corso spento associo un oggetto corso
    }
    _corsiSpenti.sort();//se li vogliamo in ordine crescente;
    return true;
}

///prende l'_id
const int StudyCourse::getId() const {
    return _id;
}

///setta il codice del cosro si studi aggiungendo 0 dove necessario
std::string StudyCourse::setCod(int nCod) const {
    std::stringstream output;
    output<<std::setfill('0')<<std::setw(3)<<nCod;
    return output.str();
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

///controlla se ci sono corsi spenti: corsiSpenti è vuoto?
bool StudyCourse::offCoursesEmpty() const {
    return _corsiSpenti.empty();
}

///prende i corsi spenti
std::string StudyCourse::getOffCoursesString() const {
    std::stringstream output;
    int countCourses = 0;


    output <<"[";
    for( auto iterOffCourses = _corsiSpenti.begin(); iterOffCourses != _corsiSpenti.end() ; iterOffCourses++){//per ogni corso spento
        output<< *iterOffCourses;//dereferenzio l'iteratore: prendo il value puntato dall'iteratore
        if( countCourses< _corsiSpenti.size() - 1)//se l'ultimo corso non va messa la virgola
            output <<",";
        countCourses ++;
    }
    output <<"]";
    return output.str();//ritorno la stringstream sottoforma di stringa
}

bool StudyCourse::updateSemestersAndOffCourses(const std::string &idCourse) {

for(auto iterSemesters = _semesters.begin(); iterSemesters != _semesters.end(); iterSemesters++){

     std::vector<std::string> &corsi = iterSemesters->second;
     auto found = std::find(corsi.begin(),corsi.end(),idCourse);

        if(found != corsi.end()) {
             corsi.erase(found);
            _corsiSpenti.push_back(idCourse);
        }
    }

    return true;
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

