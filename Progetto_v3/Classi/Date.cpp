
#include <iostream>
#include <string>
#include <iomanip>
#include "Date.h"
#include "cmath"
using namespace std;

const std::vector<unsigned int> Date::_days{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//costruttore
Date::Date(int year, int month, int day) {
   setDate(year, month, day);
    _weekday = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
}

Date::Date(const std::string &date) {
    std::stringstream ss;
    int yy, mm, dd;
    char c;
    ss << date;
    ss >> yy >> c >> mm >> c >> dd;
    setDate(yy,mm,dd);
}

// set year, month, day
void Date::setDate(int yy, int mm, int dd) {

    if (yy >= 1900 && yy <= 2100) {
        _year = yy;
    }
    else {
        throw invalid_argument{"l'anno deve essere >= 1900 e <= 2100\n"};
    }

    if (mm >= 1 && mm <= 12) {
      _month = mm;
   }
   else {
      throw invalid_argument{"Il mese deve essere tra 1-12\n"};
   }

   // test se anno bisestile
   if ((_month == 2 && leapYear(_year) && dd >= 1 && dd <= 29) || (dd >= 1 && dd <= _days[_month])) {
      _day = dd;
   }
   else if( dd >31) {
      throw std::invalid_argument("Il giorno non puo' essere maggiore di 31\n");
   }else{
      throw invalid_argument{"Per questo anno e questo mese il giorno scelto non va bene\n"};
   }
}

// overloaded prefix increment operator                 
Date& Date::operator++() {
   helpIncrement(); // increment date                   
   return *this; // reference return to create an lvalue
}

// overloaded postfix increment operator
Date Date::operator++(int) {
   Date temp(_year,_month,_day); // hold current state of object
   temp.helpIncrement();
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

// controllo se un anno bisestile
bool Date::leapYear(int testYear) {
   return (testYear % 400 == 0 ||  (testYear % 100 != 0 && testYear % 4 == 0));
}

// determina la fine di un mese
bool Date::endOfMonth(int testDay) const {
   if (_month == 2 && leapYear(_year)) {
      return testDay == 29; // nel caso in cui fosse un anno bisestile, Febbraio
   }
   else {
      return testDay == _days[_month];
   }
}

// controlla l'incremento
void Date::helpIncrement() {
   // Se il giorno non è la fine del mese
   if (!endOfMonth(_day)) {
      ++_day; // incremento il giorno
   }
   else {
      if (_month < 12) { // Se il giorno è l'ultimo del mese e il mese < 12
         ++_month; // incremento il mese
         _day = 1; // setto il giorno al primo del mese
      }
      else { // Se il mese è il 12(Dicembre)
         ++_year; // incemento l'anno
         _month = 1; // setto il mese al primo dell'anno
         _day = 1; // setto il giorno al primo del mese
      }
   }
}

// overloaded output operator
unsigned int Date::getMonth() const {
    return _month;
}

unsigned int Date::getDay() const {
    return _day;
}


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

bool Date::operator>(const Date & date) const{
    if(_year>date._year){
        return true;
    }else if(_year==date._year&&_month>date._month){
        return true;
    }else if (_year==date._year&&_month==date._month&&_day>date._day){
        return true;
    }else{
        return false;
    }
}

bool Date::operator<(const Date & date) const {
    if(_year<date._year){
        return true;
    }else if(_year==date._year&&_month<date._month){
        return true;
    }else if (_year==date._year&&_month==date._month&&_day<date._day){
        return true;
    }else{
        return false;
    }
}

//prende il giorno della settimana
std::string Date::getWeekDay() {
    Date d(_year,_month,_day);
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
bool Date::checkGapGiven(int weeks, Date d){
    int numDays = weeks * 7; //a quanti giorni corrisponde il numero di settimane da considerare
    Date compare = this->add(numDays);//aggiungo alla data iniziale il numero di giorni  corrispondente a quelle settimane
    return d.isEqual(compare);
}

///restituisce la data corrispondente ad un periodo(in giorni) dalla data iniziale
Date Date::add(int daysToAdd) {
    Date toReturn(_year,_month,_day);
    for(int i=0; i<daysToAdd; i++){
        toReturn.helpIncrement();
    }
    return toReturn;
}

unsigned int Date::getYear() const {
    return _year;
}

///ritorna la data sottofirma di stringa
std::string Date::toString() {
    std::stringstream ss;
    ss << Date(_year,_month,_day);
    return ss.str();
}

bool Date::isEqual(const Date compare) {
    return (_year == compare._year && _month == compare._month && _day == compare._day); //true o false
}


///gap tra due date (start session e date dove quest'ultima è la data da controllare)
int Date::whatIsTheGap(Date& date) {
    bool areNotEqual = true;
    Date currentDate(_year,_month,_day);//data di inizio della sessione
    int gap = 0;
    while(areNotEqual){
        if(currentDate.isEqual(date)){//se uguali ho raggiunto la data finale del gap che volevo conoscere
            areNotEqual = false;
        }
        else{//se non uguali controllo il giorno successivo e aggiorno il gap
            ++currentDate;
            gap++;
        }
    }
    return gap;
}

bool Date::operator<=(const Date & d) const {
    if(*this < d || *this == d){
        return true;
    }
    else
        return false;
}

bool Date::operator>=(const Date & d) const {
    if(*this < d){
        return false;
    }
    else
        return true;
}

bool Date::operator==(const Date& compare) const {
    return (_year == compare._year && _month == compare._month && _day == compare._day); //true or false
}

Date Date::subtract(int daysToSubtract) {
    Date toReturn(_year,_month,_day);
    for(int i=0; i<daysToSubtract; i++){
        toReturn.helpDecrement();
    }
    return toReturn;
}
//postfix Date--
Date Date::operator--(int) {
    Date temp(_year,_month,_day);
    temp.helpDecrement();
    return temp;
}

void Date::helpDecrement() {
    // non è il primo giorno del mese
    if (_day>1) {
        --_day; //decremento il giorno
    }
    else { // se il giorno è il primo del mese
        if (_month > 1) { // ma il mese non è il primo dell'anno
            --_month; // decremento il mese
            if(_month==2 && leapYear(_year)){
                _day = 29;
            }
            _day = _days[_month]; // giorno = ultimo giorno del mese
        }
        else { // se giorno=1 e mese=1 devo decrementare l'anno
            --_year;
            _month = 12; //mese = 12 (dicembre)
            _day = 31; // giorno = 31 (ultimo giorno del mese)
        }
    }
}

//prefix --Date
Date &Date::operator--() {
    helpDecrement();
    return *this;
}
