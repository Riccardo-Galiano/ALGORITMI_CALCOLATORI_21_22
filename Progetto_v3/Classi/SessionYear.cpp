//
// Created by Andrea on 03/12/2021.
//

#include "SessionYear.hpp"
#include "Parse.hpp"

bool SessionYear::addSession(std::string acYear,std::string sessionDates, std::string name) {
    _acYear = Parse::getAcStartYear(acYear);
    std::vector<Date> dates = Parse::getDates(sessionDates);
    if(name == "autumnSession") {
        if (!dates[0].checkGapGiven(4, dates[1]))
            throw std::invalid_argument("durata sessione autunnale diversa da 4 settimane");
    }
    else{
        if (!dates[0].checkGapGiven(6, dates[1]))
            throw std::invalid_argument("durata sessione diversa da 6 settimane");
    }

    session s{name,dates[0],dates[1]};
    _yearSessions.insert(std::pair<std::string,session>(name,s));
    return false;
}
