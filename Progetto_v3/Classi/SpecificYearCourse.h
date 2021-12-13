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
    SpecificYearCourse(std::string sY_eY,bool active,int nCrsiPar, std::vector<std::string> prof, std::vector<std::string> exam,std::vector<std::string> idGrouped, std::string yy_semester, int studyCourse);

    int getEndYear() const;
    bool getisActive() const;
    int getParalleleCours() const;
    const std::string getProfParString() const;
    const std::string getGroupedCoursesIdString() const;
    const std::string getExamString() const;
    int getStartYear() const;
    const Exam &getExam() const;
    std::vector<professor> getProfsFromString (std::string profs);
    std::vector<int> getAllProfMatr(); //ritorna LE MATRICOLE dei prof, indistintamente rispetto al corso parallelo a cui sono assegnati

    std::string setId(int nMatr)const;
    bool setProfMap(int, std::vector<std::string>);
    bool addStudent(Student,std::string,int);
    bool setYear();
    int getSemester() const;
    int getYearOfTheSemester() const;
    void resetAssignations();
    bool canIBeAssigneToFirstTwoWeekOfExamSession(int semesterGiven) const;
    int getStudyCourseAssign() const;
    int amIAssignedAlreadyInThisSession(int session);
    Date lastDateAssignationInGivenSession(int session);
private:
    std::string _yy_semester;
    int _studyCourseAssign;
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
