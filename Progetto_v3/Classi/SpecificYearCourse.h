//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_SPECIFICYEARCOURSE_H
#define PROGETTO_V3_SPECIFICYEARCOURSE_H


#include <vector>
#include <map>
#include "Course.h"
#include "Professor.h"
#include "Student.h"

typedef struct {
    Professor prof;
    hours assignedHours;
    bool mainProf;
    
} professor;
typedef struct {
    Student stud;
    int _startEnrolYear;
    int _endEnrolYear;
    int _grade;
    bool _passed;
    
} student;

class SpecificYearCourse : public Course {
private:
    int _startYear;
    int _endYear;
    bool _active;
    int _paralleleCours;
    // per ogni corso in parallelo ho un vettore dei prof e stud
    //key num del corso in parallelo
    //value vettore delle struct prof o stud
    std::map<int, std::vector<professor>> _professors;
    std::map<int, std::vector<student>> _student;
};


#endif //PROGETTO_V3_SPECIFICYEARCOURSE_H
