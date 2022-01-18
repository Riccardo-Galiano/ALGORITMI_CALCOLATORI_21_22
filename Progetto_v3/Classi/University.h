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

class University {
public:

    University();

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
    void removeThisAppealInfo(int acYear, std::string idCourse, int numSession, int numAppeal,Date& date,int& startSlot, std::map<std::string,std::vector<int>>& classrooms);

    ///DB management
    void versioning(std::string newVersion);
    bool checkVersioningRequest(int newVer);
    void revertChanges(int newVersion);
    void revertChanges2to1();
    void revertChanges3to2();
    bool renameOldDataBase(int version);
    bool controlDatabase(int startAcYear);
    bool dataBaseIsEmpty(int startAcYear);
    bool controlGroupedCoursesDifferentCds_Reciprocy(std::vector<std::string> &idGrouped,
                                                     std::string specificCourse, int line_counter,
                                                     std::string idCourse);
    bool controlAGAINGroupedCoursesDifferentCds_Reciprocy();
    std::vector<std::string> insertCourses(const std::string &fin);
    void insertStudentsGrades(std::string fin);
    void addStudyPlan(std::string fin);
    void registerStudentsToSpecificYearCourses(std::vector<std::string>& courses,Student& stud, std::string acYearRegistration, int line_counter);
    void thereIsAHoleInTheCoursesCodes();
    void checkDistance(std::string& minor, std::string& major);
    void assignInfoAppealPerSession(std::string acYear,std::string idCorso,std::string session, std::vector<std::string> appealInfo);
    void assignAppealsToProf(std::string idCorso,std::string appeal, int startHour, int numSlots,std::vector<int> allProfsPerYearCourse);
    void assignAppealsToClassroom(std::string appeal,int startSlotHour,std::string classrooms,int numSlot);
    void ifThereAreAlreadyCoursesFillYYSemesterVar(StudyCourse& sCourse);
    int whatIsMyStudyCourse(Course& course, int acStartYear);
    void allErrorClasses();

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
    void readAllExamAppeals();
    void readVersion();
    void readAllMinDistanceRequest();

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

    ///add
    void addStuds(const std::string &fileIn);
    void addProfessors(const std::string &fileIn);
    void addClassrooms(const std::string &fileIn);
    std::vector<std::string> addStudyCourses(const std::string &fileIn);
    std::vector<std::string> addCourses(const std::string &fileIn);

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
    //key: "acYear-IdCourse"
    //value: corsi grouped
    std::map<std::string, std::vector<std::string>> _coursesGrouped;
    std::map<int,SessionYear> _acYearSessions;
    std::vector<std::string> _tempInfoNotActiveCoursesToWriteInTheDB;
    int _version;
    std::vector<std::string> _errorStringUniversity;
    void fillGroupedCourse (std::vector<std::string>&  idGroupedLetti, std::string& idCourse, std::string& acYear, int line);
};


#endif //PROGETTO_V3_UNIVERSITY_H
