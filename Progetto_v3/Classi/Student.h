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
    explicit Student(int matr, const std::string &name, const std::string &surname, const std::string &email); ///matricola, nome, cognome, email
    explicit  Student(int matr, const std::string &name, const std::string &surname, const std::string &email,
                      const std::string &birth, const std::string &registration, const std::string &address);

    ///Student management
    void addStudyPlanPerStudent(std::string acYearRegistration, const std::vector<std::string> &coursesStudyPlan, bool addStudyPlan);
    void readStudyPlanPerStudent(std::string acYearRegistration, const std::vector<std::string> &coursesStudyPlan);
    ///getter

    std::string getPlanStudyCourseString() const;
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
