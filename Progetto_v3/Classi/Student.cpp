//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "Student.h"

Student::Student(int matr, std::string name, std::string surname, std::string email): UniversityMember(matr, name, surname, email) {}

std::ostream &operator<<(std::ostream &stud, const Student &s){
    int Id = s.getId();
    stud<< "s" << s.setId(Id)<< ";"<< s.getName()<<";"<<s.getSurname()<<";"<<s.getEmail();
    return stud;
}



