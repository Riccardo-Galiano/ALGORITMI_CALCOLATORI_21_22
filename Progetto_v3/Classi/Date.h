//
// Created by lucam on 01/12/2021.
//

#ifndef DATE_H
#define DATE_H

#include <array>
#include <iostream>
#include <vector>

class Date {
    friend std::ostream &operator<<(std::ostream & output, const Date & d);
    friend std::istream &operator>>(std::istream & input , Date & d);

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

    unsigned int getYear() const;
    std::string toString();

    bool isEqual(const Date compare);
    unsigned int getMonth() const;
    unsigned int getDay() const;

    int whatIsTheGap(Date& date);

private:
    unsigned int _day{};
    unsigned int _month{};
    unsigned int _year{};
    std::vector<std::string> _weekday;
    static const std::vector<unsigned int> _days; // num giorni per mese
    void helpIncrement();
    void helpDecrement();
};

#endif
