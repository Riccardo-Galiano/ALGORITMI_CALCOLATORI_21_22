//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_PROFESSOR_H
#define PROGETTO_V3_PROFESSOR_H

#include <vector>
#include <map>
#include "UniversityMember.h"
#include "Date.h"


class Professor : public UniversityMember {
public:
    ///constructor
    Professor():UniversityMember(){};
    Professor(int matr);
    Professor(int matr, std::string name, std::string surname, std::string email); ///matricola,nome, cognome, email
    Professor(int matr, std::string name, std::string surname, std::string email,std::string birth, std::string registration, std::string address); ///matricola,nome, cognome, email

    ///Professor management
    void setNoAvaibilities(int acYear, std::string& input);
    void addNewExam(std::string date, int hh, std::string cod_exam);

    ///output
    std::vector<std::string> outputNoAvailabilities(int nMatr);

    ///control
    void noAvailabilityClear(int year);
    bool amIavailable(std::string date, int hh, int relaxPar);

    ///remove
    void eraseThisAppealFromProfs(Date& dateAppeal, int startSlot, int numSlots);
private:
    //key: inizio anno accademico
    //value: coppia di date; inizio e fine periodo di indisponibilità
    std::map<int, std::vector<std::pair<Date, Date>>> _noAvailab;
    //key: "aaaa-mm-gg"
    //value: vettore di esami in ogni slot (hh,codice_esame)
    //NB!! se un esame dura più di uno slot, altrettanti pair ci saranno qui
    std::map<std::string,std::map<int,std::string>> _examsToDo;//data-slot-esame. Ogni prof avrà una mappa in cui salveremo gli esami che dovrà fare, in che giorno e a che ora

};

std::ostream &operator<<(std::ostream &prof, const Professor &s);


#endif //PROGETTO_V3_PROFESSOR_H
