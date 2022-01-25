//
// Created by lucam on 04/12/2021.
//

#ifndef PROGETTO_V3_SESSIONYEAR_H
#define PROGETTO_V3_SESSIONYEAR_H

#include <map>
#include "Date.h"
#include "ExamDay.h"
#include "StudyCourse.h"
#include "SessionLog.h"

class University; //forward reference usata per risolvere circular dependency

typedef struct {
    std::string season;
    Date startDate;
    Date endDate;
} session;

class SessionYear {

public:
    ///constructor
    SessionYear(std::string& acYear, std::string& winterSession,std::string& summerSession,std::string& autumnSession,std::string output_file_name = "output_file_name");
    ///SessionYear management
    bool addSession(std::string& acYear, std::string& sessionDates,  std::string& name);
    bool setCaldendar(std::vector<Date> dates);
    //ritorna true se è stato possibile generare tutta la sessione, false altrimenti
    bool generateNewYearSession(std::string &fout, int relaxPar, University &myUniversity);
    bool addProfGap(std::string& matr_idC, int gap);
    //param=> 0: invernale, 1: estiva, 2: autunnale
    //ritorna true se è stato possibile, false altrimenti
    bool generateThisSession(std::string sessName, std::map<std::string, Course> &courses,
                             std::map<int, Professor> &profs,
                             std::map<int, Classroom> &allUniversityClassrooms, int relaxPar, int gapAppeals,
                             bool sixHours, bool tryToSatisfyProfsMinDistance);
    void assignTheExamToThisExamDay(int startExamHour, Date &currentExamDay, std::map<int, Professor> &profs,
                                    std::map<int, Classroom> &allUniversityClassrooms, Course &course,
                                    std::string sessName, std::vector<std::string> &allExamAppealsToDo,
                                    std::vector<int> idRooms, bool requestChanges, int numAppealYear);
    static void popAppealFromVector(std::vector<std::string>& allExamAppealsToDo,std::string codExam);
    std::vector<std::string> getProfsOfGapProfsString();
    void assignAppealsToCalendar(std::string appeal, int startSlotHour, Course& course, int numSlots);
    void removeThisAppealInfoFromCalendar(int numSlots,Date& date, int& startSlot, std::string& idCourse);
    bool tryToSetThisExamInThisSession(University& myUniversity, Course& courseToConsider, int numSession, int numAppeal, Date& tryDate);

    ///getter
    int getAcYear() const;
    std::string getSessions() const;
    std::vector<std::string> getAllExamAppealsToDo(std::string sessName, std::map<std::string, Course>& courses);
    int getSemester(std::string sessName);
    std::vector<std::string> getGroupedCourses(const std::map<std::string, Course>& courses, std::string idCourseSelected);
    std::string getFileName(int numSession);
    void setFileNamePerSession(int numSession,std::string fileName);

    ///control
    int isPossibleToAssignThisExam(Course course,Date,std::map<int, Professor>&,std::map<int, Classroom>&,int, int, int,std::vector<int>&, int endHour,bool firstCourseOfThisLoop,int startControlExamHourSlot,bool requestChanges);
    bool sessionsPeriodIsEmpty();
    bool dateIsOK(Date &newDate, const Course &course, std::string &sessName, int gapAppeals, bool requestChanges,
                  bool tryToSatisfyProfsMinDistance);
    int checkIfProfsAvailableAndGapSameSemesterCourses(Course& course,Date& currentExamDay,std::map<int, Professor>& profs,std::map<int, Classroom>& classrooms, int relaxPar,int session,std::vector<int>& roomsFounded, int endHourSlot,bool firstCourseOfThisLoop,int startControlExamHourSlot,bool requestChanges);
    static bool checkHours(std::vector<int>& input);
    bool isSecondAppeal(Date newDate, Date lastDateAssignation);
    void updateExamDayCourse(Course course,std::vector<Date> allAppealPerCourse);
    void controlSuccessivitySessionPeriod();

    ///output
    void allExamAppealsWrite(std::map<std::string, Course>& courses);
    void generateOutputFilesSession(std::string& outputFileName, int session, const std::map<std::string, Course>& courses, bool requestChanges);
    bool fileNameIsEmpty();
    std::vector<std::string>getSessionAndFileName();
private:
    SessionLog _sysLog;
    int _acYear;
    //key: "autumn" , "winter", "summer"
    //value: session struct
    std::map<std::string,session> _yearSessions;
    std::vector<std::string> _sessionNames;
    std::vector<std::string> _allExamAppealsToDo;
    //key: data come stringa "aaaa-mm-gg"
    //value: oggetto Exam Day --> controllo: non ci deve essere nessuna domenica
    std::map<std::string,ExamDay> _yearCalendar;

    ///gap stuff
    int _gapAppealsSameCourse = 14;
    //key: data come stringa "matricola-idCorso"
    //value: gap
    std::map<std::string,int> _gapProfs;
    //vector per segnalare i prof dei quali non viene rispetatto il gap richiesto
    std::vector<std::pair<std::string,int>> _gapProfsNoRespect;
    std::vector<std::string> _errorStringSessionYear;
    //key: session : 1
    //value: nome del file : exam_2021_s1.txt
    std::map<int,std::string> _fileNamePerAcSession;
};

std::ostream &operator<<(std::ostream &stud, const SessionYear &s);


#endif //PROGETTO_V3_SESSIONYEAR_H
