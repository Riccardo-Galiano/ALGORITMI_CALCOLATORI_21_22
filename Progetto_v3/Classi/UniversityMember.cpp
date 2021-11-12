//
// Created by lucam on 05/11/2021.
//

#include "UniversityMember.h"

UniversityMember::UniversityMember(int matr, std::string n, std::string s, std::string e) {
    _name = n;
    _surname = s;
    _email = e;
    _id = matr;
}

const std::string &UniversityMember::getName() const {
    return _name;
}

const std::string &UniversityMember::getSurname() const {
    return _surname;
}

int UniversityMember::getId() const {
    return _id;
}

const std::string &UniversityMember::getEmail() const {
    return _email;
}
