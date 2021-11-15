//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDYCOURSE_H
#define PROGETTO_V3_STUDYCOURSE_H

#include <map>
#include <list>
#include "Course.h"

class StudyCourse {
public:
    StudyCourse(int id, bool isBachelor);
    bool addSemesterCourses(int year, int semester, std::string SemesterCourses);
    bool addOffCourses(std::vector<std::string> corsiSpenti );

protected:
    int _id; //codice del corso di studi
    bool _isBachelor; //magistrale?
    std::map<std::string,std::vector<std::string>> _semesters; //key: "yy-semester", value: vettore di courses
    std::map<std::string,Course> _corsiSpenti;
};


#endif //PROGETTO_V3_STUDYCOURSE_H
