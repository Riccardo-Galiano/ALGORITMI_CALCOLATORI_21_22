//
// Created by lucam on 11/11/2021.
//

#include <stdexcept>
#include "Classroom.h"


 Classroom::Classroom(int id, std::string AorL, std::string name, int seats, int examSeats) {
_id=id;
_name=name;
_nSeats=seats;
_nExamSeats=examSeats;
if(AorL == "L"){
    _lab=true;
}else if(AorL == "A"){
    _lab=false;
}else{
    throw std::invalid_argument("errore aula o lab");
}

}

int Classroom::getId() const {
    return _id;
}

std::string Classroom::getName() const {
    return _name;
}

bool Classroom::getLab() const {
    return _lab;
}

int Classroom::getNSeats() const {
    return _nSeats;
}

int Classroom::getNExamSeats() const {
    return _nExamSeats;
}
