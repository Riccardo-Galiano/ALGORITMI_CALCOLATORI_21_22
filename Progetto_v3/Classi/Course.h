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

    Course(const std::string &idCorso, const std::string &nomeCorso, const int cfu, const int oreLezione,const int oreEsercitazione, const int oreLaboratorio);

    bool addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,std::vector<std::string> exam, std::vector<std::string> idGrouped);
    bool fillSpecificYearCourse(std::vector<std::string>& );
    bool courseOfTheYearIsEmpty();
    SpecificYearCourse &getLastSpecificYearCourse();
    std::vector<SpecificYearCourse>  getSpecificYearsCourse();
    SpecificYearCourse getThisYearCourse(int);
    const std::string &getId() const;
    const std::string &getName() const;
    int getCfu() const;

    const hours &getHours() const;
    int getSpecificYearCourseSize() const;
    bool addStudentToSpecYearCourse(int,Student,std::string,int);
    bool fillAcYearsEmpty();
private:
    std::string _id;///codice identificativo del corso
    std::string _name;///nome del corso
    int _cfu;/// numero di crediti
    hours _hours;///ore impiegate nel corso; suddivise in ore di lezione,laboratori ed esame(struct hours)
    std::map<int, SpecificYearCourse> _courseOfTheYear;//key: accademic yy
};
std::ostream &operator<<(std::ostream &course, Course&s);


#endif //PROGETTO_V3_COURSE_H
