//
// Created by lucam on 11/11/2021.
//

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include "Classroom.h"
#include "Parse.hpp"


Classroom::Classroom(int id, std::string AorL, std::string name, int seats, int examSeats) {
    _id = id;
    _name = name;
    _nSeats = seats;
    _nExamSeats = examSeats;
    if (AorL == "L") {
        _lab = true;
    } else if (AorL == "A") {
        _lab = false;
    }else {
        throw std::invalid_argument("E' presente un carattere diverso da A o L");
    }
    _othersInfoClassroom._blackBoard = -1;
    _othersInfoClassroom._computer = -1;
    _othersInfoClassroom._drawingTable = -1;
    _othersInfoClassroom._projector = -1;
}

Classroom::Classroom(int id, std::string AorL, std::string name, int seats, int examSeats, int blackBoard, int computer, int drawingTable, int projector) {
    _id = id;
    _name = name;
    _nSeats = seats;
    _nExamSeats = examSeats;
    if (AorL == "L") {
        _lab = true;
    } else if (AorL == "A") {
        _lab = false;
    }else{
        throw std::invalid_argument("E' presente un carattere diverso da A o L");
    }
    _othersInfoClassroom._blackBoard = blackBoard;
    _othersInfoClassroom._computer = computer;
    _othersInfoClassroom._drawingTable = drawingTable;
    _othersInfoClassroom._projector = projector;
}


///prende l'id
const int Classroom::getId() const {
    return _id;
}

///prende il nome
const std::string Classroom::getName() const {
    return _name;
}

///prende il tipo: lab o aula
const bool Classroom::isThisLab() const {
    return _lab;
}

///prende la capienza
const int Classroom::getNSeats() const {
    return _nSeats;
}

///prende la capienza per gli esami
const int Classroom::getNExamSeats() const {
    return _nExamSeats;
}

///aggiorna il nome
void Classroom::updateName(const std::string &name) {
    _name = name;
}

///aggiorna il tipo: lab o aula
void Classroom::updateType(const bool &LorA) {
    _lab = LorA;
}

///aggiorna la capienza
void Classroom::updateNSeats(const int &NSeats) {
    _nSeats = NSeats;
}

///aggiorna la capienza per l'esame
void Classroom::updateNExamSeats(const int &NExamSeats) {
    _nExamSeats = NExamSeats;
}

bool Classroom::checkAvailability(Date& date,int slotHour) {
    std::stringstream ss;
    ss << date.toString() << ';' << slotHour;
    if(_disavailableRooms.count(ss.str())==1)
        return false;
    else
        return true;
}

void Classroom::setDisavailability(Date& date, int slotStartHour, int numSlot) {
    for(int slot=0; slot < numSlot; slot++) {
        int hour = slotStartHour + (slot * 2);
        std::stringstream ss;
        ss << date.toString() << ';' << std::setfill('0')<<std::setw(2)<< hour;
        _disavailableRooms.insert(std::pair<std::string, int>(ss.str(), 1));
    }
}

std::string Classroom::getOthersInfo() {
    std::stringstream ss;
    ss<<_othersInfoClassroom._drawingTable <<";"<<_othersInfoClassroom._computer <<";"<<_othersInfoClassroom._projector<<";"<<_othersInfoClassroom._blackBoard;
    return ss.str();
}

int Classroom::getDrawingTable() {
    return _othersInfoClassroom._drawingTable;
}

int Classroom::getComputer() {
    return _othersInfoClassroom._computer;
}

int Classroom::getProjector() {
    return _othersInfoClassroom._projector;
}

int Classroom::getBlackBoard() {
    return _othersInfoClassroom._blackBoard;
}

void Classroom::updateComputer(int computer) {
    _othersInfoClassroom._computer = computer;
}

void Classroom::updateDrawingTable(int drawingTable) {
    _othersInfoClassroom._drawingTable = drawingTable;
}

void Classroom::updateProjector(int projector) {
    _othersInfoClassroom._projector = projector;
}

void Classroom::updateBlackBoard(int blackBoard) {
   _othersInfoClassroom._blackBoard = blackBoard;
}

void Classroom::eraseThisAppealFromClassrooms(Date &date, int startSlot, int numSlots) {
    ///ciclare per numslots!!!!!!!!!!
    for(int i = 0; i<numSlots; i++){
        int slot = startSlot + 2*i;
        if (slot == 8)
            _disavailableRooms.erase(date.toString() + ";0" + std::to_string(slot));
        else
            _disavailableRooms.erase(date.toString() + ";" + std::to_string(slot));
    }
}


std::ostream &operator<<(std::ostream &room, const Classroom &c) {
    std::string settedId = Parse::setId('A', 3, c.getId());
    room << settedId << ";";
    if (c.isThisLab())
        room << "L;";
    else
        room << "A;";

    room << c.getName() << ";" << c.getNSeats() << ";" << c.getNExamSeats();
    return room;
}
