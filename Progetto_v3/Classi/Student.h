//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDENT_H
#define PROGETTO_V3_STUDENT_H


#include "UniversityMember.h"
#include "Date.h"

class Student : public UniversityMember {
public:
    ///constructor
    Student()= default;
    explicit Student(int, std::string, std::string, std::string); ///matricola, nome, cognome, email
    explicit  Student(int, std::string, std::string, std::string,std::string,std::string,std::string);

    ///Student management
    bool addStudyPlanPerStudent(std::string,std::vector<std::string>,bool addStudyPlan);

    ///getter
    std::string getPlanStudyCourseString();

    ///control
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
