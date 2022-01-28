//
// Created by lucam on 01/12/2021.
//

#ifndef DATE_H
#define DATE_H

#include <array>
#include <iostream>
#include <vector>

class Date {


public:
    //Date() = default;
    Date(const std::string& date);
    explicit Date(int year = 1900, int month = 1, int day = 1); // default constructor
    void setDate(int yy, int mm, int dd); // set year, month, day
    Date &operator++(); // prefix increment operator
    Date operator++(int); // postfix increment operator
    Date &operator--(); // prefix decrement operator
    Date operator--(int); // postfix decrement operator
    Date &operator+=(unsigned int);

    bool operator>(const Date & date) const;
    bool operator<(const Date & date) const;
    bool operator<=(const Date & date) const;
    bool operator>=(const Date & date) const;
    bool operator==(const Date & date) const;

    static bool leapYear(int testYear); // anno bisestile?
    bool endOfMonth(int testDay) const; // fine del mese?
    Date add(int daysToAdd);
    Date subtract(int  daysToSubtract);

    std::string getWeekDay();
    bool checkGapGiven(int weeks, Date d);

    int getYear() const;
    std::string toString();

    int getMonth() const;
    int getDay() const;

    int whatIsTheGap(Date& date);
    void helpIncrement();
    void helpDecrement();

private:
    int _day;
    int _month;
    int _year;
    std::vector<std::string> _weekday;

};
std::ostream &operator<<(std::ostream & output, const Date & d);
std::istream &operator>>(std::istream & input , Date & d);

#endif
