//
// Created by Andrea on 03/12/2021.
//

#ifndef PROGETTO_V3_SESSIONYEAR_HPP
#define PROGETTO_V3_SESSIONYEAR_HPP

#include <map>
#include "Date.h"

typedef struct {
    std::string season;
    Date startDate;
    Date endDate;
} session;

class SessionYear {
private:
    int _acYear;
    std::map<std::string,session> _yearSessions;
public:
    bool addSession(std::string, std::string,  std::string);
};


#endif //PROGETTO_V3_SESSIONYEAR_HPP
