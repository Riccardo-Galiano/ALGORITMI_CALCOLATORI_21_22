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

    StudyCourse(const int, const bool &);
    bool addSemesterCourses(const int,const  int ,const std::string &,const std::map<int, StudyCourse>&, std::map<std::string, Course>&,int);
    bool addOffCourses(const std::vector<std::string> &);
    const  int getId() const;
    bool getIsBachelor() const;
    std::string getSemestersString() const;
    std::string getOffCoursesString() const;
    std::vector<std::string> getAllCoursesOfStudyCourse();
    bool offCoursesEmpty() const;
    bool updateSemestersAndOffCourses( const std::string&,std::vector<std::string>&);
    std::string setCod(int) const ;
    //param: corso da cercare
    //return "" stringa vuoota se non esiste in questo corso di studio, oppure "anno-semestre" corrispondente
    std::string isInWhichSemester(std::string) const;
    bool sameSemester(std::string,const std::map<int, StudyCourse>&,int);
    bool assignStudyCourse(std::string);

private:
    int _id; //codice del corso di studi
    bool _isBachelor; //magistrale?
    std::map<std::string,std::vector<std::string>> _semesters; //key: "yy-semester", value: vettore di stringhe
    std::list <std::string> _offCourses;

};
std::ostream &operator<<(std::ostream &studC, const StudyCourse &s);

#endif //PROGETTO_V3_STUDYCOURSE_H
