//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDENT_H
#define PROGETTO_V3_STUDENT_H


#include "UniversityMember.h"
#include "Date.h"

class Student : public UniversityMember {
public:
    explicit Student(int, std::string, std::string, std::string); ///matricola, nome, cognome, email
    explicit  Student(int, std::string, std::string, std::string,std::string,std::string,std::string);
    Student()= default;
    bool addStudyPlanPerStudent(std::string,std::vector<std::string>);
    std::string getPlanStudyCourseString();
    bool clearStudyPlan();
    bool studyPlanIsEmpty();

private:
    std::vector<std::string> _studyPlan; //vettore di codici corso
    int _yearRegistration;
public:
    int getYearRegistration() const;
};

std::ostream &operator<<(std::ostream &stud, const Student &s);


#endif //PROGETTO_V3_STUDENT_H
