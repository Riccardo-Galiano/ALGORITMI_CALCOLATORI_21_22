//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "StudyCourse.h"

StudyCourse::StudyCourse(int id, bool isBachelor) {

}

bool StudyCourse::addCourse(int year, int semester, Course course) {
    std::stringstream ss;
    ss << year << "-" << semester;
    std::string key = ss.str();
    if (!_semesters.count(key)) {
        std::vector<Course> vect;
        vect.push_back(course);
        _semesters.insert(std::pair<std::string,std::vector<Course>>(key, vect));
    }else{
        _semesters.at(key).push_back(course);
    }


}
