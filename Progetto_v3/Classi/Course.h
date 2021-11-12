//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_COURSE_H
#define PROGETTO_V3_COURSE_H


#include <string>
#include <vector>

typedef struct {
    int _lec;///ore lezione
    int _lab;///ore laboratori
    int _ex;///ore esame
} hours;

class Course {

protected:
    std::string _id;///codice identificativo del corso
    std::string _name;///nome del corso
    int _cfu;/// numero di crediti
    hours _hours;///ore impiegate nel corso; suddivise in ore di lezione,laboratori ed esame(struct hours)
    std::vector<Course*>_groupedCourse; ///corsi che avranno l'esame negli stessi giorni e nelle stesse ore
};


#endif //PROGETTO_V3_COURSE_H
