//
// Created by lucam on 04/12/2021.
//

#ifndef PROGETTO_V3_SESSIONYEAR_H
#define PROGETTO_V3_SESSIONYEAR_H


#include <map>
#include "Date.h"
#include "ExamDay.h"

typedef struct {
    std::string season;
    Date startDate;
    Date endDate;
} session;

class SessionYear {

public:
    SessionYear(std::string, std::string,std::string,std::string);
    bool addSession(std::string, std::string,  std::string);
    int getAcYear() const;
    std::string getSessions() const;

private:
    int _acYear;
    //key: "autumn" , "winter", "summer"
    //value: session struct
    std::map<std::string,session> _yearSessions;
    //key: data come stringa "aaaa-mm-gg"
    //value: oggetto Exam Day
    std::map<std::string,ExamDay> _yearCalendar;

};
std::ostream &operator<<(std::ostream &stud, const SessionYear &s);


#endif //PROGETTO_V3_SESSIONYEAR_H
