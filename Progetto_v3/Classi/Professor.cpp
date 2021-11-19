//
// Created by lucam on 05/11/2021.
//

#include "Professor.h"

Professor::Professor(int matr, std::string name, std::string surname, std::string email)
        : UniversityMember(matr, name, surname, email) {
}

Professor::Professor(int matr) : UniversityMember() {
    _id = matr;
}
