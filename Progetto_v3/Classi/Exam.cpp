//
// Created by lucam on 05/11/2021.
//

#include "Exam.h"

Exam::Exam(int time, int enterTime, int leaveTime, const std::string &mode, std::string place) : _time(time), _enterTime(enterTime), _leaveTime(leaveTime), _mode(mode), _place(place) {}

int Exam::getTime() const {
    return _time;
}

int Exam::getEnterTime() const {
    return _enterTime;
}

int Exam::getLeaveTime() const {
    return _leaveTime;
}

const std::string &Exam::getMode() const {
    return _mode;
}

const std::string &Exam::getPlace() const {
    return _place;
}
