//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "Professor.h"

Professor::Professor(int matr, std::string name, std::string surname, std::string email): UniversityMember(matr, name, surname, email) {
}


Professor::Professor(int matr) : UniversityMember() {
    _id = matr;
}

///overload operatore <<
std::ostream &operator<<(std::ostream &prof, const Professor &s){

    int Id = s.getId();
    prof<< "d" << s.setId(Id)<< ";"<< s.getName()<<";"<<s.getSurname()<<";"<<s.getEmail();
    return prof;
}