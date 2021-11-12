//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_SPECIFICYEARCOURSE_H
#define PROGETTO_V3_SPECIFICYEARCOURSE_H


#include <vector>
#include <map>
#include "Course.h"
#include "Professor.h"
#include "Student.h"

typedef struct {
    Professor prof;
    hours assignedHours; ///ore assegnate ad ogni prof
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

class SpecificYearCourse : public Course {
private:
    int _startYear;    ///anno di inizio
    int _endYear;    ///anno di fine
    bool _active;    ///attivo o non attivo?
    int _paralleleCours;    ///numero di corsi in parallelo
    // per ogni corso in parallelo ho un vettore dei prof e stud
    //key num del corso in parallelo
    //value vettore delle struct prof o stud
    std::map<int, std::vector<professor>> _professors;
    std::map<int, std::vector<student>> _student;
};


#endif //PROGETTO_V3_SPECIFICYEARCOURSE_H
