//
// Created by Andrea on 06/12/2021.
//

#ifndef PROGETTO_V3_EXAMDAY_H
#define PROGETTO_V3_EXAMDAY_H

#include "SpecificYearCourse.h"
#include "Course.h"
#include "StudyCourse.h"
#include <map>

class ExamDay {
public:
    ExamDay(Date);
    bool assignExamToProf(std::map<int,Professor>&, Course , int , int );
    bool assignExamToExamDay(int, Course,int);
    int isPossibleToAssignThisExamToProf(Course, std::map<int, Professor>&, int, int); //ritorna lo slot dell'orario iniziale, oppure -1 se non trovato
    bool sameStudyCourseAndYear(Course,int);
    bool setSlot();
    std::vector<std::string> getSlotsToString();

private:
    std::string getFormattedCoursesPerSlot(std::vector<Course>&, std::vector<Course>&);
    bool firstSlotCourses(Course,std::vector<Course>&);

    //key: orario inizio slot. value:corsi associati (paralleli compresi)
    //slots dalle 8 alle 20; uno slot dura due ore --> 8, 10, 12, 14, 16, 18
    std::map<int,std::vector<Course>> _slots;
    Date _date;

};

#endif //PROGETTO_V3_EXAMDAY_H
