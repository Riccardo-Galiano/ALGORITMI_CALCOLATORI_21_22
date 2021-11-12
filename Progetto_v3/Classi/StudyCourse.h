//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDYCOURSE_H
#define PROGETTO_V3_STUDYCOURSE_H

#include <map>
#include <list>
#include "Course.h"
class StudyCourse {
protected:
    std::string _name;
    bool _bachelor; ///magistrale?
    std::map<std::string,Course> _courses;
    std::list<Course> _stoppedCourses;
};


#endif //PROGETTO_V3_STUDYCOURSE_H
