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
    void addStudyPlanPerStudent(std::string,std::vector<std::string>,bool addStudyPlan);
    void readStudyPlanPerStudent(std::string acYearRegistration, std::vector<std::string> coursesStudyPlan);
    ///getter
    std::string getPlanStudyCourseString();
    int getYearRegistration() const;
    std::vector<std::string> getStudyPlan() const;
    ///control
    void clearStudyPlan();
    bool studyPlanIsEmpty();

private:
    //vettore corsi del piano di studio
    std::vector<std::string> _studyPlan;
    int _yearRegistration;

};

std::ostream &operator<<(std::ostream &stud, const Student &s);


#endif //PROGETTO_V3_STUDENT_H
