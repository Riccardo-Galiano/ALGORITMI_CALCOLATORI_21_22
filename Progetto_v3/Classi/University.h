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

class University {
public:

    University();

    ///session management
    bool setProfsNoAvailability(std::string ,const std::string& );
    bool setSessionPeriod( std::string &, std::string &, std::string &,std::string &);
    std::vector<std::string> allProfsNoAvailabilities();
    bool setExamDate(std::string,std::string);
    bool setMinDistance(std::string ,std::string );
    void minDistanceRequestWrite();

    ///DB management
    bool versioning(int);
    bool checkVersioningRequest(int);
    void revertChanges(int);
    void revertChanges2to1();
    void revertChanges3to2();
    bool renameOldDataBase(int);
    bool controlDatabase(int);
    bool dataBaseIsEmpty(int);
    bool controlGroupedCourses(int,std::vector<std::string>&,std::string,int,std::string);
    void controlReciprocyGrouped();
    bool insertCourses(const std::string &fin);
    bool insertStudentsGrades(std::string fin);
    bool addStudyPlan(std::string fin);
    void registerStudentsToSpecificYearCourses(std::vector<std::string>& courses,Student& stud, int acYearRegistration);
    void thereIsAHoleInTheCoursesCodes();
    void checkDistance(std::string&, std::string&);


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
    void writeVersion(int);


    ///get new ID
    const int getNewStudentId() const ;
    const int getNewProfessorId() const;
    const int getNewClassroomId() const;
    const int getNewStudyCourseId() const;
    const std::string getNewCourseId()const;

    ///add
    bool addStuds(const std::string &fileIn);
    bool addProfessors(const std::string &fileIn);
    bool addClassrooms(const std::string &fileIn);
    bool addStudyCourses(const std::string &fileIn);
    bool addCourses(const std::string &fileIn);

    ///update
    bool updateStuds(const std::string &fin);
    bool updateProfessors(const std::string &fin);
    bool updateClassroom(const std::string &fin);
    void updateStudyPlan(std::string fin);

private:

    friend SpecificYearCourse;
    std::map<int, Professor> _professors;
    std::map<int, Student> _students;
    std::map<int, Classroom> _classroom;
    std::map<int, StudyCourse> _studyCourse;
    std::map<std::string, Course> _courses;
    std::map<int,SessionYear> _acYearSessions;
    std::vector<std::string> _tempInfoNotActiveCoursesToWriteInTheDB;
    int _version;
};


#endif //PROGETTO_V3_UNIVERSITY_H
