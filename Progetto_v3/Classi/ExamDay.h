//
// Created by Andrea on 06/12/2021.
//

#ifndef PROGETTO_V3_EXAMDAY_H
#define PROGETTO_V3_EXAMDAY_H

#include "SpecificYearCourse.h"
#include "Course.h"
#include <map>

class ExamDay {
public:

    bool assignExamToProf(std::vector<Professor>&, Course course, int hhStart, int num_slots); //corso da assegnare in un dato slot, per una certa durata
    int isPossibleToAssignThisExam(Course course, std::map<int, Professor>& allUniversityProfs, int numSlotsRequired); //ritorna lo slot dell'orario iniziale, oppure -1 se non trovato

private:
    //key: orario inizio slot. value:corsi associati (paralleli compresi)
    //slots dalle 8 alle 20; uno slot dura due ore --> 8, 10, 12, 14, 16, 18
    std::map<int,std::vector<Course>> _slots;
    Date _date;
};


#endif //PROGETTO_V3_EXAMDAY_H
