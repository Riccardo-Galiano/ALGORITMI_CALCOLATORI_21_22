//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "Student.h"
#include "Parse.hpp"

Student::Student(int matr, std::string name, std::string surname, std::string email): UniversityMember(matr, name, surname, email) {

}

Student::Student(int matr, std::string name, std::string surname, std::string email,std::string birth, std::string registration, std::string address ): UniversityMember(matr, name, surname, email, birth, registration,address) {

}



bool Student::addStudyPlanPerStudent(std::string acYearRegistration, std::vector<std::string> coursesStudyPlan) {
    ///questa funzione è usata per la add e per la update quindi io aggiungo l'anno di

    _yearRegistration = Parse::getAcStartYear( acYearRegistration);
    if(_studyPlan.empty() == false){
        std::string settedId = Parse::setId('s',6,getId());
        throw std::invalid_argument("Il piano di Studio per lo studente con matricola "+ settedId + " e' stato gia' definito \n");
    }
    _studyPlan.insert(_studyPlan.begin(),coursesStudyPlan.begin(),coursesStudyPlan.end());
    return true;
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

bool Student::clearStudyPlan() {
    _studyPlan.erase( _studyPlan.begin(), _studyPlan.end());
    return true;
}

bool Student::studyPlanIsEmpty() {
    return  _studyPlan.empty();
}

///overload operatore <<
std::ostream &operator<<(std::ostream &stud, const Student &s){
    std::string settedId = Parse::setId('s',6,s.getId());
    stud<< settedId<< ";"<< s.getName()<<";"<<s.getSurname()<<";"<<s.getEmail();
    return stud;
}



