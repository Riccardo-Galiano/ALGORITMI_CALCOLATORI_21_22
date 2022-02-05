
#include <iostream>
#include <string>
#include <iomanip>
#include "Date.h"
#include "cmath"



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
    std::vector<int> days{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (yy >= 1900 && yy <= 2100) {
        _year = yy;
    }
    else {
        throw std::invalid_argument{"l'anno deve essere >= 1900 e <= 2100\n"};
    }

    if (mm >= 1 && mm <= 12) {
        //se il mese è tra 1 e 12 va bene, altrimenti esco
        _month = mm;
   }
   else {
      throw std::invalid_argument{"Il mese deve essere tra 1-12\n"};
   }

   // test se anno bisestile
   if ((_month == 2 && leapYear(_year) && dd >= 1 && dd <= 29) || (dd >= 1 && dd <= days[_month])) {
       ///il giorno è ok se:
       //nel caso di Febbraio e anno bisestile il giorno deve essere compreso tra 1 e 29
       //oppure quando il giorno è compreso tra 1 e il max dei giorni del mese scelto (il cui numero ce l'abbiamo in days)
       _day = dd;
   }
   else if( dd >31) {
      throw std::invalid_argument("Il giorno non puo' essere maggiore di 31\n");
   }else{
      throw std::invalid_argument{"Per questo anno e questo mese il giorno scelto non va bene\n"};
   }
}

// overloaded prefix increment operator
Date& Date::operator++() {
   helpIncrement(); //incremento controllato della data
   return *this;
}

// overloaded postfix increment operator
Date Date::operator++(int) {
    //salvo la data prima di incrementarla
   Date temp(_year,_month,_day);
   temp.helpIncrement();
    //incremento la data temporanea
    return temp;
}


// controllo se un anno bisestile
bool Date::leapYear(int testYear) {
   return (testYear % 400 == 0 ||  (testYear % 100 != 0 && testYear % 4 == 0));
}

// determina la fine di un mese
bool Date::endOfMonth(int testDay) const {
    std::vector<int> days{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   if (_month == 2 && leapYear(_year)) {
       //se il mese è Febbraio e l'anno è bisestile
       return testDay == 29;
   }
   else {
      return testDay == days[_month];
   }
}

// Incremento controllato
void Date::helpIncrement() {
    // Se il giorno non è la fine del mese incremento il giorno
   if (!endOfMonth(_day)) {
      ++_day;
   }
   else {
       if (_month < 12) {
           //se è l'ultimo del mese mi chiedo se è Dicembre
           ++_month; // incremento il mese
         _day = 1; // setto il giorno al primo del mese
      }
      else {
           // Se il mese è il 12(Dicembre) incremento l'anno,setto il mese al primo dell'anno,setto il giorno al primo del mese
           ++_year; // incemento l'anno
         _month = 1; // setto il mese al primo dell'anno
         _day = 1; // setto il giorno al primo del mese
      }
   }
}

// overloaded output operator
int Date::getMonth() const {
    return _month;
}

int Date::getDay() const {
    return _day;
}


std::ostream& operator<<(std::ostream& output, const Date& d){

    output <<std::setfill('0') << d.getYear() << "-" << std::setw(2) << d.getMonth()<< "-" << std::setw(2) << d.getDay();
    return output;
}

// overloaded input operator
std::istream& operator>>(std::istream& input, Date& d){
    unsigned int month;
    unsigned int day;
    unsigned int year;
    input >> year >> month >> day;
    d.setDate(year, month, day);
    return input;
}

//overload comparatore di maggioranza
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

//overload comparatore di minoranza
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
    //creo il giorno
    Date d(_year,_month,_day);
    int month;
    if (d.getMonth() > 2)
        //da Marzo a Dicembre
        month = d.getMonth();
    else {
        month = (12 + d.getMonth()); //for Jan and Feb, month code will be 13 and 14
        d._year--; //decrease year for month Jan and Feb
    }
    std::string year = std::to_string(getYear());
    int y = std::stoi(year.substr(2,2));
    int c = std::stoi(year.substr(0,2));
    int w = (d.getDay() + floor((13 * (month + 1)) / 5) + y + floor(y / 4) + floor(c / 4) + (5 * c));
    w = w % 7;
    return _weekday[w];
}

///controlla se le sessioni si svolgono nelle settimane richieste
bool Date::checkGapGiven(int weeks, Date d){
    int numDays = weeks * 7; //a quanti giorni corrisponde il numero di settimane da considerare
    Date compare = this->add(numDays);//aggiungo alla data iniziale il numero di giorni  corrispondente a quelle settimane
    return d == compare;
}

///restituisce la data corrispondente ad un periodo(in giorni) dalla data iniziale
Date Date::add(int daysToAdd) {
    Date toReturn(_year,_month,_day);
    for(int i=0; i<daysToAdd; i++){
        toReturn.helpIncrement();
    }
    return toReturn;
}

int Date::getYear() const {
    return _year;
}

///ritorna la data sottofirma di stringa
std::string Date::toString() {
    std::stringstream ss;
    ss << Date(_year,_month,_day);
    return ss.str();
}

///gap tra due date (date è la data da controllare rispetto a quella per cui il metodo è stato chiamato)
int Date::whatIsTheGap(Date& date) {
    bool areNotEqual = true;
    Date currentDate(_year,_month,_day);//data di inizio della sessione
    int gap = 0;
    while(areNotEqual){
        if(currentDate==date){//se uguali ho raggiunto la data finale del gap che volevo conoscere
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
    std::vector<int> days{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (_day>1) {
        --_day; //decremento il giorno
    }
    else { // se il giorno è il primo del mese
        if (_month > 1) { // ma il mese non è il primo dell'anno
            --_month; // decremento il mese
            if(_month==2 && leapYear(_year)){
                _day = 29;
            }
            _day = days[_month]; // giorno = ultimo giorno del mese
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
