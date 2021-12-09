//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include <vector>
#include "Professor.h"
#include "Parse.hpp"

///costruttori
Professor::Professor(int matr, std::string name, std::string surname, std::string email) : UniversityMember(matr, name,surname, email) {
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
    if(_noAvailab.count(acYear)==0) {//se non c'è ancora l'anno accademico a cui sto facendo riferimento
        vect.push_back(std::pair<Date, Date>(d1, d2));
        _noAvailab.insert(std::pair<int,std::vector<std::pair<Date,Date>>>(acYear,vect));//metto in una mappa per anno accademico il vettore di tuple
    }
    else{
        _noAvailab.at(acYear).push_back(std::pair<Date, Date>(d1, d2));//se già esiste l'anno accademico devo solo aggiungere il periodo di indisponibilità al vettore di tuple corrispondende all'acYear
    }

    return true;
}

///stringa destinata alla scrittura del file in cui ci sono le indisponibilità del prof
std::vector<std::string> Professor::outputNoAvailabilities(int nMatr) {
   std::vector<std::string> allProfNoAvailabilities;
        for(auto iterNoAvailab = _noAvailab.begin(); iterNoAvailab != _noAvailab.end();iterNoAvailab++) {
            int i = 0;
            std::stringstream output;
            output << iterNoAvailab->first << "-" << iterNoAvailab->first + 1 << ";" << "d" << setId(nMatr) << ";";//aaaa-aaaa ; dxxxxxx ;
                for (auto iterDate = iterNoAvailab->second.begin(); iterDate != iterNoAvailab->second.end(); iterDate++) {//per tutti i periodi di indisponibilità
                    output << iterDate->first << "|" << iterDate->second; //aaaa-mm-gg|aaaa-mm-gg
                    if (i < iterNoAvailab->second.size() - 1)//se è l'ultimo periodo non va messo il punto e virgola
                        output << ";";
                    std::string prova = output.str();//lo stream diventa una stringa
                    i++;//per capire se va messo il punto e virgola
                }
        std::string tokens = output.str();
        allProfNoAvailabilities.push_back(tokens);
}
    return allProfNoAvailabilities ;
}

///pulisce la map ad un preciso anno di _nonAvaib
void Professor::noAvailabilityClear(int year) {
    _noAvailab.erase(year);
}

///aggiunge un esame alla mappa degli esami che un professore deve preparare
bool Professor::addNewExam(std::string date, int hh, std::string cod_exam) {
    if (_examsToDo.count(date) == 0){
        //mappa di esami in quel giorno ancora non esistente
        std::map<int,std::string> examsPerDay;//mappa che contiene ad un particolare slot un esame
        examsPerDay.insert(std::pair<int,std::string>(hh,cod_exam));
        _examsToDo.insert(std::pair<std::string,std::map<int,std::string>>(date,examsPerDay));
    }
    else{
        //mappa di esami in quel giorno esiste già
        _examsToDo.at(date).insert(std::pair<int,std::string>(hh,cod_exam));//aggiunge in un particolar slot (hh) di una data (date) il codice dell'esame (cod_exam)
    }
    return true;
}

///ritorna se lo slot a quell'ora per una certa data è libero(aggiungere i controlli per i periodi di indisponibilità per ogni prof)
bool Professor::amIavailable(std::string date, int hh) {
    int year = Parse::getAcStartYear(date);
    Date d(date);

    if(_noAvailab.count(year) != 0) {//per quell'anno abbiamo delle indisponibilità del prof? se si controllo i periodi
        for (auto iterNoAvailab = _noAvailab.at(year).begin(); iterNoAvailab !=_noAvailab.at(year).end(); iterNoAvailab++ ){//controllo tutti i periodi di indisponibilità del prof
             if (d > iterNoAvailab->first && d < iterNoAvailab->second){//se la data appartiene ad un intervallo di indisponibilità allora non posso mettere quell'esame quel giorno
                 return false;
             }
        }
    }

    if (_examsToDo.count(date) == 0) {//il docente non ha esami in quel giorno
                return true;
            } else if (_examsToDo.at(date).count(hh) ==
                       0) {//il docente ha esami in quel giorno ma ha quello slot libero?
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