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
    bool assignExamToProf(std::map<int,Professor>&, Course course, int hhStart, int num_slots);
    bool assignExamToExamDay(int, Course,int);
    int isPossibleToAssignThisExamToProf(Course course, std::map<int, Professor>& allUniversityProfs, int numSlotsRequired); //ritorna lo slot dell'orario iniziale, oppure -1 se non trovato
    bool sameStudyCourseAndYear(Course,int);
    bool setSlot();
    std::vector<std::string> getSlotsToString();
    std::string getSingleSlotCourses(std::vector<Course>,std::vector<Course>);
    bool firstSlotCourses(Course,std::vector<Course>);

private:
    //key: orario inizio slot. value:corsi associati (paralleli compresi)
    //slots dalle 8 alle 20; uno slot dura due ore --> 8, 10, 12, 14, 16, 18
    std::map<int,std::vector<Course>> _slots;
    Date _date;

};

#endif //PROGETTO_V3_EXAMDAY_H
