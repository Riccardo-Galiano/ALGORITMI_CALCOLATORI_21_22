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
    bool addCourse(int year, int semester, Course course);
protected:
    int _id; //codice del corso di studi
    bool _isBachelor; //magistrale?
    std::map<std::string,std::vector<Course>> _semesters; //key: "yy-semester", value: vettore di courses
};


#endif //PROGETTO_V3_STUDYCOURSE_H
