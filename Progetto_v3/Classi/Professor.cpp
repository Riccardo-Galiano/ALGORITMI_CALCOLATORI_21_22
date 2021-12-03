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

///setta uno dei periodi di indisponibilità del prof analizzato
bool Professor::setAvaibilities(int acYear, std::string &input) {
    std::vector<std::string> dates;
    dates = Parse::splittedLine(input,'|');//inizio periodo | fine periodo
    Date d1 = Date(dates[0]); //creo un oggetto Date passando la stringa dell'intera data(vedere costructor in Date)
    Date d2 = Date(dates[1]);

    //controllo su vettore (value)
    if(_nonAvaib.count(acYear)==0) {//se non c'è ancora l'anno accademico a cui sto facendo riferimento
        std::vector<std::pair<Date, Date>> vect;//tupla di vettori in cui mettere data di inizio e fine del periodo di indisponibilità
        vect.push_back(std::pair<Date, Date>(d1, d2));
        _nonAvaib.insert(std::pair<int,std::vector<std::pair<Date,Date>>>(acYear,vect));//metto in una mappa per anno accademico il vettore di tuple
    }
    else{
        _nonAvaib.at(acYear).push_back(std::pair<Date, Date>(d1, d2));//se già esiste l'anno accademico devo solo aggiungere il periodo di indisponibilità al vettore di tuple corrispondende all'acYear
    }
    return true;
}

///overload operatore <<
std::ostream &operator<<(std::ostream &prof, const Professor &s){

    int Id = s.getId();
    prof<< "d" << s.setId(Id)<< ";"<< s.getName()<<";"<<s.getSurname()<<";"<<s.getEmail();
    return prof;
}