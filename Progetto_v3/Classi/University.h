//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_UNIVERSITY_H
#define PROGETTO_V3_UNIVERSITY_H

#include "AccademicYear.h"
#include "Professor.h"
#include "Student.h"
#include "Classroom.h"
#include "StudyCourse.h"
#include "Course.h"

class University {
public:

    University();

    const int getNewStudentId() const ;

    const int getNewProfessorId() const;

    const int getNewClassroomId() const;

    const int getNewStudyCourse() const;

    const int getNewCourse()const;

    bool insertStuds(const std::string &fileIn);

    bool insertProfessors(const std::string &fileIn);

    bool insertClassroom(const std::string &fileIn);

    bool insertStudyCourse(const std::string &fileIn);

    bool insertCourse(const std::string &fileIn);

private:
    /// annoInizio-annoAccademico
    std::map<int, AccademicYear> _accademicYears;
    std::map<int, Professor> _professors;
    std::map<int, Student> _students;
    std::map<int, Classroom> _classroom;
    std::map<int, StudyCourse> _studyCourse;
    std::map<int, Course> _courses;

    void readStudents();

    void readProfessor();

    void readClassroom();

    void readStudyCourse();

    void readCourse();


};


#endif //PROGETTO_V3_UNIVERSITY_H
