//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_EXAM_H
#define PROGETTO_V3_EXAM_H


#include <string>

class Exam {
public:
    Exam(){};
    Exam(int time, int enterTime, int leaveTime, const std::string & mode, std::string place);
    int getTime() const;
    int getEnterTime() const;
    int getLeaveTime() const;
    const std::string &getMode() const;
    char getPlace() const;
    int howManySlots()const;

private:
    int _time; //tempo dell'esame
    int _enterTime; //tempo per entrare
    int _leaveTime; //tempo per uscire
    std::string _mode; //modalità
    char _place; //luogo

};


#endif //PROGETTO_V3_EXAM_H
