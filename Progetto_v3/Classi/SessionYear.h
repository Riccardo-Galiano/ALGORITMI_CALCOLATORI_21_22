//
// Created by lucam on 04/12/2021.
//

#ifndef PROGETTO_V3_SESSIONYEAR_H
#define PROGETTO_V3_SESSIONYEAR_H


#include <map>
#include "Date.h"
#include "ExamDay.h"
#include "StudyCourse.h"

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
    //ritorna true se è stato possibile generare tutta la sessione, false altrimenti
    bool generateNewYearSession(std::string& ,const std::map<std::string, Course>&,std::map<int, Professor>&,const std::map<int, StudyCourse>&);
    std::vector<std::string> getAllExamAppealsToDo(std::string, const std::map<std::string, Course>&);
    int isPossibleToAssignThisExam(Course ,Date,std::map<int, Professor>&,const std::map<int, StudyCourse>&,int numSlot);
    bool sameYearAndStudyCourse();
private:
    int _acYear;
    //key: "autumn" , "winter", "summer"
    //value: session struct
    std::map<std::string,session> _yearSessions;
    //key: data come stringa "aaaa-mm-gg"
    //value: oggetto Exam Day --> controllo: non ci deve essere nessuna domenica
    std::map<std::string,ExamDay> _yearCalendar;
    //param=> 0: invernale, 1: estiva, 2: autunnale
    //ritorna true se è stato possibile, false altrimenti
    bool generateThisSession(std::string,const std::map<std::string, Course>&,std::map<int, Professor>&,const std::map<int, StudyCourse>&);
    int getSemester(std::string);

};
std::ostream &operator<<(std::ostream &stud, const SessionYear &s);


#endif //PROGETTO_V3_SESSIONYEAR_H
