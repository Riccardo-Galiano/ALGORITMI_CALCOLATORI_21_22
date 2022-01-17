//
// Created by lucam on 01/12/2021.
//

#ifndef DATE_H
#define DATE_H

#include <array>
#include <iostream>
#include <vector>

class Date {
    friend std::ostream &operator<<(std::ostream &, const Date &);
    friend std::istream &operator>>(std::istream &, Date &);

public:
    //Date() = default;
    Date(const std::string&);


    explicit Date(int y = 1900, int m = 1, int d = 1); // default constructor
    void setDate(int, int, int); // set year, month, day
    Date &operator++(); // prefix increment operator
    Date operator++(int); // postfix increment operator
    Date incrementOf(int);
    Date &operator+=(unsigned int);

    bool operator>(const Date &) const;
    bool operator<(const Date &) const;
    bool operator<=(const Date &) const;
    bool operator>=(const Date &) const;
    bool operator==(const Date &) const;

    static bool leapYear(int); // anno bisestile?
    bool endOfMonth(int) const; // fine del mese?
    Date add(int);
    Date subtract(int); //LUCA E' PER TE

    std::string getWeekDay();
    bool checkGapGiven(int weeks, Date d);

    unsigned int getYear() const;
    std::string toString();

    bool isEqual(const Date);
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
};

#endif
