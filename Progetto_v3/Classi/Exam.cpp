//
// Created by lucam on 05/11/2021.
//

#include "Exam.h"

Exam::Exam(int time, int enterTime, int leaveTime, const std::string &mode, std::string place) : _time(time),
                                                                                          _enterTime(enterTime),
                                                                                          _leaveTime(leaveTime),
                                                                                          _mode(mode), _place(place) {}
