#include <iostream>
#include "./Classi/University.h"
#include <fstream>
#include <sstream>
#include "Classi/Student.h"
#include <vector>
#include "Classi/Professor.h"
#include "Classi/DbException.h"


enum {
    add_student,
    add_professor,
    add_classroom,
    add_course,
    add_studyCourse,
    update_student,
    update_professor,
    update_classroom,
    insert_course,
    enroll_students,
    set_session_period,
    set_availability,
    set_exam_date
};


int returnCode(char *argv[]) {
    std::string paramInput = argv[1];
    std::string secondParamInput = argv[2];
    if (paramInput.compare("-a:s") == 0)
        return add_student;
    else if (paramInput.compare("-a:d") == 0)
        return add_professor;
    else if (paramInput.compare("-a:a") == 0)
        return add_classroom;
    else if (paramInput.compare("-a:c") == 0)
        return add_course;
    else if (paramInput.compare("-a:f") == 0)
        return add_studyCourse;
    else if (paramInput.compare("-u:s") == 0)
        return update_student;
    else if (paramInput.compare("-u:d") == 0)
        return update_professor;
    else if (paramInput.compare("-u:a") == 0)
        return update_classroom;
    else if (paramInput.compare("-i:c") == 0)
        return insert_course;
    else if (paramInput.compare("-e:s") == 0)
        return enroll_students;
    else if(paramInput.compare("-s") == 0 && secondParamInput.compare("current_a")==0)
        return set_session_period;
    else if(paramInput.compare("-s") == 0 && secondParamInput.compare("set_availability")==0)
        return set_availability;
    else if (paramInput.compare("-g")==0)
        return set_exam_date;
    return -1;
}

void startProgram(University &uni, char *argv[]) {
    int code = returnCode(argv);
    switch (code) {
        case add_student: {
            uni.addStuds(argv[2]);
            break;
        }
        case add_professor: {
            uni.addProfessors(argv[2]);
            break;
        }
        case add_classroom: {
            uni.addClassrooms(argv[2]);
            break;
        }
        case add_course: {
            uni.addCourses(argv[2]);
            break;
        }
        case add_studyCourse: {
            uni.addStudyCourses(argv[2]);
            break;
        }
        case update_student: {
            uni.updateStuds(argv[2]);
            break;
        }
        case update_professor: {
            uni.updateProfessors(argv[2]);
            break;
        }
        case update_classroom: {
            uni.updateClassroom(argv[2]);
            break;
        }
        case insert_course: {
            uni.insertCourses(argv[2]);
            break;
        }
        case enroll_students:{
            uni.enrollStudents(argv[2]);
            break;
        }
        case set_session_period:{
           uni.setSessionPeriod(argv[3],argv[4],argv[5],argv[6]);
            break;
        }
        case set_availability: {
            uni.setProfsNoAvailability(argv[3], argv[4]);
            break;
        }
        case set_exam_date:{

            break;
        }

        default:
            throw std::invalid_argument("comando non trovato");
    }
};


using namespace std;

int main(int argc, char *argv[]) {
    /*
    try {
        std::map<int, Student> students = readMembers<Student>(argv[1]);
        std::map<int, Professor> professors = readMembers<Professor>(argv[2]);
        return 2;
    } catch (logic_error &) {
        cerr << "not valid input!!";

        return -1;
    }
*/
    University poliTo;
    if (argc < 3) {
        throw std::invalid_argument("errore numero parametri linea di comando");
    }
    startProgram(poliTo, argv);


    return 0;
}
