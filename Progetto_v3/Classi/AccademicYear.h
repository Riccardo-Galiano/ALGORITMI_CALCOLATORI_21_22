//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_ACCADEMICYEAR_H
#define PROGETTO_V3_ACCADEMICYEAR_H


#include <map>
#include <vector>
#include "StudyCourse.h"

class AccademicYear {
    int _startYear;
    int _endYear;
    ///nomeCorso-CorsoDiStudio
    std::map<std::string,StudyCourse> _studyCourses;
};


#endif //PROGETTO_V3_ACCADEMICYEAR_H
