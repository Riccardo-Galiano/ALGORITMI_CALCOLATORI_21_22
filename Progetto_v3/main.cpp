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
    insert_course
};


int returnCode(std::string paramInput) {
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
    return -1;
}

void startProgram(University &uni, char *argv[]) {
    int code = returnCode(argv[1]);
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
        default:
            throw std::invalid_argument("comando non trovato");
            break;
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
    if (argc != 3) {
        throw std::invalid_argument("errore numero parametri linea di comando");
    }
    startProgram(poliTo, argv);


    return 0;
}
