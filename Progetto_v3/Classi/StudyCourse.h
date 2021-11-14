//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDYCOURSE_H
#define PROGETTO_V3_STUDYCOURSE_H

#include <map>
#include <list>
#include "Course.h"

class StudyCourse {
    StudyCourse(int id, std::string BSorMs);
protected:
    std::string _name; //nome del corso di studi
    bool _bachelor; //magistrale?
    bool _period; //primo o secondo semestre;
    std::map<std::string,Course> _courses; //per ogni "_name" associa un oggetto corso che comprende i vari corsi specifici di ogni corso di studi
    std::list<Course> _stoppedCourses; //lista corsi spenti
};


#endif //PROGETTO_V3_STUDYCOURSE_H
