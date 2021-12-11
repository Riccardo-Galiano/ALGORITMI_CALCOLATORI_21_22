//
// Created by lucam on 04/12/2021.
//

#include <algorithm>
#include "SessionYear.h"
#include "Parse.hpp"
#include "ExamDay.h"

///costruttore
SessionYear::SessionYear(std::string acYear, std::string winterSession, std::string summerSession,
                         std::string autumnSession) {
    _acYear = Parse::getAcStartYear(acYear);
    this->addSession(acYear, winterSession, "winter");
    this->addSession(acYear, summerSession, "summer");
    this->addSession(acYear, autumnSession, "autumn");

}

///aggiunge il periodo delle sessioni
bool SessionYear::addSession(std::string acYear, std::string sessionDates, std::string name) {
    _acYear = Parse::getAcStartYear(acYear);
    std::vector<Date> dates = Parse::getDates(sessionDates);//vettore di Date in cui metterò la data di inizio e di fine
    if (name == "autumn") {
        if (!dates[0].checkGapGiven(4,
                                    dates[1]))//controllo se il periodo di settimane per la sessione autunnale sia esatto
            throw std::invalid_argument("durata sessione autunnale diversa da 4 settimane");
    } else if (!dates[0].checkGapGiven(6,
                                       dates[1])) {//controllo se il periodo di settimane per le sessione invernale/estiva sia esatto
        if (name == "summer")
            throw std::invalid_argument("durata sessione estiva diversa da 6 settimane");
        else
            throw std::invalid_argument("durata sessione invernale diversa da 6 settimana");
    }

    session s{name, dates[0], dates[1]};//salvo le info della sessione nome, data di inizio e data di fine

    _yearSessions.insert(
            std::pair<std::string, session>(name, s));//sessione Invernale, Estiva, Autunnale. Salvo la sessione
    return true;
}

///prende l'anno
int SessionYear::getAcYear() const {
    return _acYear;
}

///prende i periodi delle sessioni e li compatta in una stringa
std::string SessionYear::getSessions() const {
    std::stringstream output;
    int i = 0;
    ///parto dalla ine perchè la mappa organizza in ordine alfabetico quindi avremo nella struttura rispettivamente: autunno, estate e inverno
    /// ma il formato vuole prima l'inverno, poi l'estate e poi l'autunno
    for (auto iterSession = _yearSessions.rbegin();
         iterSession != _yearSessions.rend(); iterSession++) { //per ogni sessione dell'anno
        output << iterSession->second.startDate << "_" << iterSession->second.endDate;//aaaa-mm-gg_aaaa-mm-gg
        if (i < _yearSessions.size() - 1)//l'ultimo periodo non avrà il ";"
            output << ";";
        i++;
    }

    return output.str();
}

std::ostream &operator<<(std::ostream &sessions, const SessionYear &s) {
    sessions << s.getAcYear() << "-" << s.getAcYear() + 1 << ";"
             << s.getSessions();//aaaa-aaaa ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg
    return sessions;
}


bool SessionYear::generateNewYearSession(std::string& fout, const std::map<std::string, Course>& courses,std::map<int, Professor> &professors) {

    ///generare sessione invernale
    bool winter = generateThisSession("winter", courses,professors);
    ///generare sessione estiva
    bool summer = generateThisSession("summer", courses,professors);
    ///generare sessione autunnale
    bool autumn = generateThisSession("autumn",courses,professors);
    bool result;
    if (winter && summer && autumn)
        result = true;
    else
        result = false;
    return result;
}

//programma una sessione in particolare
bool SessionYear::generateThisSession(std::string sessName, const std::map<std::string, Course>& courses,std::map<int, Professor>& profs) {
    ///prendiamo intervallo di date della sessione richiesta su cui dobbiamo ciclare
    session thisSession = _yearSessions.at(sessName);
    Date startDate = thisSession.startDate;
    Date endDate = thisSession.endDate;
    ///raggruppiamo tutti gli esami specifici di quest'anno
    std::vector<SpecificYearCourse> examsToDoInThisSession;
    std::vector<std::string> allExamAppealsToDo = getAllExamAppealsToDo(sessName,courses); //contiene le stringhe dei codici esame per OGNI appello

    for(Date currentExamDay(startDate.getYear(),startDate.getMonth(),startDate.getDay());
            !currentExamDay.isEqual(endDate++);
            currentExamDay++){
        if(!(currentExamDay.getWeekDay() == "Sunday")){
            //non è domenica, ok
            for(int i=0; i < allExamAppealsToDo.size(); i++){

               Course course = courses.at(allExamAppealsToDo[i]);
               Exam exam = courses.at(allExamAppealsToDo[i]).getExamSpecificYear(_acYear);//tempi, aule o lab, modalità

               int counter = count(allExamAppealsToDo.begin(),allExamAppealsToDo.end(),allExamAppealsToDo[i]);
               //funzione che fa tutti i controlli: prof liberi, spazio dei due giorni
               controlOfAllocationRequirements(currentExamDay,course,exam,profs);

               //aggiungo primo appello a _slots di ExamDay
               if (counter == 2){
                   //devo aggiungere il secondo appello, a 14gg dall'altro
                   i++;
               }

            }
        }
    }

    return false;
}

int SessionYear::getSemester(std::string sessName) {
    if(sessName == "winter")
        return 1;
    else if(sessName == "summer")
        return 2;
    return 3;
}

///prende tutti gli appelli da fare nella sessione sessName
std::vector<std::string> SessionYear::getAllExamAppealsToDo(std::string sessName, const std::map<std::string, Course>& courses) {
    std::vector<std::string> allExamAppealsToDo;
    int semesterOfThisSession = getSemester(sessName); //primo semestre = winter, sec sem = summer, (terzo semestre) = autumn
    for(auto iterCourse = courses.begin(); iterCourse!=courses.end(); iterCourse++){
        SpecificYearCourse sp = iterCourse->second.getSpecificYearCourseFromYear(_acYear);
        int semester = sp.getSemester();
        ///controllo se è un corso di questo semestre
        if(semester == semesterOfThisSession){
            ///devo fare due appelli se i semestri sono uguali!!!!
            allExamAppealsToDo.push_back(iterCourse->first);
            allExamAppealsToDo.push_back(iterCourse->first);
        }
        else
            ///altrimenti solo uno...
            allExamAppealsToDo.push_back(iterCourse->first);
    }
    return allExamAppealsToDo;
}

bool SessionYear::controlOfAllocationRequirements(Date currentExamDate, Course course, Exam examToAssign,std::map<int, Professor> allUniversityProfs) {
    ExamDay examDay = _yearCalendar.at(currentExamDate.toString());
    int numSlots = examToAssign.howManySlots();//numero di slot che servono per l'esame
    int foundedStartHourSlot = examDay.isPossibleToAssignThisExam(course,allUniversityProfs,numSlots);//ora di inizio dello slot per l'esame


    return false;
}
