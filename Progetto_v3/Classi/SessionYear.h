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
    bool generateNewYearSession(std::string& ,std::map<std::string, Course>&,std::map<int, Professor>&);
    std::vector<std::string> getAllExamAppealsToDo(std::string, std::map<std::string, Course>&);
    int isPossibleToAssignThisExam(Course ,Date,std::map<int, Professor>&,int numSlot);
    bool setCaldendar(std::vector<Date>);
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
    bool generateThisSession(std::string, std::map<std::string, Course>&,std::map<int, Professor>&);
    bool dateIsOK(Date& newDate, SpecificYearCourse& sp, std::string& sessName);
    int getSemester(std::string);
    int checkIfProfsAvailableAndGapSameSemesterCourses(Course&,Date&,std::map<int, Professor>&);
    void assignTheExamToThisExamDay(int,Date&,std::map<int, Professor>&, Course& , int);
    void generateOutputFiles(std::string&,int);
};
std::ostream &operator<<(std::ostream &stud, const SessionYear &s);


#endif //PROGETTO_V3_SESSIONYEAR_H
