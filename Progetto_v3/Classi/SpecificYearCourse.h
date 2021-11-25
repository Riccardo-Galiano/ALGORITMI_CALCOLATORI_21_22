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
    int _startEnrolYear; ///inizio corso di studi
    int _endEnrolYear; ///fine corso di studi
    int _grade;   ///voto
    bool _passed; ///promosso o bocciato
    
} student;

class SpecificYearCourse {
public:
    SpecificYearCourse(std::string sY_eY,bool active,int nCrsiPar, std::vector<std::string> prof, std::vector<std::string> exam,std::vector<std::string> idPar);
    //bool addProf(hours, Professor);
    const int getNumParallel() const;

    int getEndYear() const;

    bool getisActive() const;

    int getParalleleCours() const;

    const Exam &getExam() const;

    const std::map<std::string, std::vector<professor>> &getProfessors() const;

    const std::map<std::string, std::vector<student>> &getStudent() const;

    const std::string getProfParSTring() const;

    const std::string getParCourseIdString() const;
    const std::string getExamString() const;
    int getStartYear() const;

private:
    int _startYear;    ///anno di inizio
    int _endYear;    ///anno di fine
    bool _active;    ///attivo o non attivo?
    int _paralleleCours;    ///numero di corsi in parallelo
    std::vector<std::string> _idPar;
    Exam _exam;
    // per ogni corso in parallelo ho un vettore dei prof e stud
    //key id del corso in parallelo
    //value vettore delle struct prof o stud
    std::map<std::string, std::vector<professor>> _professors;
    std::map<std::string, std::vector<student>> _student;
    std::vector<professor> getProfsFromString (std::string profs);
    bool setProfMap(int, std::vector<std::string>, std::vector<std::string>);
};

std::ostream& operator<<(std::ostream& output, const SpecificYearCourse& s);

#endif //PROGETTO_V3_SPECIFICYEARCOURSE_H
