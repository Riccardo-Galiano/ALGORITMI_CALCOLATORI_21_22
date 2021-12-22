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
    Student stud;
    int _startEnrolYear; ///inizio corso
    int _endEnrolYear; ///fine corso
    int _grade;   ///voto
    bool _passed; ///promosso o bocciato
} student;

class SpecificYearCourse {
public:
    SpecificYearCourse(std::string ,bool ,int , std::vector<std::string> , std::vector<std::string> ,std::vector<std::string> , std::string , std::vector<int>);

    int getStartYear() const;
    int getEndYear() const;
    bool getisActive() const;
    int getParalleleCours() const;
    const std::map<int, std::vector<professor>> getProfsOfParallelCourses()const;
    const std::string getProfParString() const;
    std::vector<int> getAllProfMatr(); //ritorna LE MATRICOLE dei prof, indistintamente rispetto al corso parallelo a cui sono assegnati
    const std::string getGroupedCoursesIdString() const;
    std::vector<professor> getProfsFromString (std::string);
    const std::vector<std::string> &getIdGroupedCourses() const;
    const std::string getExamString() const;
    const Exam &getExam() const;
    int getSemester() const;
    int getYearOfTheSemester() const;
    std::vector<int> getStudyCourseAssigned() const;

    bool setYear();
    std::string setId(int)const;
    bool setProfMap(int, std::vector<std::string>);
    bool addStudent(Student,std::string,int);
    bool canIBeAssigneToFirstTwoWeekOfExamSession(int) const;
    int amIAssignedAlreadyInThisSession(int);
    Date lastDateAssignationInGivenSession(int);
    bool assignExamInThisSpecificYearCourse(Date,int);




private:
    std::string _yy_semester; //unico per tutti i corsi!!!
    std::vector<int> _studyCourseAssigned;
    int _startYear;    ///anno di inizio
    int _endYear;    ///anno di fine
    bool _active;    ///attivo o non attivo?
    int _parallelCourses;    ///numero di corsi in parallelo
    int totStudentsEnrolled = 0;
    int totStudentsNotPassed = 0;
    std::vector<std::string> _idGroupedCourses;
    Exam _exam;
    // per ogni corso in parallelo ho un vettore dei prof
    //key: numero versione parallela, quindi da 0 a ... n
    //value vettore delle struct prof
    std::map<int, std::vector<professor>> _professors;
    //key: student id
    //value: struct studente
    std::map<int, student> _student;
    //key: semester (session)
    //value: quante volte è stato programmato un suo esame in quel semestre
    std::map<int, std::vector<Date>> _howManyTimesIAmAssignedInASession;

};

std::ostream& operator<<(std::ostream& output, const SpecificYearCourse& s);

#endif //PROGETTO_V3_SPECIFICYEARCOURSE_H
