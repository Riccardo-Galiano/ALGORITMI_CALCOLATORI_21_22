//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDENT_H
#define PROGETTO_V3_STUDENT_H


#include "UniversityMember.h"

class Student : public UniversityMember {
public:
    explicit Student(int, std::string, std::string, std::string); ///matricola, nome, cognome, email
    Student()= default;
};

std::ostream &operator<<(std::ostream &stud, const Student &s);


#endif //PROGETTO_V3_STUDENT_H
