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

Professor::Professor(int matr, std::string name, std::string surname, std::string email, std::string birth, std::string registration, std::string address): UniversityMember(matr, name,surname, email,birth,
registration,
address) {

}


///setta uno dei periodi di indisponibilità del prof analizzato
bool Professor::setNoAvaibilities(int acYear, std::string &input) {
    std::vector<std::string> dates;
    dates = Parse::splittedLine(input, '|');//inizio periodo | fine periodo
    if(dates.size() != 2)
        throw std::invalid_argument("Errore formato del periodo ");
    Date startPeriodDate = Parse::controlItCanBeADate(dates[0]);
    Date endPeriodDate = Parse::controlItCanBeADate(dates[1]);
    //la data iniziale deve essere inferiore alla data finale
    if(startPeriodDate < endPeriodDate) {
        //dimostro che ci sia coerenza tra l'anno dato da comando e l'anno del periodo analizzato
        ///AAA SI ASSUME, SECONDO LA LEGISLAZIONE ITALIANA, CHE UN ANNO ACCADEMICO INIZI IL 01/11/aaaa E FINISCA IL 31/10/aaaB
        Date acStartDate = Date(acYear,11,01);
        Date acEndDate = Date(acYear+1,10,31);
        if (startPeriodDate > acStartDate && endPeriodDate < acEndDate) {

            std::vector<std::pair<Date, Date>> vect;//vettore di tuple in cui mettere data di inizio e fine del periodo di indisponibilità
            if (_noAvailab.count(acYear) == 0) {//se non c'è ancora l'anno accademico a cui sto facendo riferimento
                vect.push_back(std::pair<Date, Date>(startPeriodDate, endPeriodDate));
                _noAvailab.insert(std::pair<int, std::vector<std::pair<Date, Date>>>(acYear,
                                                                                     vect));//metto in una mappa per anno accademico il vettore di tuple
            } else {
                _noAvailab.at(acYear).push_back(std::pair<Date, Date>(startPeriodDate,
                                                                      endPeriodDate));//se già esiste l'anno accademico devo solo aggiungere il periodo di indisponibilità al vettore di tuple corrispondende all'acYear
            }
        }else{
            throw std::logic_error("Non c'e' coerenza tra l'anno accademico passato da comando e il periodo " );
        }
    } else
        throw std::logic_error("La data finale e' inferiore a quella iniziale nel periodo " );

    return true;
}

///stringa destinata alla scrittura del file in cui ci sono le indisponibilità del prof
std::vector<std::string> Professor::outputNoAvailabilities(int nMatr) {
   std::vector<std::string> allProfNoAvailabilities;
        for(auto iterNoAvailab = _noAvailab.begin(); iterNoAvailab != _noAvailab.end();iterNoAvailab++) {
            int i = 0;
            std::stringstream output;
            std::string settedId = Parse::setId('d',6,nMatr);
            output << iterNoAvailab->first << "-" << iterNoAvailab->first + 1 << ";" <<settedId << ";";//aaaa-aaaa ; dxxxxxx ;
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
bool Professor::amIavailable(std::string date, int hh, int relaxPar) {
    int year = Parse::getAcStartYear(date);
    Date d(date);
    /// controllo indisponibilità docenti compatibile con data esame
    if(_noAvailab.find(year - 1) != _noAvailab.end() && relaxPar < 3) {//per quell'anno abbiamo delle indisponibilità del prof? se si controllo i periodi
        for (auto iterNoAvailab = _noAvailab.at(year - 1).begin(); iterNoAvailab !=_noAvailab.at(year - 1).end(); iterNoAvailab++ ){
            //controllo tutti i periodi di indisponibilità del prof
            Date lower = iterNoAvailab->first;
            Date upper = iterNoAvailab->second;

            if (d >= lower && d <= upper){
                 //se la data appartiene ad un intervallo di indisponibilità allora
                 // non posso mettere quell'esame quel giorno
                 return false;
             }

        }
    }
    ///controllo in quel giorno che non si sovrappongano fasce esame
    if (_examsToDo.count(date) == 0) {//il docente non ha esami in quel giorno
                return true;
            }else if (_examsToDo.at(date).count(hh) == 0) {
                //il docente ha esami in quel giorno ma ha quello slot libero? se ci entra, allora no
                return true;
            }
        return false;

}

void Professor::eraseThisAppealFromProfs(Date& dateAppeal, int startSlot, int numSlots) {
    ///ciclare per numslots!!!!!!!!!!
    for(int i = 0; i<numSlots; i++){
        int slot = startSlot + 2*i;
        _examsToDo.at(dateAppeal.toString()).erase(slot);
    }

}


///overload operatore <<
std::ostream &operator<<(std::ostream &prof, const Professor &s) {
    std::string settedId = Parse::setId('d',6,s.getId());
    prof << settedId << ";" << s.getName() << ";" << s.getSurname() << ";" << s.getEmail();
    return prof;
}