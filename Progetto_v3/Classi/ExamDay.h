//
// Created by Andrea on 06/12/2021.
//

#ifndef PROGETTO_V3_EXAMDAY_H
#define PROGETTO_V3_EXAMDAY_H

#include "SpecificYearCourse.h"
#include "Course.h"
#include "StudyCourse.h"
#include "Classroom.h"
#include <map>

class ExamDay {
public:
    ///constructor
    explicit ExamDay(Date);

    ///ExamDay management
    bool assignExamToProf(std::map<int,Professor>&, Course , int , int );
    bool assignExamToExamDay(int, Course&, int);
    int isPossibleToAssignThisExamToProfs(Course course, std::map<int, Professor>& allUniversityProfs,
                                          std::map<int, Classroom>& allUniversityClassrooms, int numSlotsRequired,
                                          int relaxPar, std::vector<int>& idRoomsFounded, int endHour,bool firstCourseOfThisLoop,int startControlExamHourSlot); //ritorna lo slot dell'orario iniziale, oppure -1 se non trovato
    bool sameStudyCourseAndYear(Course,int);
    bool setSlot();
    std::vector<std::string> getSlotsToString();
    bool allSLotsAreEmpty();
    bool searchAvailableClassroomsInThisSlot(std::map<int, Classroom>& allUniversityClassrooms, int numSeatsToSeach, std::vector<int>& idRoomsFounded, int slotHour,int numSlotsRequired,std::string labOrClass);
    bool checkProfsAvaibility(SpecificYearCourse& specificCourse, std::map<int, Professor>& allUniversityProfs, int relaxPar, int slotHour);
    void eraseTempGroupedCourseClassrooms();
    std::string classroomString(std::vector<int>);
    std::string getFormattedCoursesPerSlot(std::vector<Course>&, std::vector<Course>&);
    bool firstSlotCourses(Course,std::vector<Course>&);
    int getEndHourOfThisCourseExam(const Course&);

    ///remove
    void removeThisAppealInfo(int startSlot,std::string& idCourse);
private:

    //key: orario inizio slot. value:corsi associati (paralleli compresi)
    //slots dalle 8 alle 20; uno slot dura due ore --> 8, 10, 12, 14, 16, 18
    std::map<int,std::vector<Course>> _slots;
    Date _date;
    std::vector<int> _tempGroupedCourseClassrooms;

};

#endif //PROGETTO_V3_EXAMDAY_H
