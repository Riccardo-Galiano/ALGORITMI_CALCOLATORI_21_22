//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_PROFESSOR_H
#define PROGETTO_V3_PROFESSOR_H


#include "UniversityMember.h"

class Professor : public UniversityMember {
public:
    Professor(int matr);
    Professor(int, std::string, std::string, std::string); ///matricola,nome, cognome, email
};


#endif //PROGETTO_V3_PROFESSOR_H
