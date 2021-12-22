//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDYCOURSE_H
#define PROGETTO_V3_STUDYCOURSE_H

#include <map>
#include <list>
#include "Course.h"
#include "SpecificYearCourse.h"

class StudyCourse {
public:

    StudyCourse(const int id, const bool &isBachelor);

    bool addSemesterCourses(const int year,const  int  semester,const std::string & SemesterCourses,const std::map<int, StudyCourse>&);
    bool addOffCourses(const std::vector<std::string> &corsiSpenti);

    const  int getId() const;
    bool getIsBachelor() const;
    std::string getSemestersString() const;
    std::string getOffCoursesString() const;

    bool offCoursesEmpty() const;
    bool updateSemestersAndOffCourses( const std::string&);
    std::string setCod(int nCod) const ;
    //param: corso da cercare
    //return "" stringa vuoota se non esiste in questo corso di studio, oppure "anno-semestre" corrispondente
    std::string isInWhichSemester(std::string);
    bool controlOfTheExistenceOfCourses(const std::map<std::string,Course>&,int);
    bool controlStudyCourseOfGroupedCourse(const std::map<std::string, Course>&,int, int);

    std::vector<std::string> getAllCoursesOfStudyCourse();
    const std::map<std::string, std::vector<std::string>> &getSemesters() const;
    bool controlUniqueness();
    bool controlOfGroupedCourses(const std::map<std::string,Course> &,int);
    std::vector<std::string> getAllGroupedCourses(std::vector<std::string>,const std::map<std::string, Course>&, int);

private:
    int _id; //codice del corso di studi
    bool _isBachelor; //magistrale?
    std::map<std::string,std::vector<std::string>> _semesters; //key: "yy-semester", value: vettore di stringhe
    std::list <std::string> _offCourses;

};
std::ostream &operator<<(std::ostream &studC, const StudyCourse &s);

#endif //PROGETTO_V3_STUDYCOURSE_H
