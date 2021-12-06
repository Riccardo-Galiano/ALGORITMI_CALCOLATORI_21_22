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

    const int getNewStudentId() const ;
    const int getNewProfessorId() const;
    const int getNewClassroomId() const;
    const int getNewStudyCourseId() const;
    const std::string getNewCourseId()const;

    bool addStuds(const std::string &fileIn);
    bool addProfessors(const std::string &fileIn);
    bool addClassrooms(const std::string &fileIn);
    bool addStudyCourses(const std::string &fileIn);
    bool addCourses(const std::string &fileIn);
    bool updateStuds(const std::string &fin);
    bool updateProfessors(const std::string &fin);
    bool updateClassroom(const std::string &fin);
    bool insertCourses(const std::string &fin);
    bool enrollStudents(const std::string &fin);
    bool setSessionPeriod(const std::string &acYear,const std::string &wSession, const std::string &sSession,const std::string &aSession);
    std::vector<std::string> allProfsNoAvailabilities(int);

    void dbStudsWrite();
    void dbProfsWrite();
    void dbClassRoomWrite();
    void dbStudyCourseWrite();
    void dbCourseWrite();
    void dateSessionsWrite();
    bool setProfsNoAvailability(std::string acYear,const std::string& fin);
    void noAvailabilityWrite(int acYear);
    bool noAvailabilityPeriodIsEmpty(int id);
    bool isCurrentYear(int,int);

private:
    friend SpecificYearCourse;
    std::map<int, Professor> _professors;
    std::map<int, Student> _students;
    std::map<int, Classroom> _classroom;
    std::map<int, StudyCourse> _studyCourse;
    std::map<std::string, Course> _courses;
    SessionYear _acYearSessions; //da cambiare con mappa???????????
    void readStudents();
    void readProfessor();
    void readClassroom();
    void readStudyCourse();
    void readCourse();
    void readSessionAcYear();
    void readProfsNoAvailability();


};


#endif //PROGETTO_V3_UNIVERSITY_H
