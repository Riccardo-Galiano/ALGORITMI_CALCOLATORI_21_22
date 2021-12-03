//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "Professor.h"
#include "Parse.hpp"

Professor::Professor(int matr, std::string name, std::string surname, std::string email): UniversityMember(matr, name, surname, email) {
}


Professor::Professor(int matr) : UniversityMember() {
    _id = matr;
}

bool Professor::setAvaibilities(int acYear, std::string &input) {
    std::vector<std::string> dates;
    dates = Parse::splittedLine(input,'|');
    Date d1 = Date(dates[0]);
    Date d2 = Date(dates[1]);
    //controllo su vettore (value)
    if(_nonAvaib.count(acYear)==0) {
        std::vector<std::pair<Date, Date>> vect;
        vect.push_back(std::pair<Date, Date>(d1, d2));
        _nonAvaib.insert(std::pair<int,std::vector<std::pair<Date,Date>>>(acYear,vect));
    }
    else{
        _nonAvaib.at(acYear).push_back(std::pair<Date, Date>(d1, d2));
    }
    return true;
}

///overload operatore <<
std::ostream &operator<<(std::ostream &prof, const Professor &s){

    int Id = s.getId();
    prof<< "d" << s.setId(Id)<< ";"<< s.getName()<<";"<<s.getSurname()<<";"<<s.getEmail();
    return prof;
}