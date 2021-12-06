//
// Created by lucam on 04/12/2021.
//

#include "SessionYear.h"
#include "Parse.hpp"

///costruttore
SessionYear::SessionYear(std::string acYear, std::string winterSession,std::string summerSession,std::string autumnSession) {
    _acYear = Parse::getAcStartYear(acYear);
    this->addSession(acYear,winterSession,"winter");
    this->addSession(acYear,summerSession,"summer");
    this->addSession(acYear,autumnSession,"autumn");

}

///aggiunge il periodo delle sessioni
bool SessionYear::addSession(std::string acYear,std::string sessionDates, std::string name) {
    _acYear = Parse::getAcStartYear(acYear);
    std::vector<Date> dates = Parse::getDates(sessionDates);//vettore di Date in cui metterò la data di inizio e di fine
    if(name == "autumn") {
        if (!dates[0].checkGapGiven(4, dates[1]))//controllo se il periodo di settimane per la sessione autunnale sia esatto
            throw std::invalid_argument("durata sessione autunnale diversa da 4 settimane");
    }
    else if (!dates[0].checkGapGiven(6, dates[1])) {//controllo se il periodo di settimane per le sessione invernale/estiva sia esatto
        if (name == "summer")
            throw std::invalid_argument("durata sessione estiva diversa da 6 settimane");
        else
            throw  std::invalid_argument("durata sessione invernale diversa da 6 settimana");
    }

    session s{name,dates[0],dates[1]};//salvo le info della sessione nome, data di inizio e data di fine

    _yearSessions.insert(std::pair<std::string,session>(name,s));//sessione Invernale, Estiva, Autunnale. Salvo la sessione
    return true;
}

///prende l'anno
int SessionYear::getAcYear() const {
    return _acYear;
}

///prende i periodi delle sessioni e li compatta in una stringa
std::string SessionYear::getSessions() const {
    std::stringstream output;
    int i = 0;
    ///parto dalla ine perchè la mappa organizza in ordine alfabetico quindi avremo nella struttura rispettivamente: autunno, estate e inverno
    /// ma il formato vuole prima l'inverno, poi l'estate e poi l'autunno
    for(auto iterSession = _yearSessions.rbegin(); iterSession != _yearSessions.rend(); iterSession++) { //per ogni sessione dell'anno
        output << iterSession->second.startDate <<"_"<<iterSession->second.endDate;//aaaa-mm-gg_aaaa-mm-gg
        if(i <_yearSessions.size()-1)//l'ultimo periodo non avrà il ";"
            output<<";";
        i++;
    }

    return output.str() ;
}

std::ostream &operator<<(std::ostream &sessions, const SessionYear &s){
    sessions << s.getAcYear()<<"-"<<s.getAcYear()+1 <<";"<< s.getSessions();//aaaa-aaaa ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg
    return sessions;
}