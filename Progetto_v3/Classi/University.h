//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_UNIVERSITY_H
#define PROGETTO_V3_UNIVERSITY_H

#include "AccademicYear.h"
#include "Professor.h"
#include "Student.h"
#include "Classroom.h"

class University {
public:

    University();

    int getNewStudentId();

    int getNewProfessorId();

    int getNewClassroomId();

    bool insertStuds(const std::string &fileIn);

    bool insertProfessors(const std::string &fileIn);

    bool insertClassroom(const std::string &fileIn);

private:
    /// annoInizio-annoAccademico
    std::map<int, AccademicYear> _accademicYears;
    std::map<int, Professor> _professors;
    std::map<int, Student> _students;
    std::map<int, Classroom> _classroom;

    void readStudents();

    void readProfessor();

    void readClassroom();


};


#endif //PROGETTO_V3_UNIVERSITY_H
