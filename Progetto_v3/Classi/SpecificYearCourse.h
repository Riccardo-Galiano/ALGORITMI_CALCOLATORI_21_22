//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_SPECIFICYEARCOURSE_H
#define PROGETTO_V3_SPECIFICYEARCOURSE_H


#include <vector>
#include <map>
#include <memory>
#include "Professor.h"
#include "Student.h"
#include "Exam.h"


typedef struct {
    int prof_id;
    int hLez;///ore assegnate ad ogni prof
    int hExe;
    int hLab;
    bool mainProf; ///titolare del corso?
   } professor;

///per ogni oggetto studente(in cui avrò matricola, nome, cognome e email) verrà tenuta traccia
typedef struct {
    int _studId;
    int _startEnrolYear; ///inizio corso
    int _passYear; ///fine corso
    int _grade;   ///voto
    bool _passed;
    Date _appealPassed;
} student;

class SpecificYearCourse {
public:
    ///constructor
    SpecificYearCourse(std::string sY_eY ,bool active ,int nCrsiPar, std::vector<std::string> prof, std::vector<std::string> exam,std::vector<std::string> idGrouped, std::string  yy_semester, std::vector<int> studyCourse, int line_counter);

    ///SpecificYearCourse management
    void addStudent(int acYearRegistration, Student &stud);
    void addGradeToStudent(Student &stud, int passYear, int mark, std::string appealsDate, std::string idCourse);
    int amIAssignedAlreadyInThisSession(int session);
    Date lastDateAssignationInGivenSession(int session);
    Date& dateAssignationInGivenSession(int numSession, int numAppeal);
    int startSlotAssignationInGivenSession(int numSession, int numAppeal);
    std::vector<int> classroomsAssignedInGivenSession(int numSession, int numAppeal);
    void assignExamInThisSpecificYearCourse(Date examDay, int session);
    void assignYY_SemToAllYear(std::string& acYYoff, std::string& yy_semester);
    void addClassroomsToAppeal(int numAppeal, std::vector<int>& rooms);
    void assignAllStudsPassedExam(std::vector<std::pair<std::string, int>> allStudPassedExam, std::string appealDate);
    void assignAppeals(std::string session, std::vector<Date> appealPerSession, std::vector<int> startSlotPerAppeal, std::vector<std::string> classroomsPerAppeal);
    void addStartSlotToAppeal(int numAppeal, int startExamHour);
    void reassignAppeal(int numAppeal,int numSession,Date date,int startSlot,std::vector<int>classroomsPerCourse);

    ///remove
    void removeInfoThisAppealFromSpecificYear(int numSession, int numAppeal);
    void updateIdGroupedCourses(std::vector<std::string> grouped);

    ///getter
    int getStartYear() const;
    int getEndYear() const;
    bool getIsActive() const;
    int getNumParallelCourses() const;
    const std::map<int, std::vector<professor>> getProfsOfParallelCourses()const;
    const std::string getProfParString() const;
    std::vector<int> getAllProfMatr()const; //ritorna LE MATRICOLE dei prof, indistintamente rispetto al corso parallelo a cui sono assegnati
    const std::string getGroupedCoursesIdString() const;
    std::vector<professor> getProfsFromString (std::string profs, int line_counter)const;
    const std::vector<std::string> &getIdGroupedCourses() const;
    const std::string getExamString() const;
    const Exam &getExam() const;
    int getSemester() const;
    int getYearOfTheSemester() const;
    std::vector<int> getStudyCourseAssigned() const;
    int getTotStudentsEnrolled() const;
    std::string getAcYearOff() const;
    int getTotStudentsExam()const;
    std::vector<Date> getAllAppeals() const;
    std::vector<int> getRoomsAppeal() ;
    int getNumNextAppeal() const;
    const std::map<int, std::vector<Date>> &getHowManyTimesIAmAssignedInASession() const;
    std::map<int, student> getStudentsPassedInThisAppeal(Date dateAppeal)const;
    std::string getAppealsForAllSession()const;
    std::string getRoomsPerAppealsString(int numAppeals)const;
    int getStartHourAppeal(int numAppeals)const;
    char getPlaceExam()const;
    int getNumAppealFromNumSessNumAppealInSession(int numAppeal, int startExamHour)const;
    int getNumAppealsAssignedPerSession(int numSession)const;
    int getNumAppeal() const;
    std::map<int, student> getStudentsEnrolled() const;


    ///setter
    void setYear();
    void setProfMap(int numCorsiPar, std::vector<std::string> profsToSplit, int line_counter);
    void setNewYear(int newStartYear);
    void setAcYearOff(std::string yearOff);
    void assignGrouped (std::vector<std::string>& idGrouped, std::string& idCourse,std::string& thisCourse);
    void eraseNumAppeal();
    void updateStudyCourseAssigned(int studyCourseAssigned);

    ///control
    bool canIBeAssigneToFirstTwoWeekOfExamSession(int semesterGiven) const;
    bool notExamsAssigned();
    void setYySemester(const std::string &yySemester);
    bool studyCourseEmpty();


private:
    std::string _yy_semester;
    //unico per tutti i corsi!!!
    std::string _acYearOff;
    //anno accademico in cui il corso è stato spento
    std::vector<int> _studyCourseAssigned;
    int _startYear;   ///anno di inizio
    int _endYear;    ///anno di fine
    bool _active;    ///attivo o non attivo?
    int _parallelCourses;    ///numero di corsi in parallelo
    int _totStudentsEnrolled;
    int _totStudentsNotPassed;
    std::vector<std::string> _idGroupedCourses;
    Exam _exam;
    // per ogni corso in parallelo ho un vettore dei prof
    //key: numero versione parallela, quindi da 0 a ... n
    //value vettore delle struct prof
    std::map<int, std::vector<professor>> _professors;
    //key: student id
    //value: struct studente
    std::map<int, student> _studentsEnrolled;
    ///exam stuff
    //key: semester (session)
    //value: quante volte è stato programmato un suo esame in quel semestre
    std::map<int, std::vector<Date>> _howManyTimesIAmAssignedInASession;

    //key: num appello: 0, 1, 2, 3. (4 nell'intero anno)
    //value: vettore di aule per quel'appello
    std::map<int, std::vector<int>> _roomsEachAppeal;
    //key: num appello 0,1,2,3??? se non è cosi errore!!!
    //value startSlot
    std::map<int,int> _startSlotPerEachAppeal;
    int _numAppeal;


};

std::ostream& operator<<(std::ostream& output, const SpecificYearCourse& s);

#endif //PROGETTO_V3_SPECIFICYEARCOURSE_H
