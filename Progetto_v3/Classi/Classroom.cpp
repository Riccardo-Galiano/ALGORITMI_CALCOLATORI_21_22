//
// Created by lucam on 11/11/2021.
//

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include "Classroom.h"
#include "Parse.hpp"

///costruttori
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
        throw std::invalid_argument("E' presente un carattere diverso da A o L ");
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
void Classroom::setName(const std::string &name) {
    _name = name;
}

///aggiorna il tipo: lab o aula
void Classroom::setType(const bool &LorA) {
    _lab = LorA;
}

///aggiorna la capienza
void Classroom::setNSeats(const int &NSeats) {
    _nSeats = NSeats;
}

///aggiorna la capienza per l'esame
void Classroom::setNExamSeats(const int &NExamSeats) {
    _nExamSeats = NExamSeats;
}

///controlla la indisponibilità
bool Classroom::checkAvailability(Date& date,int slotHour) {
    std::stringstream ss;
    ss << date.toString() << ';' << slotHour;
    if(_disavailableRooms.count(ss.str())==1)
        return false;
    else
        return true;
}

///setta la indisponibilità
void Classroom::setDisavailability(Date& date, int slotStartHour, int numSlot) {
    for(int slot=0; slot < numSlot; slot++) {
        int hour = slotStartHour + (slot * 2);
        std::stringstream ss;
        ss << date.toString() << ';' << std::setfill('0')<<std::setw(2)<< hour;
        _disavailableRooms.insert(std::pair<std::string, int>(ss.str(), 1));
    }
}

///prende le altre info della versioning
std::string Classroom::getOthersInfo()const {
    std::stringstream ss;
    ss<<_othersInfoClassroom._drawingTable <<";"<<_othersInfoClassroom._computer <<";"<<_othersInfoClassroom._projector<<";"<<_othersInfoClassroom._blackBoard;
    return ss.str();
}

///prende il numero di tavoli da disegno
int Classroom::getDrawingTable() const{
    return _othersInfoClassroom._drawingTable;
}

///prende il numero di computer
int Classroom::getComputer()const {
    return _othersInfoClassroom._computer;
}

///prende il numero di proiettori
int Classroom::getProjector() const{
    return _othersInfoClassroom._projector;
}

///prende il numero di lavagne
int Classroom::getBlackBoard() const{
    return _othersInfoClassroom._blackBoard;
}

///setta il numero dei computer
void Classroom::setComputer(int computer) {
    _othersInfoClassroom._computer = computer;
}

///setta il numero dei tavoli da disegno
void Classroom::setDrawingTable(int drawingTable) {
    _othersInfoClassroom._drawingTable = drawingTable;
}

///setta il numero dei proiettori
void Classroom::setProjector(int projector) {
    _othersInfoClassroom._projector = projector;
}

///setta il numero delle lavagne
void Classroom::setBlackBoard(int blackBoard) {
   _othersInfoClassroom._blackBoard = blackBoard;
}

///toglie l'appello dalle indisponibilità dell'aula
void Classroom::eraseThisAppealFromClassrooms(Date &date, int startSlot, int numSlots) {
    for(int i = 0; i<numSlots; i++){
        int slot = startSlot + 2*i;
        if (slot == 8)
            _disavailableRooms.erase(date.toString() + ";0" + std::to_string(slot));
        else
            _disavailableRooms.erase(date.toString() + ";" + std::to_string(slot));
    }
}

///overload output operator
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
