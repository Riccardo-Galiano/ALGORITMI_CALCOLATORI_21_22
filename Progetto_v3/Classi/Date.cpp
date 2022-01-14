
#include <iostream>
#include <string>
#include <iomanip>
#include "Date.h"
#include "cmath"

using namespace std;

const std::vector<unsigned int> Date::_days{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//costruttore
Date::Date(int year, int month, int day) {
    //default parameter: year=1900, month =1, day=1
    setDate(year, month, day);
    _weekday = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
}

Date::Date(const std::string &date) {
    std::stringstream ss;
    int yy, mm, dd;
    char c;
    ss << date;
    ss >> yy >> c >> mm >> c >> dd;
    setDate(yy, mm, dd);
}

// set year, month, day
void Date::setDate(int yy, int mm, int dd) {

    if (yy >= 1900 && yy <= 2100) {
        _year = yy;
    } else {
        throw invalid_argument{"Anno deve essere >= 1900 e <= 2100"};
    }

    if (mm >= 1 && mm <= 12) {
        _month = mm;
    } else {
        throw invalid_argument{"Mese deve essere 1-12"};
    }

    // test se anno bisestile
    if ((_month == 2 && leapYear(_year) && dd >= 1 && dd <= 29) ||
        (dd >= 1 && dd <= _days[_month])) {
        _day = dd;
    } else {
        throw invalid_argument{
                "Giorno non compatibile con mese e anno"};
    }
}

// test anno bisestile
bool Date::leapYear(int testYear) {
    return (testYear % 400 == 0 || (testYear % 100 != 0 && testYear % 4 == 0));
}

// controlla se il giorno è l'ultimo del mese
bool Date::endOfMonth(int testDay) const {
    if (_month == 2 && leapYear(_year)) {
        return testDay == 29; // last day of Feb. in leap year
    } else {
        return testDay == _days[_month];
    }
}

// incrementa di un giorno la data
void Date::helpIncrement() {
    if (!endOfMonth(_day)) {
        ++_day;
    } else {
        if (_month < 12) {
            ++_month;
            _day = 1;
        } else {
            ++_year;
            _month = 1;
            _day = 1;
        }
    }
}

unsigned int Date::getMonth() const {
    return _month;
}

unsigned int Date::getDay() const {
    return _day;
}

unsigned int Date::getYear() const {
    return _year;
}

/// ritorna il giorno della settimana
std::string Date::getWeekDay() {
    Date d(_year, _month, _day);
    int mon;
    if (d.getMonth() > 2)
        mon = d.getMonth(); //for march to december month code is same as month
    else {
        mon = (12 + d.getMonth()); //for Jan and Feb, month code will be 13 and 14
        d._year--; //decrease year for month Jan and Feb
    }
    int y = d.getYear() % 100; //last two digit
    int c = d.getYear() / 100; //first two digit
    int w = (d.getDay() + floor((13 * (mon + 1)) / 5) + y + floor(y / 4) + floor(c / 4) + (5 * c));
    w = w % 7;
    return _weekday[w];
}

///controlla se le sessioni si svolgono nelle settimane richieste
bool Date::checkGapGiven(int weeks, Date d) {
    int numDays = weeks * 7; //a quanti giorni corrisponde il numero di settimane da considerare
    // Date compare = this->add(numDays);//aggiungo alla data iniziale il numero di giorni corrispondente a quelle settimane
    // return d.isEqual(compare);
    Date comp = d;
    d += numDays;
    return d == comp;

}

///ritorna la data sottoforma di stringa AAAA-MM-GG
std::string Date::toString() {
    std::stringstream ss;
    ss << Date(_year, _month, _day);
    return ss.str();
}

///gap tra due date in giorni (start session e date dove quest'ultima è la data da controllare)
int Date::whatIsTheGap(Date &date) {
    bool areNotEqual = true;
    Date currentDate(_year, _month, _day);//data di inizio della sessione
    int gap = 0;
    while (areNotEqual) {
        if (currentDate == date) {//se uguali ho raggiunto la data finale del gap che volevo conoscere
            areNotEqual = false;
        } else {//se non uguali controllo il giorno successivo e aggiorno il gap
            ++currentDate;
            gap++;
        }
    }
    return gap;
}

// overloaded ++Data
Date &Date::operator++() {
    helpIncrement();
    return *this;
}

// overloaded Data++
Date Date::operator++(int) {
    Date temp(_year, _month, _day); // salva la data da ritornare
    temp.helpIncrement();
    // ritorna la data salvata non incrementata
    return temp; //valore (non ref) xk temp muore finita la chiamata dell'operatore
}

// Data += days
//!potrebbe servire per request_changes
//! funziona solo in avanti!
Date &Date::operator+=(unsigned int additionalDays) {
    for (unsigned int i = 0; i < additionalDays; i++) {
        helpIncrement();
    }
    return *this;
}

// overloaded output operator
ostream &operator<<(ostream &output, const Date &d) {
    //output: AAAA-MM-GG
    output << setfill('0') << d._year << "-" << setw(2) << d._month << "-" << setw(2) << d._day;
    return output;
}

// overloaded input operator
istream &operator>>(istream &input, Date &d) {
    unsigned int year;
    unsigned int month;
    unsigned int day;
    // input: AAAA-MM-GG
    input >> year >> month >> day;
    d.setDate(year, month, day);
    return input;
}

bool Date::operator>(const Date &date) const {
    if (_year > date._year) {
        return true;
    } else if (_year == date._year && _month > date._month) {
        return true;
    } else if (_year == date._year && _month == date._month && _day > date._day) {
        return true;
    } else {
        return false;
    }
}

bool Date::operator<(const Date &date) const {
    if (_year < date._year) {
        return true;
    } else if (_year == date._year && _month < date._month) {
        return true;
    } else if (_year == date._year && _month == date._month && _day < date._day) {
        return true;
    } else {
        return false;
    }
}

bool Date::operator<=(const Date &d) const {
    if (*this < d || *this == d) {
        return true;
    } else
        return false;
}

bool Date::operator>=(const Date &d) const {
    if (*this < d) {
        return false;
    } else
        return true;
}

bool Date::operator==(const Date &compare) const {
    return (_year == compare._year && _month == compare._month && _day == compare._day); //true or false
}


/* funzione uguale all'operatore ==
bool Date::isEqual(const Date compare) {
    return (_year == compare._year && _month == compare._month && _day == compare._day); //true or false
}
*/

/*funzione uguale all'operatore +=
Date Date::add(int daysToAdd) {
    Date toReturn(_year,_month,_day);
    for(int i=0; i<daysToAdd; i++){
        toReturn.helpIncrement();
    }
    return toReturn;
}
*/
