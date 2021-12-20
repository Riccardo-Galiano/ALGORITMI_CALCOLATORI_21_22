//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include <algorithm>
#include "StudyCourse.h"
#include "Parse.hpp"
#include "DbException.h"

StudyCourse::StudyCourse(const int id, const bool &isBachelor) : _id{id}, _isBachelor{isBachelor} {}

///aggiunge un semsetre con i relativi corsi al corso di studio
bool StudyCourse::addSemesterCourses(const int year, const int semester, const std::string &SemesterCourses,const std::map<int, StudyCourse>& studyCourse) {
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
        _offCourses.push_back(corsiSpenti[i]);//per ogni codice del corso spento associo un oggetto corso
    }
    _offCourses.sort();//se li vogliamo in ordine crescente;
    return true;
}

///controlla se ci sono corsi spenti: corsiSpenti è vuoto?
bool StudyCourse::offCoursesEmpty() const {
    return _offCourses.empty();
}

bool StudyCourse::updateSemestersAndOffCourses(const std::string &idCourse) {

for(auto iterSemesters = _semesters.begin(); iterSemesters != _semesters.end(); iterSemesters++){

     std::vector<std::string> &corsi = iterSemesters->second;
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

std::string StudyCourse::isInWhichSemester(std::string codCourse) {
    for (auto iterSemester = _semesters.begin(); iterSemester != _semesters.end(); iterSemester++){//controllo tutti i semestri del corso di studio
        auto iterCourse = find(iterSemester->second.begin(),iterSemester->second.end(),codCourse);//punto ai corsi di un semestre e cerco tra questi il corso passato e di cui dovrò assegnare l'esame
        if(iterCourse != iterSemester->second.end()) {//se il corso è presente in uno dei semestri
            return iterSemester->first; //ne prendo il semestre(primo o secondo)
        }
    }
    return "";
}

bool StudyCourse::controlOfTheExistenceOfCourses(const std::map<std::string, Course>& courses, int line) {
    for(auto iterSemester = _semesters.begin(); iterSemester != _semesters.end(); iterSemester++){
        for(int i = 0; i<iterSemester->second.size();i++) {
            auto iterCourses = courses.find(iterSemester->second[i]);
            if(iterCourses == courses.end())
                throw DbException("Nel database dei corsi non e' presente il seguente corso:",iterSemester->second[i],". Rivedere la seguente riga nel file per l'inserimento dei corsi di studio alla riga:",line);
        }
    }
    return true;
}

bool StudyCourse::controlStudyCourseOfGrouppedCourse(const std::map<std::string, Course>& courses, int line) {
    std::vector<std::string> allCourses = getAllCoursesOfStudyCourse();//prendo tutti i corsi del corso di studio che sto inserendo
    for(int i = 0; i<allCourses.size();i++){//per ogni corso controllo se nello stesso corso di studio ci siano suoi corsi raggruppati
        courses.at(allCourses[i]).controlItsGrouppedCourse(allCourses,line);
    }
    return false;
    /*for(int i = 0; i < grouppedCourses.size(); i++){
        auto iterGrouppedCourse = courses.find(grouppedCourses[i]);//cerco nel database(o futuro database) dei corsi se esiste
        SpecificYearCourse sp = iterGrouppedCourse->second.getThisYearCourse(year);//prendo il corso all'anno considerato
        std::vector<int> studyCoursesOfGrouppedCourses = sp.getStudyCourseAssigned();//prendo i corsi di studio a cui è assegnato in quell'anno
        for (int j = 0; j < studyCoursesOfGrouppedCourses.size(); j++) {
            int count = std::count(allSC.begin(),allSC.end(),studyCoursesOfGrouppedCourses[i]);
            if(count > 1) {
                grouppedCourses.insert(grouppedCourses.begin(),getId());
                throw DbException("Controllare i seguenti corsi in modo che non abbiano corsi di studio in comune:",allName," con rispettivamente i seguenti codici: ",grouppedCourses);
            }
        }
    }
    return true;*/
}

const std::map<std::string, std::vector<std::string>> &StudyCourse::getSemesters() const {
    return _semesters;
}

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

