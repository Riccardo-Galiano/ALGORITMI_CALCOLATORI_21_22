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

    Course(const std::string &idCorso, const std::string &nomeCorso, const int cfu, const int oreLezione,const int oreEsercitazione, const int oreLaboratori);

    bool addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,std::vector<std::string> exam, std::vector<std::string> idGrouped, std::string yy_semester, std::vector<int> studyCourse);
    bool fillSpecificYearCourse(std::vector<std::string>& );
    bool courseOfTheYearIsEmpty();
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
    bool addStudentToSpecYearCourse(int,Student,std::string,int);
    bool fillAcYearsEmpty();
    bool controlOfGroupedCourses(const std::map<std::string,Course> &,int);
    bool controlTheExistenceOfGroupedCourse(std::vector<std::string>grouppedCourses,const std::map<std::string, Course> &courses,int year);
    bool controlTheExistenceAndHoursOfProfessors(const std::map<int,Professor> &,int, hours);
    hours controlProfsOfSingleCourse(std::vector<professor>,const std::map<int,Professor> &);
    bool controlItsGroupedCourse(std::vector<std::string>,std::vector<std::string>, int,int) const;
    bool courseOfTheYearFounded(int);
    bool controlExistenceSpecificYear(std::string, int);

private:
    std::string _id;///codice identificativo del corso
    std::string _name;///nome del corso
    int _cfu;/// numero di crediti
    hours _hours;///ore impiegate nel corso; suddivise in ore di lezione,laboratori ed esame(struct hours)
    std::map<int, SpecificYearCourse> _courseOfTheYear;//key: accademic yy
};
std::ostream &operator<<(std::ostream &course, Course&s);


#endif //PROGETTO_V3_COURSE_H
