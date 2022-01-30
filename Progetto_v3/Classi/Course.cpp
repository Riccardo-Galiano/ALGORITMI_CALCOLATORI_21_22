//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "Course.h"
#include "Parse.hpp"
#include "DbException.h"
#include "InvalidDbException.h"
#include "StudyCourse.h"
#include <algorithm>
#include <iomanip>

Course::Course(const std::string &idCourse, const std::string &courseName, int cfu, int lessonHours,
               int exerciseHours, int labHours) {

    _id = idCourse;
    _name = courseName;
    _cfu = cfu;
    _hours._lec = lessonHours;
    _hours._ex = exerciseHours;
    _hours._lab = labHours;


}

///aggiunge per ogni anno accademico il corso con le sue informazioni
void Course::addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,
                                    std::vector<std::string> exam, std::vector<std::string> idGrouped,
                                    std::string yy_semester, std::vector<int> studyCourse, int line_counter) {
    bool lastYearActivity = false;
    int lastYear = 0;
    if(_courseOfTheYear.empty()== false){
        //prendo l'ultimo specifico anno prima del prossimo inserimento
        SpecificYearCourse lastSpecificYear = getLastSpecificYearCourse();
        lastYearActivity = lastSpecificYear.getIsActive();
        lastYear = lastSpecificYear.getStartYear();
    }

    ///key: l'anno di inizio dell'anno accademico. Value:: un oggetto SpecificYearCourse che conterrà le varie info specifiche per ogni anno accademico per ogni corso
    //inserisco il nuovo anno
    _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(stoi(sY_eY.substr(0, 4)),SpecificYearCourse(sY_eY, active, nCrsiPar, prof, exam,idGrouped, yy_semester,studyCourse,line_counter)));

    //se il penultimo anno il corso era attivo e quest'anno è spento setto anche l'anno di spegnimento
    if( lastYearActivity && active== false){
        _courseOfTheYear.at(stoi(sY_eY.substr(0, 4))).setAcYearOff(sY_eY);
    } else if (active == false){
        //è spento ma lo era anche l'anno prima
        //prendo l'anno di spegnimento salvato e lo risalvo l'anno dopo
        std::string offYear = _courseOfTheYear.at(lastYear).getAcYearOff();
        _courseOfTheYear.at(stoi(sY_eY.substr(0, 4))).setAcYearOff(offYear);
    }
}

///riempie il vettore di stringhe specificYearcourse
void Course::fillSpecificYearCourse(std::vector<std::string> &specificYearCourse,int line_counter) {
    std::string acYear = specificYearCourse[1];
    std::stringstream ss(acYear);
    int startYear, endYear;
    char c;
    std::string posLastGroupedCourses;

    //se l'utente usasse la possibilità di lasciare vuoto il campo invariato rispetto all'anno precedente sull'ultimo campo di informazioni(id corsi)
    //specificYearCourse avrebbe soltanto 6 campi quindi dovremmo aggiungere un settimo campo inizialmente vuoto che verrà poi riempito dalle info
    //dell'anno precedente
    if (specificYearCourse.size() == 6) {
        specificYearCourse.push_back(posLastGroupedCourses);
    }
    ss >> startYear >> c >> endYear;
    SpecificYearCourse lastYearSpecificCourse = getLastSpecificYearCourse();
    std::stringstream last;

    std::vector<std::string> lastYearSpecificYearCourseSplitted;
    last << lastYearSpecificCourse;
    lastYearSpecificYearCourseSplitted = Parse::splittedLine(last.str(), ';');
    ///se il campo è vuoto lo riempio con le info dell'anno precedente
    for (int i = 2; i < specificYearCourse.size(); i++) {
        if (specificYearCourse[i].empty()) {
            specificYearCourse[i] = lastYearSpecificYearCourseSplitted[i - 1];
        }
    }
    std::string profSenzaQuadre;
    int new_num_par_courses = stoi(specificYearCourse[3]);
    //check correttezza

    profSenzaQuadre = specificYearCourse[4].substr(1, specificYearCourse[4].size() - 2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
    //tornerà errore se non congruenti
    std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre, new_num_par_courses);//divido i vari corsi in parallelo

}

///modifica uno studente in un anno specifico con il suo voto
void Course::modifyStudentAsPassedToSpecYearCourse(int acYear, Student& stud, int enrolYear, int mark,std::string appealsDate) {
    SpecificYearCourse sp = getLastSpecificYearCourse();
    int lastYear = sp.getStartYear();
    //controllo che lo studente sia iscritto al corso
    std::vector<std::string> studyPlan = stud.getStudyPlan();
    if(find(studyPlan.begin(),studyPlan.end(),getId())== studyPlan.end()){
        throw std::invalid_argument(" non e' iscritto al corso " + getId() + "\n");
    }
    for(int year = acYear; year <= lastYear ; year++) {
        _courseOfTheYear.at(year).addGradeToStudent(stud, acYear, mark, appealsDate,getId());
    }
}

///prende l'ultimo oggetto SpecificYearCourse dalla map _coursOfTheYear
SpecificYearCourse &Course::getLastSpecificYearCourse() {
    int lastYear = 0, actualYear;

    for (auto iter = _courseOfTheYear.begin();
         iter != _courseOfTheYear.end(); iter++) {//per ogni anno accademico di un corso
        actualYear = iter->second.getStartYear();//punta all'oggetto di tipo SpecificYearCourse e ne prende l'anno di inizio(che poi sarebbe la key)
        //potrei scrivere actualYear = iter->first
        if (actualYear > lastYear)
            lastYear = actualYear;//prendo l'ultimo anno

    }
    return _courseOfTheYear.at(lastYear);
}

///legge l'Id
const std::string &Course::getId() const {
    return _id;
}

///lette il nome
const std::string &Course::getName() const {
    return _name;
}

///legge i cfu
int Course::getCfu() const {
    return _cfu;
}

///legge le ore
const hours &Course::getHours() const {
    return _hours;
}

///ogni stringa è costituita dalle info di uno specifico anno accademico del corso analizzato
std::vector<SpecificYearCourse> Course::getSpecificYearsCourse() {

    std::vector<SpecificYearCourse> specificYearsCourse;

    for (auto iterSpecificYearCourse = _courseOfTheYear.begin();
         iterSpecificYearCourse != _courseOfTheYear.end(); iterSpecificYearCourse++) {//per ogni anno accademico
        //std::stringstream tokens;
        SpecificYearCourse specific = iterSpecificYearCourse->second;//salvo le info puntate da iterSpecificYearCourse che contiene le info dello specifico anno accademico
        //tokens << "a;" << specific; //overload dell'operatore. (si rimanda all'overload di << in SpecificYearCourse.cpp)
        specificYearsCourse.push_back(specific);//salvo la stringa con le info dell'anno
    }

    return specificYearsCourse;
}

///prendo la dimensione della mappa di anni accademici
int Course::getSpecificYearCourseSize() const {
    return _courseOfTheYear.size();
}

///prende il corso con le sue info ad uno specifico anno
const SpecificYearCourse& Course::getThisYearCourse(int year) const {
        return _courseOfTheYear.at(year);
}

///prende le regole e gli orari dell'esame per un anno specifico di un corso
const Exam Course::getExamSpecificYear(int acYear) const {
    return _courseOfTheYear.at(acYear).getExam();
}

///ritorna il corso ad un anno specifico anno ma come reference
SpecificYearCourse &Course::getThisYearCourseReference(int year){
    if(_courseOfTheYear.count(year)==0){
        ///non ci sono corsi per quell'anno
        throw std::invalid_argument("Non ci sono corsi selezionabili nell'anno accademico richiesto\n");
    }
    return _courseOfTheYear.at(year);
}

///il corso è vuoto?
bool Course::courseOfTheYearIsEmpty() {
    if (_courseOfTheYear.empty()) {
        return true;
    } else {
        return false;
    }
}

///riempi i buchi tra anni accademici di un corso
void Course::fillAcYearsEmpty() {
    for (auto iterCourseOfTheYear = _courseOfTheYear.begin();iterCourseOfTheYear != _courseOfTheYear.end(); iterCourseOfTheYear++) {//per ogni anno accademico
        auto token = iterCourseOfTheYear;//iteratore all'elemento corrente
        auto iterSuccessiveCourse = ++token;//iteratore al successivo elemento in memoria

        if ((iterSuccessiveCourse->first - iterCourseOfTheYear->first > 1) && iterSuccessiveCourse != _courseOfTheYear.end()) {//se c'è un gap di almeno 2 anni (mancano degli anni accademici) e non sono alla fine di _courseOfTheYear
            int range = (iterSuccessiveCourse->first - iterCourseOfTheYear->first) - 1;//quanti anni accademici mancano e che devo aggiungere
            int lastYear = iterCourseOfTheYear->first;//ultimo anno prima del gap
            for (int i = 0; i < range; i++) {//per il numero di anni accademici da aggiungere
                //riscrivo l'anno precedente
                lastYear++;//il nuovo _courseOfTheYear avrà l'anno subito successivo all'ultimo anno accademico prima del gap
                //prendo l'ultimo SpecificYearCourse prima del gap
                SpecificYearCourse sC = iterCourseOfTheYear->second;
                sC.setYear();//setta l'anno (aggiungerà 1 all'anno di inizio e all'anno di fine)
                //insert dell'anno nuovo
                _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(lastYear, sC));//aggiungo l'anno accademico
                iterCourseOfTheYear++;//per il prossimo ciclo devo prendere l'ultimo prima del gap(che sarà quello appena aggiunto)
            }
        }
    }
}

///controllo che esistano i professori e che le ore in totale per lezioni, lab ed esercitazioni combacino con quelle del corso
void Course::controlTheExistenceAndHoursOfProfessors(const std::map<int, Professor> &professors, int year,
                                                     int line_counter) {
        SpecificYearCourse sp = _courseOfTheYear.at(year);
        std::string error;
        bool isOk = true;
        hours hoursCourse = _hours;
        std::map<int, std::vector<professor>> profsOfParallelCourses = sp.getProfsOfParallelCourses();
        for (int i = 0; i < profsOfParallelCourses.size(); i++) {
            std::vector<professor> profsOfSingleCourse = profsOfParallelCourses.at(i);
            hours hourProfs = controlProfsOfSingleCourse(profsOfSingleCourse, professors, line_counter);
            if(hoursCourse._lec != hourProfs._lec) {
                error.append("Le ore delle lezioni non sono coerenti con le ore del corso alla riga " + std::to_string(line_counter) + "\n");
                isOk = false;
            }
            else if (hoursCourse._lab != hourProfs._lab) {
                error.append("Le ore dei laboratori non sono coerenti con le ore del corsoalla riga " + std::to_string(line_counter) +"\n");
                isOk = false;
            }
            else if (hoursCourse._ex != hourProfs._ex) {
                error.append("Le ore delle esercitazioni non sono coerenti con le ore del corsoalla riga " + std::to_string(line_counter) + "\n");
                isOk = false;
            }
        }
        if(isOk == false){
            throw std::invalid_argument(error);
        }
}

///prendo le ore totali dei prof per un singolo corso
hours Course::controlProfsOfSingleCourse(std::vector<professor> profsOfSingleCourse,
                                         const std::map<int, Professor> &professors, int line_counter) {
    hours h{0,0,0};
    ///per ogni prof del corso verifico se esista nel db_professori, in tal caso prendo le sue ore e le sommo
    for (int i = 0; i < profsOfSingleCourse.size(); i++) {
        if (professors.find(profsOfSingleCourse[i].prof_id) == professors.end()) {
            std::string settedId = Parse::setId('d',6,profsOfSingleCourse[i].prof_id);
            throw std::invalid_argument("Il seguente professore " + settedId +" al rigo "+std::to_string(line_counter)+ " non e' stato trovato nel database\n");
        } else
        {
            h._lec = h._lec + profsOfSingleCourse[i].hLez;
            h._lab = h._lab + profsOfSingleCourse[i].hLab;
            h._ex = h._ex + profsOfSingleCourse[i].hExe;
        }
    }
    return h;
}

///per anno accademico ho i corsi raggruppati di un corso
std::map<int,std::vector<std::string>> Course::getGroupedCourseFromAllYear() {
    std::map<int, std::vector<std::string>> allGrouped;
    for(auto iterSpecificYear = _courseOfTheYear.begin();iterSpecificYear != _courseOfTheYear.end(); iterSpecificYear++){
        std::vector<std::string> vect = iterSpecificYear->second.getIdGroupedCourses();
        allGrouped.insert(std::pair<int, std::vector<std::string>>(iterSpecificYear->first,vect));
    }
    return  allGrouped;
}

///controlla se è stato già spento
void Course::notActive() {
     bool firstTime = false;
     for(auto iterSpecific = _courseOfTheYear.begin(); iterSpecific != _courseOfTheYear.end();iterSpecific++){
         if(iterSpecific->second.getIsActive() == false)///controllo se il corso è non attivo
             firstTime = true;
         else if(firstTime)///se è attivo ma negli anni precedenti era già disattivato
             throw std::logic_error("Il corso con codice:" + getId() + "e' gia' stato spento! non puo' essere riattivato! Controllare l'anno:" + std::to_string(iterSpecific->first));
    }
}

///prende il semestre a cui è associato un corso
int Course::getSemesterAtYear(int acStartYear,std::string name) {
   int sem = 0;
    if(_courseOfTheYear.find(acStartYear) != _courseOfTheYear.end())
          sem =_courseOfTheYear.at(acStartYear).getSemester();
    else
        throw InvalidDbException("Non esistono info per il corso: " + name + "\n");
    return sem;
}

///controllo se i raggruppati siano dello stesso semestre affinchè possano essere effettivamente raggruppabili
void Course::sameSemesterGrouped(std::map<std::string,Course> courses) {
    bool isOk = true;
    std::string error;
    for(auto iterSpecificYear = _courseOfTheYear.begin(); iterSpecificYear != _courseOfTheYear.end(); iterSpecificYear++){
        std::vector<std::string> groupedCourse = iterSpecificYear->second.getIdGroupedCourses();
        int sem = 0;
        bool active = iterSpecificYear->second.getIsActive();
        for (int i = 0; i < groupedCourse.size();i++){
            ///controllo che i raggruppati siano o tutti spenti o tutti attivi
            SpecificYearCourse sp = courses.at(groupedCourse[i]).getThisYearCourse(iterSpecificYear->first);
            bool activeGrouped = sp.getIsActive();
            ///se i due corsi non si trovano nello stato di attività non posso raggrupparli
            if(active != activeGrouped) {
                error.append("Il seguente corso raggruppato " + groupedCourse[i] +
                             " non e' nello stesso stato di attivita' del corso: " + getName() +
                             " ;corrispondente al codice: " + getId() + "\n");
                isOk = false;
            }
            ///se sono entrambi attivi mi chiedo se sono dello stesso semestre
            if(active != false && activeGrouped != false) {
                sem = iterSpecificYear->second.getSemester();
                int semGrouped = courses.at(groupedCourse[i]).getSemesterAtYear(iterSpecificYear->first,groupedCourse[i]);
                if (sem != -1 && semGrouped != -1) {
                    //se almeno uno dei due è -1 la coerenza tra loro non si può dimostrare quini lo considero ok
                    if (semGrouped != sem) {
                        error.append("Il seguente corso raggruppato " + groupedCourse[i] +
                                     " non e' dello stesso semestre di: " + getName() + " ;corrispondente al codice: " +
                                     getId() + "\n");
                        isOk = false;
                    }
                }
            }
        }
    }
    if(isOk == false){
        throw std::logic_error(error);
    }
}

///tengo traccia del semstre e dell'anno del corso spento
void Course::assignYY_Sem(std::string& acYYoff, std::string& yy_semester) {
    for(auto iterSpecific = _courseOfTheYear.begin();iterSpecific != _courseOfTheYear.end(); iterSpecific++){
        iterSpecific->second.assignYY_SemToAllYear(acYYoff, yy_semester);
    }
}

///registro uno studente ad uno specifico anno(anno di iscrizione)
void Course::registerStudentsToSpecificYear(int acYearRegistration, Student &stud) {
    ///prendo l'ultimo anno e riempio la struttura fino all'anno che mi serve per l'iscrizione perchè l'ultimo aggiornamento può essere vecchio
    SpecificYearCourse sp = getLastSpecificYearCourse();
    int lastYear = sp.getStartYear();

    std::string acYear = std::to_string(acYearRegistration) + "-" + std::to_string(acYearRegistration+1);
    std::string settedId = Parse::setId('s',6,stud.getId());
    if(acYearRegistration > lastYear) {
        fillAcYearsUntilStartAcYear(acYearRegistration, lastYear);
    }
    //se non lo trova è perchè il corso non era ancora attivo in quell'anno
    if(_courseOfTheYear.find(acYearRegistration) == _courseOfTheYear.end()) {
        throw InvalidDbException("il seguente corso: " + getId() + " non era attivo quando si e' iscritto lo studente con matricola: " + settedId + " nel "+ acYear);
    } else if (_courseOfTheYear.at(acYearRegistration).getIsActive() == false){
        std::string firstAcYearOff = this->getFirstAcYearOff();
        throw InvalidDbException("il seguente corso: " + getId() + " e' stato spento nel " + firstAcYearOff + ". Lo studente con matricola: " + settedId + " non puo' essere iscritto nel "+ acYear);
    }
    ///salviamo i suoi dati di iscrizione nell'anno di iscrizione
    /// ma dobbiamo ricordarci di aggiornare gli iscritti anche per gli anni successivi al suo
    for(int year = acYearRegistration; year <= lastYear ; year++) {
        _courseOfTheYear.at(year).addStudent(acYearRegistration, stud);
    }


}

///formatta le righe per il dbAppelli trasformando le info per ogni studente in stringhe
std::vector<std::string> Course::getAcYearStudExam() {
    std::vector<std::string> allAppealsPerYearToString;
    bool push = false;
  for(auto iterSpecific = _courseOfTheYear.begin();iterSpecific != _courseOfTheYear.end(); iterSpecific++){

      std::vector<Date> allAppealsPerYear = iterSpecific->second.getAllAppeals();
      for(int i = 0; i < allAppealsPerYear.size(); i++) {
          std::stringstream ss;
          std::map<int, student> allStudentPassed = iterSpecific->second.getStudentsPassedInThisAppeal(allAppealsPerYear[i]);
          ss <<getId()<<";"<< iterSpecific->first <<"-"<<iterSpecific->first+1<<";"<<allAppealsPerYear[i]<<";[";
          int count = 0;
           for(auto iterStudent = allStudentPassed.begin();iterStudent != allStudentPassed.end();iterStudent++){
               student currentStud = iterStudent->second;
                   //controllo a quale appello si sta facendo riferimento

                       //l'appello è lui quindi deve essere scritto nel database
                       std::string settedId = Parse::setId('s',6,currentStud._studId);
                       ss<< "{"<< settedId << "," << currentStud._grade << "}";
                       push = true;

                       if(count<allStudentPassed.size()-1)
                       ss<<",";

               count++;
           }
          ss<<"]";
           if(push == true) {
               push = false;
               allAppealsPerYearToString.push_back(ss.str());
           }
      }
  }
    return  allAppealsPerYearToString;
}

///assegna gli appelli agli studenti per un anno specifico
void Course::assignStudToAppealPerYear(std::string acYear, std::string appealDate, std::string allStudsPassedExamString) {
    std::vector<std::pair<std::string, int>> allStudPassedExam = splittAllStudPassedExamString(allStudsPassedExamString);
    int startAcYear = Parse::getAcStartYear(acYear);
    _courseOfTheYear.at(startAcYear).assignAllStudsPassedExam(allStudPassedExam,appealDate);

}

///splitta matricola e voto
std::vector<std::pair<std::string, int>> Course::splittAllStudPassedExamString(std::string allStudsPassedExamString) {
    std::vector<std::pair<std::string, int>> allStudsPassedExam;
    std::vector<int> posCB = Parse::posCurlyBrackets(allStudsPassedExamString);
    for(int i = 0; i < posCB.size()-1; i = i+2){
        std::string studAndGrade = allStudsPassedExamString.substr(posCB[i]+1,10);
        std::vector<std::string> token = Parse::splittedLine(studAndGrade,',');
        allStudsPassedExam.emplace_back(token[0],stoi(token[1]));
    }
    return allStudsPassedExam;
}

///prende gli appelli di un anno
std::vector<std::string> Course::getAcYearAppeals() {
    std::vector<std::string> allAppealsPerCourses;
    for(auto iterSpecific = _courseOfTheYear.begin();iterSpecific != _courseOfTheYear.end(); iterSpecific++){
        if(iterSpecific->second.notExamsAssigned() == false) {
            std::stringstream ss;
            std::string appealsForSessions = iterSpecific->second.getAppealsForAllSession();
            ss << getId() << ";" << iterSpecific->first << "_" << iterSpecific->first + 1 << ";" << appealsForSessions;
            allAppealsPerCourses.push_back(ss.str());
        }
    }
    return allAppealsPerCourses;
}

///assegna gli appelli ad un anno
void Course::assignAppealToSpecificYear(std::string acYear, std::string session, std::vector<Date> appealsPerSession, std::vector<int> startSlotPerAppeal, std::vector<std::string> classroomsPerAppeal) {
    int acStartYear = Parse::getAcStartYear(acYear);
    _courseOfTheYear.at(acStartYear).assignAppeals(session,appealsPerSession,startSlotPerAppeal,classroomsPerAppeal);
}

///controllo che l'appello dia effettivamente una data e che esiat nella sessione
void Course::controlAppeal(std::string appealDate) {
    Date appeal = Parse::controlItCanBeADate(appealDate);
    int yearAppeal = appeal.getYear();
    int month = appeal.getMonth();
    auto firstYearCourse = _courseOfTheYear.begin();

    ///controllo che per l'anno di quell'appello il corso risulti già attivo
    //se novembre di AAAA devo considerare come inizio anno accademico l'anno dell'appello
    //se da gennaio devo considerare che l'inizio dell'anno accademico non l'anno dell'appello ma quello precedente
    if(month >= 1 && month <= 10){
        yearAppeal--;
    }
    if(yearAppeal < firstYearCourse->first ) {
        throw std::invalid_argument("Il corso non esisteva nell'anno accademico" + std::to_string(yearAppeal) + "_" +
                                    std::to_string(yearAppeal + 1) + ". Impossibile assegnare voti \n");
    }
    std::vector<Date> allAppealsPerYear;
    //DA DIMOSTRARE QUANDO SI GENERANO SESSIONI PER ANNI IN CUI NON ABBIAMO AGGIORNAMENTE DEL CORSO
    if(_courseOfTheYear.find(yearAppeal) == _courseOfTheYear.end()){
        throw std::invalid_argument(" Non e' stato assegnato alcun esame per il corso nell'anno accademico" + std::to_string(yearAppeal) + "_" +
                                    std::to_string(yearAppeal + 1) + ". Impossibile assegnare voti \n");

    }
    allAppealsPerYear = _courseOfTheYear.at(yearAppeal).getAllAppeals();
    if (std::find(allAppealsPerYear.begin(), allAppealsPerYear.end(), appeal) == allAppealsPerYear.end())
        throw std::invalid_argument("In data " + appealDate + " non ci sono esami effettuati per il corso " + getId() +"\n");

}

///Controlla se il prof sia effettivamente assegnato a questo corso in uno specifico anno
bool Course::profHaveThisCourse(int matr, int acStartYear) {
    SpecificYearCourse sp = getLastSpecificYearCourse();
    int lastYear = sp.getStartYear();
    int firstYear = getFirstYearOfActivity();
    ///se il corso non era ancora attivo non si puo' fare nulla, scateno l'eccezione
    if(acStartYear < firstYear){
        throw std::invalid_argument("Il corso "+ getId() + " non era ancora attivo nel "+ std::to_string(acStartYear) + "-" + std::to_string(acStartYear+1)+ "\n");
    }else if(lastYear <acStartYear){
        ///se il corso semplicemente non è stato aggiornato
        fillAcYearsUntilStartAcYear(acStartYear,lastYear);
    }
    std::vector<int> profOfCourse = _courseOfTheYear.at(acStartYear).getAllProfMatr();
    return std::find(profOfCourse.begin(), profOfCourse.end(), matr) != profOfCourse.end();
}

///prende il numero di slots necessari alla generazione dell'esame in un determinato anno
int Course::getExamSlotPerYear(std::string acYear) {
    int startAc = Parse::getAcStartYear(acYear);
    Exam examInfo = _courseOfTheYear.at(startAc).getExam();
    return examInfo.howManySlots();
}

///prende i prof assegnati ad un corso in un determinato anno
std::vector<int> Course::getProfsPerYear(std::string acYear) {
    int startAc = Parse::getAcStartYear(acYear);
    return  _courseOfTheYear.at(startAc).getAllProfMatr();
}

///prende il primo anno di attività del corso
int Course::getFirstYearOfActivity() {
    auto iterSpecificYearCourse = _courseOfTheYear.begin();
    return iterSpecificYearCourse->first;
}

///eredita per un corso le informazioni necessarie di anno in anno fino a lastYear
void Course::fillAcYearsUntilStartAcYear(int startAcYear, int lastYear) {

    for(int i = lastYear; i < startAcYear; i++){
        SpecificYearCourse nextSpecificYearCourse = _courseOfTheYear.at(lastYear);
        int newStartYear = ++lastYear;
        nextSpecificYearCourse.setNewYear(newStartYear);
        _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(newStartYear,nextSpecificYearCourse));
    }

}

///aggiorna l'anno e semestre in tutti gli specific year del corso
void Course::updateYYSemesterInAllSpecYearCourse(std::string& yy_semester) {
    for(auto iter = _courseOfTheYear.begin(); iter != _courseOfTheYear.end(); iter++) {
        iter->second.setYySemester(yy_semester);
    }
}

///prend eil primo anno di spegnimento
std::string Course::getFirstAcYearOff() {
    for(auto iterSpecificYear = _courseOfTheYear.begin(); iterSpecificYear != _courseOfTheYear.end(); iterSpecificYear++){
        int startYear = iterSpecificYear->second.getStartYear();
        int endYear = iterSpecificYear->second.getEndYear();
        std::string acYear = std::to_string(startYear) + "-" + std::to_string(endYear);
        if(iterSpecificYear->second.getIsActive() == false){
            return acYear;
        }
    }
    return  "";
}

///prende gli id dei raggruppati per uno specifico anno
std::vector<std::string> Course::getIdGroupedCourseFromYear(int acYear) {
    return _courseOfTheYear.at(acYear).getIdGroupedCourses();
}

///riassegna l'appello ad uno specifico anno. Request_changes nl caso in cui non si possa riscrivere l'esame
void Course::reassignAppealToSpecificYear(int acYear,int numAppeal, int numSession, Date date, int startSlot,
                                          std::vector<int> classroomsPerCourse) {
    _courseOfTheYear.at(acYear).reassignAppeal(numAppeal,numSession,date,startSlot,classroomsPerCourse);


}

///aggiorna i corsi di studio per uno specifico anno
void Course::updateStudyCourseInAllSpecYearCourse(int idStudyCourse) {
    for(auto iter = _courseOfTheYear.begin(); iter != _courseOfTheYear.end(); iter++) {
        iter->second.updateStudyCourseAssigned(idStudyCourse);
    }
}

///Mi chiedo se il corso esiste anche in quell'anno
bool Course::courseExistInThisYear(int year) {
    return _courseOfTheYear.count(year) != 0;
}

///output operator overload
std::ostream &operator<<(std::ostream &course, Course &c) {
    course << "c;" << c.getId() << ";" << c.getName() << ";" << c.getCfu() << ";" << c.getHours()._lec << ";"
           << c.getHours()._ex << ";" << c.getHours()._lab << std::endl;
    int size = c.getSpecificYearCourseSize();
    std::vector<SpecificYearCourse> SYCourse = c.getSpecificYearsCourse();
    for (int i = 0; i < size; i++) {
        course << "a;" << SYCourse[i] << std::endl;
    }
    return course;
}
