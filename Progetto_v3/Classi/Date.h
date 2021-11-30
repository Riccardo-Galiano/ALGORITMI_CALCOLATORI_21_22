
#ifndef DATE_H
#define DATE_H

#include <array>
#include <iostream>

class Date {
   friend std::ostream& operator<<(std::ostream&, const Date&);
   friend std::istream& operator>>(std::istream&, Date&);

public:
   Date() = delete;
   explicit Date(int y = 1900, int m = 1, int d = 1 ); // default constructor
   void setDate(int, int, int); // set year, month, day
   Date& operator++(); // prefix increment operator                  
   Date operator++(int); // postfix increment operator       
   Date& operator+=(unsigned int); // add _days, modify object
   bool operator>(const Date&) const;
   bool operator<(const Date&) const;

   static bool leapYear(int); // is year a leap year?
   bool endOfMonth(int) const; // is day at the end of month?
private:
   unsigned int _month;
   unsigned int _day;
   unsigned int _year;

   static const std::array<unsigned int, 13> _days; // _days per month
   void helpIncrement(); // utility function for incrementing date
};

#endif
