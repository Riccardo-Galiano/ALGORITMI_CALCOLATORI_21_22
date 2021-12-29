//
// Created by lucam on 11/11/2021.
//

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include "Classroom.h"


Classroom::Classroom(int id, std::string AorL, std::string name, int seats, int examSeats) {
    _id = id;
    _name = name;
    _nSeats = seats;
    _nExamSeats = examSeats;
    if (AorL == "L") {
        _lab = true;
    } else if (AorL == "A") {
        _lab = false;
    } else {
        throw std::invalid_argument("errore aula o lab");
    }

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
const bool Classroom::getLab() const {
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

///setto il codice con n 0 davanti
std::string Classroom::setCod(int nCod) const {
    std::stringstream output;
    output << std::setfill('0') << std::setw(3) << nCod;
    return output.str();
}

bool Classroom::checkAvailability(Date& date,int slotHour) {
    std::stringstream ss;
    ss << date.toString() << ';' << slotHour;
    if(_disavailableRooms.count(ss.str())==1)
        return false;
    else
        return true;
}

void Classroom::setDisavailability(Date& date, int slotHour, int numSlot) {
    for(int s=0; s < numSlot; s++) {
        int hour = slotHour + (s*2);
        std::stringstream ss;
        ss << date.toString() << ';' << hour;
        _disavailableRooms.insert(std::pair<std::string, int>(ss.str(), 1));
    }
}

std::ostream &operator<<(std::ostream &room, const Classroom &s) {

    int nCod = s.getId();

    room << "A" << s.setCod(nCod) << ";";
    if (s.getLab())
        room << "L;";
    else
        room << "A;";

    room << s.getName() << ";" << s.getNSeats() << ";" << s.getNExamSeats();
    return room;
}
