//
// Created by Andrea on 03/12/2021.
//

#include "SessionYear.hpp"
#include "Parse.hpp"

///aggiunge il periodo delle sessioni
bool SessionYear::addSession(std::string acYear,std::string sessionDates, std::string name) {
    _acYear = Parse::getAcStartYear(acYear);
    std::vector<Date> dates = Parse::getDates(sessionDates);//vettore di Date in cui metterò la data di inizio e di fine
    if(name == "autumnSession") {
        if (!dates[0].checkGapGiven(4, dates[1]))//controllo se il periodo di settimane per la sessione autunnale sia esatto
            throw std::invalid_argument("durata sessione autunnale diversa da 4 settimane");
    }
    else if (!dates[0].checkGapGiven(6, dates[1])) {//controllo se il periodo di settimane per le sessione invernale/estiva sia esatto
            if (name == "summerSession")
                throw std::invalid_argument("durata sessione estiva diversa da 6 settimane");
            else
                throw  std::invalid_argument("durata sessione invernale diversa da 6 settimana");
        }


    session s{name,dates[0],dates[1]};//salvo le info della sessione nome, data di inizio e data di fine

    _yearSessions.insert(std::pair<std::string,session>(name,s));//sessione Invernale, Estiva, Autunnale. Salvo la sessione
    return true;
}

bool SessionYear::clearYearSession() {
    _yearSessions.clear();
    return true;
}

int SessionYear::getAcYear() const {
    return _acYear;
}

std::string SessionYear::getSessions() const {
    std::stringstream output;
    int i = 0;
    for(auto iterSession = _yearSessions.rbegin(); iterSession != _yearSessions.rend(); iterSession++) {
        output << iterSession->second.startDate <<"_"<<iterSession->second.endDate;
        if(i <_yearSessions.size()-1)
            output<<";";
        i++;
    }

    return output.str() ;
}

std::ostream &operator<<(std::ostream &sessions, const SessionYear &s){
        sessions << s.getAcYear()<<"-"<<s.getAcYear()+1 <<";"<< s.getSessions();
    return sessions;
}



