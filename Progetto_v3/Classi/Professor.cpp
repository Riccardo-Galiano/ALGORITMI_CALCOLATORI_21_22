//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include <vector>
#include "Professor.h"
#include "Parse.hpp"

Professor::Professor(int matr, std::string name, std::string surname, std::string email) : UniversityMember(matr, name,
                                                                                                            surname,
                                                                                                            email) {
}


Professor::Professor(int matr) : UniversityMember() {
    _id = matr;
}

///setta uno dei periodi di indisponibilità del prof analizzato
bool Professor::setNoAvaibilities(int acYear, std::string &input) {
    std::vector<std::string> dates;
    dates = Parse::splittedLine(input, '|');//inizio periodo | fine periodo
    Date d1 = Date(dates[0]); //creo un oggetto Date passando la stringa dell'intera data(vedere costructor in Date)
    Date d2 = Date(dates[1]);
    std::vector<std::pair<Date, Date>> vect;//vettore di tuple in cui mettere data di inizio e fine del periodo di indisponibilità
    if (_noAvailab.first != acYear) {//se non abbiamo ancora un anno accademico o è diverso da quello nuovo
        vect.push_back(std::pair<Date, Date>(d1, d2));
        _noAvailab = (std::pair<int, std::vector<std::pair<Date, Date>>>(acYear,
                                                                         vect));//assegno la prima tupla di Date all'anno accademico
    } else

        _noAvailab.second.emplace_back(d1,
                                       d2);//_nonAvaib.second.push_back(std::pair(d1,d2))così non me la fa inserire!!


    return true;
}

///stringa destinata alla scrittura del file in cui ci sono le indisponibilità del prof
std::string Professor::outputNoAvailabilities(int nMatr) {

    std::stringstream output;
    int i = 0;
    output << _noAvailab.first << "-" << _noAvailab.first + 1 << ";" << "d" << setId(nMatr) << ";";
    for (auto iterDate = _noAvailab.second.begin(); iterDate != _noAvailab.second.end(); iterDate++) {
        output << iterDate->first << "|" << iterDate->second;
        if (i < _noAvailab.second.size() - 1)
            output << ";";
        i++;
    }

    return output.str();
}

///restituisce true se _nonAvaib è vuoto
bool Professor::noAvailabilityPeriodIsEmpty() {
    return _noAvailab.second.empty();
}

///pulisce la tupla _nonAvaib
void Professor::noAvailabilityClear() {
    _noAvailab.second.clear();
}

///l'anno corrispondente alle indisponibilità
int Professor::getNoAvalaibilityYear() const {
    return _noAvailab.first;
}

bool Professor::addNewExam(std::string date, int hh, std::string cod_exam) {
    if (_examsToDo.count(date) == 0){
        //mappa di esami in quel giorno ancora non esistente
        std::map<int,std::string> examsPerDay;
        examsPerDay.insert(std::pair<int,std::string>(hh,cod_exam));
        _examsToDo.insert(std::pair<std::string,std::map<int,std::string>>(date,examsPerDay));
    }
    else{
        //mappa di esami in quel giorno esiste già
        _examsToDo.at(date).insert(std::pair<int,std::string>(hh,cod_exam));
    }
    return true;
}

bool Professor::amIavailable(std::string date, int hh) {
    if(_examsToDo.count(date) == 0){
        return true;
    }
    else{
        //il docente ha esami in quel giorno...
        //ma ha quello slot libero??
        if(_examsToDo.at(date).count(hh)==0)
            return true;
    }
    return false;
}

///overload operatore <<
std::ostream &operator<<(std::ostream &prof, const Professor &s) {

    int Id = s.getId();
    prof << "d" << s.setId(Id) << ";" << s.getName() << ";" << s.getSurname() << ";" << s.getEmail();
    return prof;
}