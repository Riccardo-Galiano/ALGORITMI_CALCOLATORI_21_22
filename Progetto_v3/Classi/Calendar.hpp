//
// Created by Andrea on 29/11/2021.
//

#ifndef PROGETTO_V3_CALENDAR_HPP
#define PROGETTO_V3_CALENDAR_HPP

#include <string>
#include <vector>
#include <cmath>
#include "ExamDay.hpp"



enum {
    Gennaio, Febbraio, Marzo, Aprile, Maggio, Giugno, Luglio, Agosto, Settembre, Ottobre, Novembre, Dicembre
};

using namespace std;
string weekday[7] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

class Calendar {
private:
    int _year;
    std::vector<int> _daysPerYear = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //_daysPerYear[Gennaio] = 31
    std::vector<ExamDay> _examDays;
    std::vector<bool> _isExamDay; //bitmap, false: not exam day, true: exam day

public:
    explicit Calendar(int);
    static string zellersAlgorithm(int day, int month, int year);

};


#endif //PROGETTO_V3_CALENDAR_HPP
