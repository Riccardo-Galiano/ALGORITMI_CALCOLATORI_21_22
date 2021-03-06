//
// Created by andre on 06/12/2021.
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
    explicit ExamDay(Date date);

    ///ExamDay management
    void assignExamToProf(std::map<int,Professor>& allUniversityProfs, const Course &course , int hhStart, int num_slots);
    void assignExamToExamDay(int hhStart, const Course &course, int numSlot);
    void setSlot();
    void updateSlot(const Course &course);

    ///control
    bool allSLotsAreEmpty();
    bool searchAvailableClassroomsInThisSlot(std::map<int, Classroom>& allUniversityClassrooms, int numSeatsToSeach, std::vector<int>& idRoomsFounded, int slotHour, int numSLotsRequired,
                                             char labOrClass, int maxNumRooms);
    int isPossibleToAssignThisExamToProfs(Course course, std::map<int, Professor>& allUniversityProfs,
                                          std::map<int, Classroom>& allUniversityClassrooms, int numSlotsRequired,
                                          int relaxPar, std::vector<int>& idRoomsFounded, int endHour,bool firstCourseOfThisLoop,int startControlExamHourSlot); //ritorna lo slot dell'orario iniziale, oppure -1 se non trovato
    bool sameStudyCourseAndYear(Course,int);
    bool checkProfsAvaibility(SpecificYearCourse& specificCourse, std::map<int, Professor>& allUniversityProfs, int relaxPar, int slotHour);
    bool firstSlotCourses(Course courseToFind,std::vector<Course>& coursesProntedSoFar);
    bool pickSomeOfTheseClassrooms(std::vector<Classroom> &potentialRooms, std::vector<int> &idRoomsFounded,
                                   int numSeatsToSeach, int maxNumRooms);
    ///getter
    std::vector<std::string> getSlotsToString();
    std::string getFormattedCoursesPerSlot(std::vector<Course>& coursesOfThisSLot, std::vector<Course>& coursesPrintedSofar);
    int getEndHourOfThisCourseExam(const Course& course);

    ///remove
    void removeThisAppealInfo(int startSlot,int numSlots,std::string& idCourse);
    void eraseTempGroupedCourseClassrooms();
private:

    //key: orario inizio slot. value:corsi associati (paralleli compresi)
    //slots dalle 8 alle 20; uno slot dura due ore --> 8, 10, 12, 14, 16, 18
    std::map<int,std::vector<Course>> _slots;
    Date _date;
    //variabile di appoggio
    std::vector<int> _tempGroupedCourseClassrooms;

};

#endif //PROGETTO_V3_EXAMDAY_H
