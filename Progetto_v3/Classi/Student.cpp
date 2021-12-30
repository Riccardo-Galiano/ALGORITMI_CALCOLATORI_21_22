//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "Student.h"
#include "Parse.hpp"

Student::Student(int matr, std::string name, std::string surname, std::string email): UniversityMember(matr, name, surname, email) {

    _othersInfoStud._birth = Date();
    _othersInfoStud._registration = Date();
    _othersInfoStud._address = "indefinito";

}

std::string Student::getOtherInfoString() {
    std::stringstream ss;
    ss << _othersInfoStud._birth <<";"<< _othersInfoStud._registration << ";" <<_othersInfoStud._address;
    return ss.str();
}

bool Student::addStudyPlanPerStudent(std::string acYearRegistration, std::vector<std::string> coursesStudyPlan) {
    _yearRegistration = Parse::getAcStartYear( acYearRegistration);
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
    int Id = s.getId();
    stud<< "s" << s.setId(Id)<< ";"<< s.getName()<<";"<<s.getSurname()<<";"<<s.getEmail();
    return stud;
}



