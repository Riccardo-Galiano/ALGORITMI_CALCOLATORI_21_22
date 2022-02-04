//
// Created by lucam on 05/11/2021.
//

#include "Exam.h"
///costruttore
Exam::Exam(int time, int enterTime, int leaveTime, const std::string &mode, std::string place) : _time(time), _enterTime(enterTime), _leaveTime(leaveTime), _mode(mode), _place(place[0]) {}

///prende il tempo dell'esame
int Exam::getTime() const {
    return _time;
}
///prende il tempo di entrata
int Exam::getEnterTime() const {
    return _enterTime;
}
///prende il tempo di uscita
int Exam::getLeaveTime() const {
    return _leaveTime;
}
///prende la modalità
const std::string &Exam::getMode() const {
    return _mode;
}

///prende il luogo(aula o lab)
char Exam::getPlace() const {
    return _place;
}

///quanti slot servono per un esame
int Exam::howManySlots()const {
    int sum = this->getTime() + this->getEnterTime() + this->getLeaveTime();//sommo le ore per l'esame per entrare e uscire dall'aula
    int num_slots = 0;
    int duration = 2 * 60; //slot da due ore
    num_slots = sum / duration + 1;//+1 perchè se venisse un numero razionale il compilatore raggionerebbe per difetto ma a noi interessa ragionare per eccesso
    return num_slots;
}
