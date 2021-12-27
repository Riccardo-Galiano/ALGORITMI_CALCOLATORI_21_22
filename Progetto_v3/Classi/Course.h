//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_COURSE_H
#define PROGETTO_V3_COURSE_H


#include <string>
#include <vector>
#include <map>
#include "SpecificYearCourse.h"



typedef struct {
    int _lec;///ore lezione
    int _lab;///ore laboratori
    int _ex;///ore esercitazione
} hours;

class Course {
public:

    Course(const std::string&, const std::string&, const int , const int ,const int , const int );

    bool addSpecificYearCourses(std::string, bool, int, std::vector<std::string>, std::vector<std::string>, std::vector<std::string>, std::string, std::vector<int>, int );
    bool addStudentToSpecYearCourse(int,Student,std::string,int);
    bool fillSpecificYearCourse(std::vector<std::string>&, int  );
    bool courseOfTheYearIsEmpty();
    bool fillAcYearsEmpty();
    bool controlTheExistenceAndHoursOfProfessors(const std::map<int,Professor> &,int);
    hours controlProfsOfSingleCourse(std::vector<professor>,const std::map<int,Professor> &);
    bool controlExistenceSpecificYear(std::string, int);
    SpecificYearCourse &getLastSpecificYearCourse();
    std::vector<SpecificYearCourse>  getSpecificYearsCourse();
    const SpecificYearCourse & getThisYearCourse(int) const;
    SpecificYearCourse & getThisYearCourseReference(int);
    const std::string &getId() const;
    const std::string &getName() const;
    int getCfu() const;
    const Exam getExamSpecificYear(int) const;
    const hours &getHours() const;
    int getSpecificYearCourseSize() const;
    std::map<int,std::vector<std::string>> getGroupedCourseFromAllYear();
    void notActive();
    int getSemesterAtYear(int,std::string);
    bool sameSemesterGrouped(std::map<std::string,Course>);
    bool assignYY_Sem(std::string);

private:
    std::string _id;///codice identificativo del corso
    std::string _name;///nome del corso
    int _cfu;/// numero di crediti
    hours _hours;///ore impiegate nel corso; suddivise in ore di lezione,laboratori ed esame(struct hours)
    std::map<int, SpecificYearCourse> _courseOfTheYear;//key: accademic yy
};
std::ostream &operator<<(std::ostream &course, Course&s);


#endif //PROGETTO_V3_COURSE_H
