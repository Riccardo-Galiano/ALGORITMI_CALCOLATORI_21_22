//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_PROFESSOR_H
#define PROGETTO_V3_PROFESSOR_H

#include <vector>
#include <map>
#include "UniversityMember.h"
#include "Date.h"

//std::cout << std::put_time(std::localtime(&now), "%Y-%m-%d") << "\n";
class Professor : public UniversityMember {
public:
    //Professor() = delete;
    Professor():UniversityMember(){};
    Professor(int matr);
    Professor(int, std::string, std::string, std::string); ///matricola,nome, cognome, email
    bool setNoAvaibilities(int, std::string& input);
    std::string outputNoAvailabilities(int);
    bool noAvailabilityPeriodIsEmpty();
    void noAvailabilityClear();
    int getNoAvalaibilityYear()const;
    bool addNewExam(std::string date, int hh, std::string cod_exam);
    bool amIavailable(std::string date, int hh);
private:
    std::pair<int, std::vector<std::pair<Date, Date>>> _noAvailab;
    //key: "aaaa-mm-gg"
    //value: vettore di esami in ogni slot (hh,codice_esame)
    //NB!! se un esame dura più di uno slot, altrettanti pair ci saranno qui
    std::map<std::string,std::map<int,std::string>> _examsToDo;
};

std::ostream &operator<<(std::ostream &prof, const Professor &s);


#endif //PROGETTO_V3_PROFESSOR_H
