//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_UNIVERSITY_H
#define PROGETTO_V3_UNIVERSITY_H

#include "Professor.h"
#include "Student.h"
#include "Classroom.h"
#include "StudyCourse.h"
#include "Course.h"
#include "SessionYear.h"
#include "SystemLog.h"

#define MAX_VERSIONING 3
#define MIN_VERSIONING 1

class University {
public:

    University();

    ///getter
    std::map<int, Professor> getProfessors() const;
    std::map<int, Classroom> getClassrooms() const;
    std::map<std::string, Course> getCourses() const;

    ///setter
    void setProfessors(const std::map<int, Professor> &professors);
    void setClassrooms(const std::map<int, Classroom> &classrooms);
    void setCourses(const std::map<std::string, Course> &courses);

    ///session management
    void setProfsNoAvailability(std::string acYear,const std::string& fin);
    void setSessionPeriod( std::string & acYear, std::string & winterSession, std::string &summerSession,std::string &autumnSession);
    std::vector<std::string> allProfsNoAvailabilities();
    void setExamDate(std::string acYear,std::string outputNameFile);
    void setMinDistance(std::string acYear,std::string name);
    void requestChanges(std::string acYear,std::string fin);
    void minDistanceRequestWrite();
    void controlCoerenceSessionDate(std::string winterSession,std::string  summerSession,std::string autumnSession,int acYear);
    void controlOfASingleSessionPeriod(std::string name,std::string sessionPeriod, int acStartYear);
    void controlUnicity(std::vector<std::string>& controlUnicityCourses,int line_counter);
    void removeThisAppealAndGroupedInfo(int acYear, std::string idCourse, int numSession, int numAppeal, Date& date, int& startSlot, std::map<std::string,std::vector<int>>& classrooms);
    void reassignThisAppealInfo(int acYear, std::string idCourse, int numSession, int numAppeal,Date date,int startSlot, std::map<std::string,std::vector<int>> classrooms);
    void generateOutputFileName();
    void popOffCoursesFromGroupedString(std::vector<std::string> &coursesToConsider, int year);

    ///DB management
    void versioning(std::string newVersion);
    bool checkVersioningRequest(int newVer);
    void revertChanges(int newVersion);
    void revertChanges2();
    void revertChanges3();
    void renameOldDataBase(int version);
    void controlDatabase(int startAcYear);
    void dataBaseIsEmpty(int startAcYear);
    bool controlGroupedCoursesDifferentCds_C(std::vector<std::string> idGrouped,
                                             std::string idCourseToAddToIdGrouped, int year);
    bool controlGroupedCoursesDifferentStudyCourse_Sc();
    void insertCourses(const std::string &fin);
    void insertStudentsGrades(std::string fin);
    void addStudyPlan(std::string fin);
    void registerStudentsToSpecificYearCourses(std::vector<std::string>& courses,Student& stud, std::string acYearRegistration, int line_counter);

    void assignAppealPerCourse(std::string acYear, std::string lastReadCourse, std::vector<int> classroom,
                               Date appeal, int startSlotHour, int numSession);
    void assignAppealsToProf(std::string idCorso,std::string appeal, int startHour, int numSlots,std::vector<int> allProfsPerYearCourse);
    void assignAppealsToClassroom(std::string appeal,int startSlotHour,std::vector<int>classrooms,int numSlot);
    void ifThereAreAlreadyCoursesFillYYSemesterVar(StudyCourse& sCourse);
    int whatIsMyStudyCourse(Course& course, int acStartYear);
    void fillGroupedCourse(std::vector<std::string> &idGroupedLetti, std::string &idCourse, std::string &acYear);

    ///Input (from DB)
    void readCourseNotActive();
    void readStudyPlan();
    void readStudents();
    void readProfessor();
    void readClassroom();
    void readStudyCourse();
    void readCourse();
    void readSessionAcYear();
    void readProfsNoAvailability();
    void readPassedAppeals();
    void readVersion();
    void readAllMinDistanceRequest();
    void readOutputFileName();
    void readExamAppealsPerAcSession(std::string acYear);

    ///Output(to DB)
    void dbStudsWrite();
    void dbCourseNotActive();
    void dbProfsWrite();
    void dbClassRoomWrite();
    void dbStudyCourseWrite();
    void dbCourseWrite();
    void dbStudyPlanWrite();
    void dbDateSessionsWrite();
    void dbNoAvailabilityWrite();
    void dbAppealsWrite();
    void writeVersion(int version);

    ///get new ID
    const int getNewStudentId() const ;
    const int getNewProfessorId() const;
    const int getNewClassroomId() const;
    const int getNewStudyCourseId() const;
    const std::string getNewCourseId()const;
    std::string getPotentialCourseString() const;

    ///add
    void addStuds(const std::string &fileIn);
    void addProfessors(const std::string &fileIn);
    void addClassrooms(const std::string &fileIn);
    void addStudyCourses(const std::string &fin);
    void addCourses(const std::string &fileIn);

    ///update
    void updateStuds(const std::string &fin);
    void updateProfessors(const std::string &fin);
    void updateClassroom(const std::string &fin);
    void updateStudyPlan(const std::string& fin);

private:
    SystemLog _universityLog;
    std::map<int, Professor> _professors;
    std::map<int, Student> _students;
    std::map<int, Classroom> _classrooms;
    std::map<int, StudyCourse> _studyCourse;
    std::map<std::string, Course> _courses;
    std::map<std::string, std::vector<std::string>> _coursesGrouped; //key: "acYear-IdCourse" value: "corsi grouped"
    std::map<int,SessionYear> _acYearSessions;
    std::vector<std::string> _tempInfoNotActiveCoursesToWriteInTheDB;
    std::vector<std::string> _potentialCourse;

private:
    int _version;
};


#endif //PROGETTO_V3_UNIVERSITY_H
