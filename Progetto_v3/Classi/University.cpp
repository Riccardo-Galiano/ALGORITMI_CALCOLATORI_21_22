//
// Created by lucam on 05/11/2021.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "University.h"

std::vector<std::string> splittedLine(const std::string &s) {
    char delimiter = ';';
    std::vector<std::string> toReturn;
    std::istringstream line(s);
    std::string token;
    while (std::getline(line, token, delimiter)) {
        toReturn.push_back(token);
    }
    return toReturn;
}

University::University() {
    readStudents();
    readProfessor();
}

int University::getNewStudentId() {
    int newStudId = 0;
    for (int i = 0; i < _students.size(); i++) {
        Student s = _students.at(i);
        if (s.getId() != newStudId)
            return newStudId;
        else
            newStudId++;
    }
    return newStudId;
}


bool University::insertStuds(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        std::cerr << "rotto\n";
        return false;
    }
    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line);
        int matr = getNewStudentId();
        _students.insert(std::pair<int, Student>(matr, Student(matr, tokens[0], tokens[1], tokens[2])));
    }
    return true;
}

bool University::insertProfs(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        std::cerr << "rotto\n";
        return false;
    }
    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line);
        int matr = getNewProfessorId();
        _professors.insert(std::pair<int, Professor>(matr, Professor(matr, tokens[0], tokens[1], tokens[2])));
    }
    return true;
}

int University::getNewProfessorId() {
    int newProfId = 0;
    for (int i = 0; i < _professors.size(); i++) {
        Professor d = _professors.at(i);
        if (d.getId() != newProfId)
            return newProfId;
        else
            newProfId++;
    }
    return newProfId;
}

/// come faccio a creare il file se non lo trovo?
void University::readStudents() {
    std::ifstream fileIn("db_studenti.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw std::invalid_argument("errore apertura database studenti");
    }
    std::string line;
    std::vector<std::string> tokens;
    char c;
    int nMatr;
    while (std::getline(fileIn, line)) {
        tokens = splittedLine(line);
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;
        if (_students.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, tokens[1], tokens[2], tokens[3])));
    }
}

void University::readProfessor() {
    std::ifstream fileIn("db_professori.txt");
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura database professori");
    }
    std::string line;
    std::vector<std::string> tokens;
    char c;
    int nMatr;
    while (std::getline(fileIn, line)) {
        tokens = splittedLine(line);
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;
        if (_professors.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _professors.insert(std::pair<int, Professor>(nMatr, Professor(nMatr, tokens[1], tokens[2], tokens[3])));
    }
}

bool University::insertClassroom(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura database aule");
        return false;
    }
    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line);
        int id = getNewClassroomId();
        _classroom.insert(std::pair<int, Classroom>(id, Classroom(id, tokens[0], tokens[1], tokens[2])));
    }
    return true;
}
/// potrebbe non esserci niente ad un certo i
int University::getNewClassroomId() {
    int newRoomId = 0;
    for (int i = 0; i < _classroom.size(); i++) {
        if (_classroom.at(i).getId() != newRoomId)
            return newRoomId;
        else
            newRoomId++;
    }
    return newRoomId;
}

return false;
}
