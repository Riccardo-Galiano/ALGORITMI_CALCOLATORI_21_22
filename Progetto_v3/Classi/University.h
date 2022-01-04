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

    ///DB management
    bool versioning(std::string);
    bool renameOldDataBase(int);
    void dbNewStudentWrite();
    void dbNewProfessorWrite();
    void dbNewAuleWrite();
    bool controlDatabase(int);
    bool dataBaseIsEmpty(int);
    bool controlGroupedCourses(int,std::vector<std::string>&,std::string,int,std::string);
    void controlReciprocyGrouped();

    ///Input (from DB)
    void readCourseNotActive();
    void readStudyPlan();
    void readStudents(); ///CHANGE
    void readProfessor(); ///CHANGE
    void readClassroom(); ///CHANGE
    void readStudyCourse();
    void readCourse();
    void readSessionAcYear();
    void readProfsNoAvailability();
    void readPassedAppeals();
    void readAllExamAppeals();
    int readVersion();

    ///Output(to DB)
    void dbStudsWrite(); ///CHANGE
    void dbCourseNotActive();
    void dbProfsWrite(); ///CHANGE
    void dbClassRoomWrite(); ///CHANGE
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
    bool addStuds(const std::string &fileIn);  ///CHANGE
    bool addProfessors(const std::string &fileIn); ///CHANGE
    bool addClassrooms(const std::string &fileIn); ///CHANGE
    bool addStudyCourses(const std::string &fileIn);
    bool addCourses(const std::string &fileIn);

    ///update
    bool updateStuds(const std::string &fin); ///CHANGE
    bool updateProfessors(const std::string &fin); ///CHANGE
    bool updateClassroom(const std::string &fin); ///CHANGE


    bool insertCourses(const std::string &fin);
    bool insertStudentsGrades(std::string fin);
    bool addStudyPlan(std::string fin);
    void updateStudyPlan(std::string fin);
    void registerStudentsToSpecificYearCourses(std::vector<std::string>& courses,Student& stud, int acYearRegistration);

private:
    void thereIsAHoleInTheCoursesCodes();
    void checkDistance(std::string&, std::string&);
    friend SpecificYearCourse;
    std::map<int, Professor> _professors;
    std::map<int, Student> _students;
    std::map<int, Classroom> _classroom;
    std::map<int, StudyCourse> _studyCourse;
    std::map<std::string, Course> _courses;
    std::map<int,SessionYear> _acYearSessions;
    std::vector<std::string> _tempInfoNotActiveCoursesToWriteInTheDB;
};


#endif //PROGETTO_V3_UNIVERSITY_H
