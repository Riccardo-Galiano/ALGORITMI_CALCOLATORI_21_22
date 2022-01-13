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
    std::vector<std::string> setProfsNoAvailability(std::string ,const std::string& );
    std::vector<std::string> setSessionPeriod( std::string &, std::string &, std::string &,std::string &);
    std::vector<std::string> allProfsNoAvailabilities();
    std::vector<std::string> setExamDate(std::string,std::string);
    std::vector<std::string> setMinDistance(std::string ,std::string );
    std::vector<std::string> requestChanges(std::string,std::string);
    void minDistanceRequestWrite();

    ///DB management
    std::vector<std::string> versioning(int);
    bool checkVersioningRequest(int);
    void revertChanges(int);
    void revertChanges2to1();
    void revertChanges3to2();
    bool renameOldDataBase(int);
    bool controlDatabase(int);
    bool dataBaseIsEmpty(int);
    bool controlGroupedCoursesDifferentCds_Reciprocy(std::vector<std::string> &idGrouped,
                                                     std::string nameCourse, int line_counter,
                                                     std::string idCourse);
    bool controlAGAINGroupedCoursesDifferentCds_Reciprocy();
    std::vector<std::string> insertCourses(const std::string &fin);
    std::vector<std::string> insertStudentsGrades(std::string fin);
    std::vector<std::string> addStudyPlan(std::string fin);
    void registerStudentsToSpecificYearCourses(std::vector<std::string>& courses,Student& stud, int acYearRegistration);
    void thereIsAHoleInTheCoursesCodes();
    void checkDistance(std::string&, std::string&);
    void assignInfoAppealPerSession(std::string,std::string,std::string, std::vector<std::string> infoPerSession);
    void assignAppealsToProf(std::string,std::string, int, int,std::vector<int>);
    void assignAppealsToClassroom(std::string,int,std::string,int);
    void ifThereAreAlreadyCoursesFillYYSemesterVar(StudyCourse&);
    int whatIsMyStudyCourse(Course& course);
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
    void writeVersion(int);


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
    std::vector<std::string> updateStudyPlan(std::string fin);

private:
    SystemLog _universityLog;
    std::map<int, Professor> _professors;
    std::map<int, Student> _students;
    std::map<int, Classroom> _classroom;
    std::map<int, StudyCourse> _studyCourse;
    std::map<std::string, Course> _courses;
    //key: "acYear-IdCourse"
    //value: corsi grouped
    std::map<std::string, std::vector<std::string>> _coursesGrouped;
    std::map<int,SessionYear> _acYearSessions;
    std::vector<std::string> _tempInfoNotActiveCoursesToWriteInTheDB;
    int _version;
    std::vector<std::string> _errorStringUniversity;
    void fillGroupedCourse (std::vector<std::string>&, std::string&, std::string&, int);
};


#endif //PROGETTO_V3_UNIVERSITY_H
