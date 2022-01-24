//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "Student.h"
#include "Parse.hpp"
//costruttore
Student::Student(int matr, std::string name, std::string surname, std::string email): UniversityMember(matr, name, surname, email) {

}

//costruttore
Student::Student(int matr, std::string name, std::string surname, std::string email,std::string birth, std::string registration, std::string address ): UniversityMember(matr, name, surname, email, birth, registration,address) {

}

///aggiunge il piano di studio e l'anno di registrazione di uno studente(add) aggiorna il priano di studio di uno studente (update)
void Student::addStudyPlanPerStudent(std::string acYearRegistration, std::vector<std::string> coursesStudyPlan,bool addStudyPlan) {
    ///questa funzione è usata per la add e per la update
    //addStudyPlan == false ---> funzione di aggiornamento updateStudyPlan
    //addStudyPlan == true ---> funzione di aggiunta addSTudyPlan
    if(addStudyPlan == false && _yearRegistration == 0 && _studyPlan.empty()){
        // non ho mai assegnato un corso di studio a questo studente(cosa che faccio con la add) quindi non posso aggiornarlo
        throw std::invalid_argument(" non e' mai stato iscritto \n");
    }
    if(addStudyPlan == false && _yearRegistration != Parse::getAcStartYear( acYearRegistration))
        //se si tratta della update mi assicuro che l'anno usato sia quello già presente
        throw std::invalid_argument(" e' iscritto ad un altro anno accademico\n");

    if(_studyPlan.empty() == false && addStudyPlan == true){
        //se si tratta della add puo' succedere che venga riproposto un nuovo studente ma non posso modificare con la add
        std::string settedId = Parse::setId('s',6,getId());
        throw std::invalid_argument("Il piano di Studio per lo studente con matricola "+ settedId + " e' stato gia' definito. ");
    }else if(_studyPlan.empty() == false && addStudyPlan == false) {
        //se si tratta della update mi devo assicurare che il corso di studio da aggiornare sia pulito prima di inserire il nuovo
        clearStudyPlan();
    }
    _yearRegistration = Parse::getAcStartYear( acYearRegistration);
    _studyPlan.insert(_studyPlan.begin(),coursesStudyPlan.begin(),coursesStudyPlan.end());
}
//leggendo il db_piano di studi aggiunge alla struttura l'informazione
void Student::readStudyPlanPerStudent(std::string acYearRegistration, std::vector<std::string> coursesStudyPlan) {
    _yearRegistration = Parse::getAcStartYear( acYearRegistration);
    _studyPlan.insert(_studyPlan.begin(),coursesStudyPlan.begin(),coursesStudyPlan.end());
}

int Student::getYearRegistration() const {
    return _yearRegistration;
}

std::string Student::getPlanStudyCourseString() {
    std::stringstream allCourses;
    allCourses<<"{";
    for(int i = 0; i<_studyPlan.size(); i++){
        allCourses << _studyPlan[i];
        if(i<_studyPlan.size()-1)
            allCourses<<";";
    }
    allCourses<<"}";
    return allCourses.str();
}

void Student::clearStudyPlan() {
    _studyPlan.erase( _studyPlan.begin(), _studyPlan.end());
}

bool Student::studyPlanIsEmpty() {
    return  _studyPlan.empty();
}

std::vector<std::string> Student::getStudyPlan() const {
    return _studyPlan;
}

///overload operatore <<
std::ostream &operator<<(std::ostream &stud, const Student &s){
    std::string settedId = Parse::setId('s',6,s.getId());
    stud<< settedId<< ";"<< s.getName()<<";"<<s.getSurname()<<";"<<s.getEmail();
    return stud;
}



