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
    bool addSemesterCourses(const int year,const  int  semester,const std::string & SemesterCourses);
    bool addOffCourses(const std::vector<std::string> &corsiSpenti);
    //bool updateThatCourse(std::vector<std::string>& infoCourse);
    const  int getId() const;

private:
    int _id; //codice del corso di studi
    bool _isBachelor; //magistrale?
    std::map<std::string,std::vector<std::string>> _semesters; //key: "yy-semester", value: vettore di courses
    //std::map<std::string,std::vector<SpecificYearCourse>> _perAccademicYear; //key: id (corso agg/nuovo), value: Corso
    //^^^^^^^^ non dovrebbe servire... -luca
    std::list <std::string> _corsiSpenti;
};


#endif //PROGETTO_V3_STUDYCOURSE_H
