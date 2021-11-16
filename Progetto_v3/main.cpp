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

/*
template<typename T>
std::map<int, T> readMembers(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    std::map<int, T> members;
    if (!fIn.is_open()) {
        std::cerr << "rotto\n";
        return members;
    }
    std::string line;
    std::vector<std::string> tokens;
    char c;
    int nMatr;
    while (std::getline(fIn, line)) {
        //tokens = splittedLine(line);
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;
        if (members.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            members.insert(std::pair<int, T>(nMatr, T(tokens[1], tokens[2], tokens[3], nMatr)));
    }
    return members;
}
*/
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
            uni.insertStuds(argv[2]);
            break;
        }
        case add_professor: {
            uni.insertProfessors(argv[2]);
            break;
        }
        case add_classroom: {
            uni.insertClassrooms(argv[2]);
            break;
        }
        case add_course:
            break;
        case add_studyCourse:
            uni.insertStudyCourses(argv[2]);
            break;
        case update_student:
            break;
        case update_professor:
            break;
        case update_classroom:
            break;
        case insert_course:
            break;
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
