//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDENT_H
#define PROGETTO_V3_STUDENT_H


#include "UniversityMember.h"
#include "Date.h"

typedef struct {
    Date _birth;
    Date _registration;
    std::string _address;
} updateStudent;

class Student : public UniversityMember {
public:
    explicit Student(int, std::string, std::string, std::string); ///matricola, nome, cognome, email
    Student()= default;
    std::string getOtherInfoString();
    bool addStudyPlanPerStudent(std::string,std::vector<std::string>);
    std::string getPlanStudyCourseString();
    bool clearStudyPlan();
    bool studyPlanIsEmpty();

private:
    updateStudent _othersInfoStud;
    std::vector<std::string> _studyPlan;
    int _yearRegistration;
public:
    int getYearRegistration() const;
};

std::ostream &operator<<(std::ostream &stud, const Student &s);


#endif //PROGETTO_V3_STUDENT_H
