//
// Created by lucam on 04/12/2021.
//

#include <algorithm>
#include <fstream>
#include "SessionYear.h"
#include "Parse.hpp"
#include "ExamDay.h"
#include "StudyCourse.h"

bool winter = false;
bool summer = false;
bool autumn = false;


enum{caso_estremo, caso_medio, caso_lasco};

///costruttore
SessionYear::SessionYear(std::string& acYear, std::string& winterSession, std::string& summerSession,std::string& autumnSession, std::string& output_file_name): _sysLog(output_file_name,Parse::getAcStartYear(acYear)){
    _acYear = Parse::getAcStartYear(acYear);
    _sessionNames.emplace_back("winter");
    _sessionNames.emplace_back("summer");
    _sessionNames.emplace_back("autumn");
    ///aggiungo le date della sessione in base all'anno e alla sessione
    this->addSession(acYear, winterSession, _sessionNames[0]);
    this->addSession(acYear, summerSession, _sessionNames[1]);
    this->addSession(acYear, autumnSession, _sessionNames[2]);
}

///aggiunge il periodo delle sessioni
bool SessionYear::addSession(std::string& acYear, std::string& sessionDates, std::string& name) {
    _acYear = Parse::getAcStartYear(acYear);
    ///vettore di Date in cui metterò la data di inizio e di fine
    std::vector<Date> dates = Parse::getDates(sessionDates);
    if (name == _sessionNames[2]) {//autunnale
        ///controllo se il periodo di settimane per la sessione autunnale sia esatto
        if (!dates[0].checkGapGiven(4,dates[1]))
            throw std::invalid_argument("durata sessione autunnale diversa da 4 settimane");
    } else if (!dates[0].checkGapGiven(6,dates[1])) {//controllo se il periodo di settimane per le sessione invernale/estiva sia esatto
        if (name == _sessionNames[1])
            throw std::invalid_argument("durata sessione estiva diversa da 6 settimane");
        else if (name == _sessionNames[0])
            throw std::invalid_argument("durata sessione invernale diversa da 6 settimana");
    }

    session s{name, dates[0], dates[1]};//salvo le info della sessione nome, data di inizio e data di fine

    _yearSessions.insert(std::pair<std::string, session>(name, s));//sessione Invernale, Estiva, Autunnale. Salvo la sessione
    this->setCaldendar(dates);

    return true;
}

///creo il calendario con i giorni delle sessioni, in cui ogni giorno sarà un examDay
bool SessionYear::setCaldendar(std::vector<Date> dates) {
    ///per ogni giorno della sessione setto la mappa YearCalendar con key la data sottoforma di stringa e un oggetto examDay come value
    for(Date d = dates[0]; !d.isEqual(dates[1]++); d = d++){
        ExamDay examDay(d);
        _yearCalendar.insert(std::pair<std::string,ExamDay>(d.toString(),examDay));
    }
    return false;
}

///genera le sessioni dell'anno accademico considerato
bool SessionYear::generateNewYearSession(std::string& fout, std::map<std::string, Course>& courses,std::map<int, Professor> &professors,int relaxPar) {
    int gapAppealsSameCourse=14;
    bool result=false;
    bool exitloop= false;

    ///eseguo il ciclo finchè posso rilassare ulteriormente il vincolo dei giorni tra appelli di uno stesso esame (14 giorni,13,12...)
    for (; gapAppealsSameCourse>=0 && !exitloop; gapAppealsSameCourse--){
        ///generare sessione invernale
        if(winter == false)
        winter = generateThisSession("winter", courses, professors, relaxPar, gapAppealsSameCourse);
        ///generare sessione estiva
        if(summer == false)
        summer = generateThisSession("summer", courses, professors,relaxPar, gapAppealsSameCourse);
        ///generare sessione autunnale
        if(autumn == false)
        autumn = generateThisSession("autumn",courses, professors, relaxPar, gapAppealsSameCourse);

        ///se le tre sessioni sono state generate passo all'output su file
        if (winter && summer && autumn) {
            generateOutputFiles(fout,1,courses);
            generateOutputFiles(fout,2,courses);
            generateOutputFiles(fout,3,courses);
            result = true;
            //posso uscire dal loop, non aspetto che il vincolo sia meno di 14 giorni
            exitloop=true;
        }
        else {///se non sono state generate
            ///reset strutture dati ToDo
            result = false;
            if (relaxPar<2){
                ///se non ci sono vincoli rilassati oppure relaxPar=1 esco
                exitloop=true;
            }
            ///else -> se relaxPar =2 opp =3, allora deve continuare a ciclare fino a quando può
        }
    }
    return result;

}

///programma una sessione in particolare
bool SessionYear::generateThisSession(std::string sessName, std::map<std::string, Course>& courses,std::map<int, Professor>& profs, int relaxPar, int gapAppeals) {
    ///prendiamo l'intervallo di date della sessione richiesta su cui dobbiamo esguire il ciclo
    session thisSession = _yearSessions.at(sessName);
    Date startDate = thisSession.startDate;
    Date endDate = thisSession.endDate;
    bool continueLoop = true;
    bool pop = false;
    ///raggruppiamo tutti gli esami specifici di quest'anno
    if(relaxPar == 0)
        _allExamAppealsToDo = getAllExamAppealsToDo(sessName, courses); //contiene le stringhe dei codici esame per OGNI appello
    ///else -> nulla, _allExamAppealsToDo già caricato
    ///cicliamo su ogni data della sessione per organizzare le date degli appelli
    for(Date currentExamDay(startDate.getYear(),startDate.getMonth(),startDate.getDay()); continueLoop && currentExamDay.isEqual(endDate++) == false; currentExamDay = currentExamDay++){
        //se non è domenica
        if(!(currentExamDay.getWeekDay() == "Sunday")){

            ///ciclo su tutti gli appelli da assegnare
            for(int indexExam=0; indexExam < _allExamAppealsToDo.size() && continueLoop; indexExam++){
                //il controllo su pop mi serve perchè se dovessi togliere degli elementi dal vettore potrei rischiare di non contare certi esami che slitterebbero ad una posizione indexExam ornai passata.
                // Quindi per sicurezza, se dovessi togliere degli elementi dal vettore, azzero l'indice e ricomincio da zero, i secondi appelli verranno saltati alla fine del primo controllo
                if(pop) {
                    indexExam = 0;
                    pop = false;
                }
                ///esami raggruppati da considerare in questo giro
                std::string codCurrentAppeal = _allExamAppealsToDo[indexExam];
                std::vector<std::string> coursesGrouped = getGroupedCourses(courses, codCurrentAppeal);
                std::vector<Course> coursesToConsiderInThisLoop;
                for(int i=0; i<coursesGrouped.size(); i++){
                    coursesToConsiderInThisLoop.push_back(courses.at(coursesGrouped[i]));
                }

                ///dobbiamo verificare che la data corrente sia possibile per tutti gli esami da inserire in questo giro
                bool dateIsOk = true;
                if(sessName != "autumn"){
                    for(int i = 0; i < coursesToConsiderInThisLoop.size() && dateIsOk; i++){
                        Course courseToConsider = coursesToConsiderInThisLoop[i];
                        //la data analizzata rispetta i primi requisiti per l'assegnazione di un esame(specificYearCourse)
                        //requisiti: se non stesso semestre o non attivo o entrambi o secondi appelli va oltre i primi 14 giorni della sessione; se secondo appello va 14 giorni oltre il primo appello
                         if(dateIsOK(currentExamDay, courseToConsider, sessName, gapAppeals) == false){
                        //se non va bene per uno non va bene per tutti
                        dateIsOk = false;
                         }
                    }
                }
                if(dateIsOk){
                    ///se i primi requisiti sono rispettati, dobbiamo controllare se i prof sono disponibili e se nei due giorni precedenti non ci siano corsi dello stesso corso di studio e semestre
                    std::vector<int> startHourPerCourse;
                    ///ciclo sull'appello del codCOurse in posizione indexExam di _allExamAppealsToDo e i suoi raggruppati se il primo controllo sulle date è andato bene
                    for(int i = 0; i < coursesToConsiderInThisLoop.size(); i++){
                        Course courseToConsider = coursesToConsiderInThisLoop[i];
                        ///controllo che i prof siano disponibili e che nei due giorni precedenti non ci siano esami già assegnati con stesso corso di studio e stesso anno dell'esame da assegnare
                        int startExamHour = checkIfProfsAvailableAndGapSameSemesterCourses(courseToConsider,currentExamDay,profs, relaxPar,getSemester(sessName));
                        startHourPerCourse.push_back(startExamHour);
                    }
                    ///controlliamo che non ci siano -1 nel vettore
                    bool againOk = checkHours(startHourPerCourse);
                    if(againOk){
                        pop = true;
                        ///allora posso assegnare i corsi (facendo pop da _allExamAppealsToDo!!!)
                        for(int i = 0; i < coursesToConsiderInThisLoop.size(); i++) {
                            Course& courseToConsider = courses.at(coursesToConsiderInThisLoop[i].getId());
                            assignTheExamToThisExamDay(startHourPerCourse[i], currentExamDay, profs, courseToConsider, sessName, _allExamAppealsToDo);
                            _sysLog.generateWarnings(coursesToConsiderInThisLoop,relaxPar,gapAppeals,_acYear);
                        }
                        ///check terminazione funzione
                        if (_allExamAppealsToDo.empty()) {
                            //se finiti gli appelli, esco
                            continueLoop = false;
                        }
                    }
                }
            }
        }
    }
    ///devo controllare che tutti gli appelli siano stati inseriti
    if(_allExamAppealsToDo.empty())
        return true;
    else
        return false;

}

///genera i file di output per le sessioni
void SessionYear::generateOutputFiles(std::string& outputFileName,int session,std::map<std::string, Course>& courses) {
    std::stringstream ssFout;
    std::string key = _sessionNames[session-1];
    ssFout<<outputFileName.substr(0,19);
    if(key == "winter")
        ssFout<<"_s1.txt";
    else if(key == "summer")
        ssFout<<"_s2.txt";
    else if(key == "autumn")
        ssFout<<"_s3.txt";

    std::fstream outputSession;
    outputSession.open(ssFout.str(), std::fstream::out | std::fstream::trunc);

    if(!outputSession.is_open()){
        throw std::exception();
    }
    Date dayOne=_yearSessions.at(key).startDate;
    Date lastDay=_yearSessions.at(key).endDate;
    bool continueLoop = true;
    for (auto iterDateCalendar = _yearCalendar.find(dayOne.toString()); continueLoop ; iterDateCalendar++) {
        ///giorno della sessione
        if (iterDateCalendar->first == lastDay.toString())
            continueLoop = false;
        ExamDay examDay = iterDateCalendar->second;
        std::string day = iterDateCalendar->first;
        if (examDay.allSLotsAreEmpty() == false) {//se tutti gli slot sono vuoti vuol dire che non ho assegnato alcun esame in quella data, non lo stampo
            outputSession << day << std::endl;
            ///prendo un vettore di stringhe, dell'intera giornata; una (stringa/riga) per ogni slot
            std::vector<std::string> allSlots = examDay.getSlotsToString();
            for (int i = 0; i < allSlots.size(); i++) {
                outputSession << allSlots[i] << std::endl;
            }
        }
    }
    outputSession.close();
}

///prende tutti gli appelli da fare nella sessione sessName
std::vector<std::string> SessionYear::getAllExamAppealsToDo(std::string sessName, std::map<std::string, Course>& courses) {
    std::vector<std::string> allExamAppealsToDo;
    int semesterOfThisSession = getSemester(sessName); //primo semestre = winter, sec sem = summer, (terzo semestre) = autumn
    for(auto iterCourse = courses.begin(); iterCourse!=courses.end(); iterCourse++) {
        int semester;
        bool isActive;
        //se non trovo l'anno specifico prendo l'ultimo
        SpecificYearCourse specificYY = iterCourse->second.getThisYearCourse(_acYear);

        semester = specificYY.getSemester();
        isActive = specificYY.getisActive();
        int acYearOff = Parse::getAcStartYear(specificYY.getAcYearOff());
        ///controllo se è un corso di questo semestre ed è ATTIVO!!!!!!!!!!
        if (semester == semesterOfThisSession && isActive) {
            ///devo fare due appelli se i semestri sono uguali!!!!
            allExamAppealsToDo.push_back(iterCourse->first);
            allExamAppealsToDo.push_back(iterCourse->first);
        } else if (semester == semesterOfThisSession && _acYear<acYearOff){
            /// se stesso semestre ma spento nel futuro
            /// allora ancora attivo per questo acYear
            allExamAppealsToDo.push_back(iterCourse->first);
            allExamAppealsToDo.push_back(iterCourse->first);
        } else {
            ///altrimenti solo uno...
            allExamAppealsToDo.push_back(iterCourse->first);
        }


    }
    return allExamAppealsToDo;
}

///prende il semstre
int SessionYear::getSemester(std::string sessName) {
    if(sessName == "winter")
        return 1;
    else if(sessName == "summer")
        return 2;
    else if(sessName == "autumn")
        return 3;
    return -1;
}

///controlla che non ci siano -1 nel vettore. Serve per capire se l'algoritmo ha trovato un buco in seguito a tutte le considerazioni da fare
bool SessionYear::checkHours(std::vector<int>& input) {
    for(int i = 0; i<input.size(); i++){
        if(input[i] == -1)
            return false;
    }
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

///prende i corsi raggruppati del corso su cui effettuo la funzione
std::vector<std::string> SessionYear::getGroupedCourses(const std::map<std::string, Course>& courses, std::string idCourseSelected) {
    ///prendo il corso considerato
    Course course = courses.at(idCourseSelected);
    ///prendo corso di questo giro + i suoi esami raggruppati
    SpecificYearCourse sp = course.getThisYearCourse(_acYear);
    ///prendo corsi raggruppati
    std::vector<std::string> groupedCourses = sp.getIdGroupedCourses();
    ///inserisco il corso selezionato inizialmente
    groupedCourses.insert(groupedCourses.begin(),idCourseSelected);
    return groupedCourses;
}

///valuta se la data è accettabile per quell'esame
bool SessionYear::dateIsOK(Date& newDate,const Course& course, std::string& sessName, int gapAppeals) {
    SpecificYearCourse sp = course.getThisYearCourse(getAcYear());//corso per un anno specifico
    ///controllo sulla data della sessione
    bool iCanBeAssigneToFirstTwoWeekOfExamSession = sp.canIBeAssigneToFirstTwoWeekOfExamSession(this->getSemester(sessName));//se stesso semestre della sessione ed è attivo
    if(iCanBeAssigneToFirstTwoWeekOfExamSession == false){
        //se non stesso semestre della sessione o spento non posso assegnarlo alle prime due settimane della sessione ma devo iniziare dalla terza in poi
        //quindi controllo se il giorno analizzato è delle prime due settimane(primi 14 giorni); in quel caso la data non va bene
        session thisSession = _yearSessions.at(sessName);//prendo il periodo della sessione
        Date startDate = thisSession.startDate;//inizio della sessione
        /// prime due settimane ok?
        if(startDate.whatIsTheGap(newDate) < 14){
            return false;

        }
    }
    //stesso semestre della sessione ed attivo o siamo oltre i 14 giorni(quindi esame semestre diverso, spento, entrambi o secondo appello di uno dello stesso semestre della sessione ed attivo)
    ///continuo con il controllo se secondo appello
    int howManyTimesIAmAssignedAlreadyInThisSession = sp.amIAssignedAlreadyInThisSession(this->getSemester(sessName));
    if(howManyTimesIAmAssignedAlreadyInThisSession == 1){//sarà un secondo appello
        ///se è stato già assegnato allora devo controllare le due settimane di scarto a meno di relaxPar
        /// se relaxPar>=2 ho il loop a monte che mi chiama la generateSession con un gap progressivamente più piccolo
        Date lastDateAssignation = sp.lastDateAssignationInGivenSession(this->getSemester(sessName));//data primo appello
        if(lastDateAssignation.whatIsTheGap(newDate) < gapAppeals){//dal primo appello sono passati 14 giorni?
            return false;
        }
        else
            return true;
    }

        ///non è stato assegnato già, quindi ok cosi
        return true;
}

/// controlla se i prof sono disponibili e se l'esame da assegnare è dello stesso corso di studi e dello stesso anno di un altro, nel range di due giorni precedenti alla data controllata
int SessionYear::checkIfProfsAvailableAndGapSameSemesterCourses(Course& course, Date& currentExamDay, std::map<int, Professor>& profs,int relaxPar, int session) {
    Exam examToAssign = course.getExamSpecificYear(_acYear);//tempi, aule o lab, modalità
    int numSlots = examToAssign.howManySlots();//numero di slot che servono per l'esame
    return isPossibleToAssignThisExam(course,currentExamDay,profs,numSlots,relaxPar, session);//ora di inizio dello slot per l'esame
}

///controlla se può e dove può essere inserito l'esame ed eventualmente prende l'ora di inizio
int SessionYear::isPossibleToAssignThisExam(Course course,Date currentExamDay,std::map<int, Professor>& profs,int numSlot , int relaxPar, int session) {
    ///controlliamo che il corso da inserire non abbia in un range di due giorni precedenti alla data analizzata un esame dello stesso corso di studi e dello stesso anno
    ///rilassamento del range da uno a due giorni: faccio il controllo dello stesso corso di studio e dello stesso anno solo se il vincolo non è rilassato
    if(relaxPar<1){
        //prendo primo giorno della sessione
        Date dayOne=_yearSessions.at(_sessionNames[session-1]).startDate;
        // controllo il gap dal dayOne al currentDay
        // se = 1 controllo solo il giorno prima
        // se = 0 non devo controllare
        int pos = dayOne.whatIsTheGap(currentExamDay);
        auto iterCalendar=_yearCalendar.find(currentExamDay.toString());
        //se sono ad un giorno successivo al terzo giorno della sessione devo controllare solo i due giorni precedenti
        if(pos>2)
        pos=2;
        //per tre volte
        for (; pos>0; pos--){
                ExamDay dayBefore = iterCalendar->second;//al primo ciclo prendo l'examday precedente alla data in cui voglio inserire l'esame. Al secondo ciclo prendo l'examDay di due giorni prima
                bool same = dayBefore.sameStudyCourseAndYear(course, _acYear);
                iterCalendar--;
                if (same)
                    return -1;
                //se sameStudyAndYear ha trovato un esame già assegnato
                //che appartenga allo stesso corso di studi e allo stesso anno da assegnare
        }
    }
    ///controlliamo se ci sono slot liberi ed eventualmente se i prof in quegli slot sono liberi
    ExamDay examCurrentDay = _yearCalendar.at(currentExamDay.toString());
    int start = examCurrentDay.isPossibleToAssignThisExamToProf(course, profs, numSlot, relaxPar);

    return start;
}

///assegna l'esame nella data considerata
void SessionYear::assignTheExamToThisExamDay(int startExamHour, Date& currentExamDay, std::map<int, Professor> & profs, Course &course, std::string sessName, std::vector<std::string>& allExamAppealsToDo) {
    Exam examToAssign = course.getExamSpecificYear(_acYear);//tempi, aule o lab, modalità
    int numSlots = examToAssign.howManySlots();//numero di slot che servono per l'esame

    SpecificYearCourse& specificYY = course.getThisYearCourseReference(getAcYear());//corso per un anno specifico

    ///aggiorno strutture dati degli esami dell'anno specifico
    specificYY.assignExamInThisSpecificYearCourse(currentExamDay, getSemester(sessName));
    ///aggiungo l'esame a quelli che i prof devono fare
    _yearCalendar.at(currentExamDay.toString()).assignExamToProf(profs,course,startExamHour,numSlots);
    //aggiungo l'esame al calendario della sessione
    _yearCalendar.at(currentExamDay.toString()).assignExamToExamDay(startExamHour,course,numSlots);
    ///elimino appello programmato da vettore di appelli da programmare
    std::string codCurrentCourse=course.getId();
    popAppealFromVector(allExamAppealsToDo, codCurrentCourse);
}

///elimina dal vettore degli esami da effettuare l'esame già assegnato
void SessionYear::popAppealFromVector(std::vector<std::string>& allExamAppealsToDo, std::string codExam) {
    ///se questo appello è stato assegnato, non lo dobbiamo più considerare!!
    auto pos = find(allExamAppealsToDo.begin(),allExamAppealsToDo.end(),codExam);
    allExamAppealsToDo.erase(pos);
}

///ritorna se il periodo delle sessioni è vuoto
bool SessionYear::sessionsPeriodIsEmpty() {
    return _yearSessions.empty();
}

std::ostream &operator<<(std::ostream &sessions, const SessionYear &s) {
    sessions << s.getAcYear() << "-" << s.getAcYear() + 1 << ";"
             << s.getSessions();//aaaa-aaaa ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg
    return sessions;
}









