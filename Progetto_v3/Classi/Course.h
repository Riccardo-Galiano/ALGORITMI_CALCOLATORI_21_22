//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_COURSE_H
#define PROGETTO_V3_COURSE_H


#include <string>
#include <vector>

typedef struct {
    int _lec;
    int _lab;
    int _ex;
} hours;

class Course {
protected:
    std::string _id;
    std::string _name;
    int _cfu;
    hours _hours;
    std::vector<Course*>_groupedCourse;
};


#endif //PROGETTO_V3_COURSE_H
