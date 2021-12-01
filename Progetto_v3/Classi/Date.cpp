// Fig. 10.7: Date.cpp
// Date class member- and friend-function definitions.
#include <iostream>
#include <string>
#include <iomanip>
#include "Date.h"

using namespace std;

// initialize static member; one classwide copy
const array<unsigned int, 13> Date::_days{
   0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Date constructor
Date::Date(int year, int month, int day) {
   setDate(year, month, day);
}

// set month, day and year
void Date::setDate(int yy, int mm, int dd) {
   if (mm >= 1 && mm <= 12) {
      _month = mm;
   }
   else {
      throw invalid_argument{"Month must be 1-12"};
   }

   if (yy >= 1900 && yy <= 2100) {
      _year = yy;
   }
   else {
      throw invalid_argument{"Year must be >= 1900 and <= 2100"};
   }

   // test for a leap year
   if ((_month == 2 && leapYear(_year) && dd >= 1 && dd <= 29) ||
      (dd >= 1 && dd <= _days[_month])) {
      _day = dd;
   }
   else {
      throw invalid_argument{
         "Day is out of range for current month and year"};
   }
}

// overloaded prefix increment operator                 
Date& Date::operator++() {
   helpIncrement(); // increment date                   
   return *this; // reference return to create an lvalue
}

// overloaded postfix increment operator; note that the  
// dummy integer parameter does not have a parameter name
Date Date::operator++(int) {
   Date temp{*this}; // hold current state of object    
   helpIncrement();

   // return unincremented, saved, temporary object      
   return temp; // value return; not a reference return  
}

// add specified number of _days to date
Date& Date::operator+=(unsigned int additionalDays) {
   for (unsigned int i = 0; i < additionalDays; ++i) {
      helpIncrement();
   }

   return *this; // enables cascading                      
}

// if the year is a leap year, return true; otherwise, return false
bool Date::leapYear(int testYear) {
   return (testYear % 400 == 0 ||
      (testYear % 100 != 0 && testYear % 4 == 0));
}

// determine whether the day is the last day of the month
bool Date::endOfMonth(int testDay) const {
   if (_month == 2 && leapYear(_year)) {
      return testDay == 29; // last day of Feb. in leap year
   }
   else {
      return testDay == _days[_month];
   }
}

// function to help increment the date
void Date::helpIncrement() {
   // day is not end of month
   if (!endOfMonth(_day)) {
      ++_day; // increment day
   }
   else {
      if (_month < 12) { // day is end of month and month < 12
         ++_month; // increment month
         _day = 1; // first day of new month
      }
      else { // last day of year
         ++_year; // increment year
         _month = 1; // first month of new year
         _day = 1; // first day of new month
      }
   }
}

// overloaded output operator

/*
ostream& operator<<(ostream& output, const Date& d) {
   static string monthName[13]{"", "January", "February",
      "March", "April", "May", "June", "July", "August",
      "September", "October", "November", "December"};
   output << monthName[d._month] << ' ' << d._day << ", " << d._year;
   return output; // enables cascading
}
*/

ostream& operator<<(ostream& output, const Date& d){

    output << setfill('0') << d._year << "-" << setw(2) << d._month << "-" << setw(2) << d._day;
    return output;
}

// overloaded input operator
istream& operator>>(istream& input, Date& d){
    unsigned int month;
    unsigned int day;
    unsigned int year;
    input >> year >> month >> day;
    d.setDate(year, month, day);
    return input;
}

bool Date::operator>(const Date & d) const{
    if(_year>d._year){
        return true;
    }else if(_year==d._year&&_month>d._month){
        return true;
    }else if (_year==d._year&&_month==d._month&&_day>d._day){
        return true;
    }else{
        return false;
    }
}

bool Date::operator<(const Date & d) const {
    if(_year<d._year){
        return true;
    }else if(_year==d._year&&_month<d._month){
        return true;
    }else if (_year==d._year&&_month==d._month&&_day<d._day){
        return true;
    }else{
        return false;
    }
}