//
// Created by lucam on 04/12/2021.
//

#include <algorithm>
#include "SessionYear.h"
#include "Parse.hpp"
#include "ExamDay.h"
#include "StudyCourse.h"

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

    _yearSessions.insert(std::pair<std::string, session>(name, s));//sessione Invernale, Estiva, Autunnale. Salvo la sessione
    this->setCaldendar(dates);

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


bool SessionYear::generateNewYearSession(std::string& fout, std::map<std::string, Course>& courses,std::map<int, Professor> &professors) {
    ///generare sessione invernale
    bool winter = generateThisSession("winter", courses, professors);
    ///generare sessione estiva
    bool summer = generateThisSession("summer", courses, professors);
    ///generare sessione autunnale
    bool autumn = generateThisSession("autumn",courses, professors);
    bool result;
    if (winter && summer && autumn)
        result = true;
    else
        result = false;
    return result;
}



///programma una sessione in particolare
bool SessionYear::generateThisSession(std::string sessName, std::map<std::string, Course>& courses,std::map<int, Professor>& profs) {
    ///prendiamo intervallo di date della sessione richiesta su cui dobbiamo ciclare
    session thisSession = _yearSessions.at(sessName);
    Date startDate = thisSession.startDate;
    Date endDate = thisSession.endDate;
    ///raggruppiamo tutti gli esami specifici di quest'anno
    std::vector<std::string> allExamAppealsToDo = getAllExamAppealsToDo(sessName,courses); //contiene le stringhe dei codici esame per OGNI appello
    ///cicliamo per organizzare le date degli appelli
    for(Date currentExamDay(startDate.getYear(),startDate.getMonth(),startDate.getDay()); currentExamDay.isEqual(endDate++) == false; currentExamDay++){
        if(!(currentExamDay.getWeekDay() == "Sunday")){
            //se non è domenica
            for(int indexExam=0; indexExam < allExamAppealsToDo.size(); indexExam++){//ciclo su tutti gli appelli da assegnare
                //prendo il corso considerato e vedo quante volte appare
                Course course = courses.at(allExamAppealsToDo[indexExam]);//corso dell'esame da assegnare
                SpecificYearCourse sp = course.getThisYearCourse(getAcYear());
                bool dateIsOk = dateIsOK(currentExamDay,sp,sessName);
                if(dateIsOk){
                    int startExamHour = checkIfProfsAvailableAndGapSameSemesterCourses(course,currentExamDay,profs);
                    if(startExamHour != -1){
                        assignTheExamToThisExamDay(startExamHour,currentExamDay,profs,sp);
                    }
                }
            }
        }
    }
    ///devo controllare che tutti gli appelli sono stati inseriti
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
std::vector<std::string> SessionYear::getAllExamAppealsToDo(std::string sessName, std::map<std::string, Course>& courses) {
    std::vector<std::string> allExamAppealsToDo;
    int semesterOfThisSession = getSemester(sessName); //primo semestre = winter, sec sem = summer, (terzo semestre) = autumn
    for(auto iterCourse = courses.begin(); iterCourse!=courses.end(); iterCourse++){
        SpecificYearCourse sp = iterCourse->second.getThisYearCourse(_acYear);
        int semester = sp.getSemester();
        bool isActive = sp.getisActive();
        ///controllo se è un corso di questo semestre ed è ATTIVO!!!!!!!!!!
        if(semester == semesterOfThisSession && isActive){
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

///controlla se può e dove può essere inserito l'esame ed eventualmente prende l'ora di inizio
int SessionYear::isPossibleToAssignThisExam(Course course,Date currentExamDay,std::map<int, Professor>& profs,int numSlot ) {
    ///controlliamo che il corso da inserire non abbia in un range di due giorni precedenti alla data analizzata un esame dello stesso corso di studi e dello stesso anno
    auto iterCalendar = _yearCalendar.find(currentExamDay.toString());//cerco la data in cui voglio inserire l'esame nel calendario
    for (int i = 0; i < 2; i++) {//per i due giorni precedenti alla data considerata
        iterCalendar--;
        ExamDay dayBefore = iterCalendar->second;//al primo ciclo prendo l'examday precedente alla data in cui voglio inserire l'esame. Al secondo ciclo prendo l'examDay di due giorni prima
        bool same = dayBefore.sameStudyCourseAndYear(course, _acYear);
        if (same)//se sameStudyAndYear ha trovato un esame già assegnato che appartenga allo stesso corso di studi e allo stesso anno da assegnare
            return -1;
    }
    ///controlliamo se ci sono slot liberi ed eventualmente se i prof in quegli slot sono liberi
    ExamDay examCurrentDay = _yearCalendar.at(currentExamDay.toString());
    int start = examCurrentDay.isPossibleToAssignThisExamToProf(course, profs, numSlot);
    return start;
}

///creo il calendario con i giorni delle sessioni, in cui ogni giorno sarà un examDay
bool SessionYear::setCaldendar(std::vector<Date> dates) {
    Date lastDay = ++dates[1];
    for(Date d = dates[0]; !d.isEqual(lastDay); d++){//per ogni giorno della sessione
        ExamDay examDay(d);
        _yearCalendar.insert(std::pair<std::string,ExamDay>(d.toString(),examDay));
    }

    return false;
}

bool SessionYear::dateIsOK(Date& newDate, SpecificYearCourse& sp, std::string& sessName) {
    ///controllo sulla data della sessione
    bool iCanBeAssigneToFirstTwoWeekOfExamSession = sp.canIBeAssigneToFirstTwoWeekOfExamSession(this->getSemester(sessName));
    if(iCanBeAssigneToFirstTwoWeekOfExamSession == false){
        session thisSession = _yearSessions.at(sessName);
        Date startDate = thisSession.startDate;
        if(startDate.whatIsTheGap(newDate) < 14){
            return false;
        }
    }
    ///continuo con il controllo se secondo appello
    int howManyTimesIAmAssignedAlreadyInThisSession = sp.amIAssignedAlreadyInThisSession(this->getSemester(sessName));
    if(howManyTimesIAmAssignedAlreadyInThisSession == 1){
        ///se è stato già assegnato allora devo controllare le due settimane di scarto
        Date lastDateAssignation = sp.lastDateAssignationInGivenSession(this->getSemester(sessName));
        if(lastDateAssignation.whatIsTheGap(newDate) < 14){
            return false;
        }
        else
            return true;
    }
    else
        ///non è stato assegnato già, quindi ok cosi
        return true;
}

int SessionYear::checkIfProfsAvailableAndGapSameSemesterCourses(Course& course, Date& currentExamDay, std::map<int, Professor>& profs) {
    Exam examToAssign = course.getExamSpecificYear(_acYear);//tempi, aule o lab, modalità
    int numSlots = examToAssign.howManySlots();//numero di slot che servono per l'esame
    return isPossibleToAssignThisExam(course,currentExamDay,profs,numSlots);//ora di inizio dello slot per l'esame
}

void SessionYear::assignTheExamToThisExamDay(int startExamHour, Date& date, std::map<int, Professor> & profs, SpecificYearCourse& sp) {
    /*


                if(startExam != -1) {//è disponibile un buco di numSlots
                         //aggiungo l'esame a quelli che i prof devono fare
                        _yearCalendar.at(currentExamDay.toString()).assignExamToProf(profs,course,startExam,numSlots);
                        //aggiungo l'esame al calendario della sessione
                        _yearCalendar.at(currentExamDay.toString()).assignExamToExamDay(startExam,course,numSlots);

                    if (counter == 2){
                        //devo aggiungere il secondo appello, ad almeno 14gg dal primo appello
                        Date secondAppealDate = currentExamDay.incrementOf(14);
                        bool found = false;
                        while(!found) {//finchè non trovo il giorno disponbibile a partire da 14 giorni dopo il primo appello
                            int startExamSecondAppeal = isPossibleToAssignThisExam(course, secondAppealDate, profs, numSlots);//ora di inizio dello slot per l'esame
                        if(startExamSecondAppeal !=-1){//ho trovato l'ora di inizio
                            //aggiungo l'esame a quelli che i prof devono fare
                            _yearCalendar.at(secondAppealDate.toString()).assignExamToProf(profs,course,startExam,numSlots);
                            //aggiungo l'esame al calendario della sessione
                            _yearCalendar.at(secondAppealDate.toString()).assignExamToExamDay(startExam,course,numSlots);
                            found = true;
                        } else
                            secondAppealDate++;
                        }
                        indexExam++;//essendo un secondo appello, se non lo facessi, in allExamAppealsToDo[indexExam] mi troverei lo stesso codice di prima
                    }
                }*/
    return;
}




