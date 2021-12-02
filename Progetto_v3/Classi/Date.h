
#ifndef DATE_H
#define DATE_H

#include <array>
#include <iostream>
#include <vector>

class Date {
   friend std::ostream& operator<<(std::ostream&, const Date&);
   friend std::istream& operator>>(std::istream&, Date&);

public:
   Date() = delete;
   explicit Date(int y = 1900, int m = 1, int d = 1 ); // default constructor
   void setDate(int, int, int); // set year, month, day
   Date& operator++(); // prefix increment operator                  
   Date operator++(int); // postfix increment operator       
   Date& operator+=(unsigned int);
   bool operator>(const Date&) const;
   bool operator<(const Date&) const;

   static bool leapYear(int); // anno bisestile?
   bool endOfMonth(int) const; // fine del mese?
private:
   unsigned int _month;
   unsigned int _day;
   unsigned int _year;

   static const std::vector<unsigned int> _days; // _days per mese
   void helpIncrement();
};

#endif
