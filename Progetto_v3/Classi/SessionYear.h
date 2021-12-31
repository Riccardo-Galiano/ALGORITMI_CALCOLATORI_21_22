//
// Created by lucam on 04/12/2021.
//

#ifndef PROGETTO_V3_SESSIONYEAR_H
#define PROGETTO_V3_SESSIONYEAR_H


#include <map>
#include "Date.h"
#include "ExamDay.h"
#include "StudyCourse.h"
#include "SystemLog.h"

typedef struct {
    std::string season;
    Date startDate;
    Date endDate;
} session;

class SessionYear {

public:
    SessionYear(std::string&, std::string&,std::string&,std::string&,std::string&);
    bool addSession(std::string&, std::string&,  std::string&);
    int isPossibleToAssignThisExam(Course ,Date,std::map<int, Professor>&,std::map<int, Classroom>&,int, int, int,std::vector<int>&);
    bool setCaldendar(std::vector<Date>);
    bool sessionsPeriodIsEmpty();
    //ritorna true se è stato possibile generare tutta la sessione, false altrimenti
    bool generateNewYearSession(std::string& ,std::map<std::string, Course>&,std::map<int, Professor>&, std::map<int, Classroom>& allUniversityClassrooms, int);
    int getAcYear() const;
    std::string getSessions() const;
    std::vector<std::string> getAllExamAppealsToDo(std::string, std::map<std::string, Course>&);


private:
    SystemLog _sysLog;
    int _acYear;
    //key: "autumn" , "winter", "summer"
    //value: session struct
    std::map<std::string,session> _yearSessions;
    std::vector<std::string> _sessionNames;
    std::vector<std::string> _allExamAppealsToDo;
    //key: data come stringa "aaaa-mm-gg"
    //value: oggetto Exam Day --> controllo: non ci deve essere nessuna domenica
    std::map<std::string,ExamDay> _yearCalendar;
    //param=> 0: invernale, 1: estiva, 2: autunnale
    //ritorna true se è stato possibile, false altrimenti
    bool generateThisSession(std::string, std::map<std::string, Course>&,std::map<int, Professor>&, std::map<int, Classroom>& allUniversityClassrooms, int, int);
    bool dateIsOK(Date& newDate,const Course& course, std::string& sessName, int);
    int getSemester(std::string);
    int checkIfProfsAvailableAndGapSameSemesterCourses(Course&,Date&,std::map<int, Professor>&,std::map<int, Classroom>&, int,int,std::vector<int>&);
    void assignTheExamToThisExamDay(int,Date&,std::map<int, Professor>&, std::map<int, Classroom>& allUniversityClassrooms, Course& , std::string, std::vector<std::string>&, std::vector<int> rooms);
    void generateOutputFiles(std::string&,int,std::map<std::string, Course>&);
    static void popAppealFromVector(std::vector<std::string>&,std::string);
    static bool checkHours(std::vector<int>&);
    std::vector<std::string> getGroupedCourses(const std::map<std::string, Course>&, std::string);
  };

std::ostream &operator<<(std::ostream &stud, const SessionYear &s);


#endif //PROGETTO_V3_SESSIONYEAR_H
