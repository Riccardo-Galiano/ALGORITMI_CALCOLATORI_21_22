//
// Created by lucam on 04/12/2021.
//

#include <algorithm>
#include <fstream>
#include "SessionYear.h"
#include "Parse.hpp"
#include "ExamDay.h"
#include "StudyCourse.h"
#include "DbException.h"
#include "University.h"



///costruttore
SessionYear::SessionYear(std::string &acYear, std::string &winterSession, std::string &summerSession,
                         std::string &autumnSession, std::string output_file_name) : _sysLog(output_file_name) {
    std::string error;
    bool isOk = true;
    _acYear = Parse::getAcStartYear(acYear);
    _sessionNames.emplace_back("winter");
    _sessionNames.emplace_back("summer");
    _sessionNames.emplace_back("autumn");
    _winter = false;
    _summer = false;
    _autumn = false;
    ///aggiungo le date della sessione in base all'anno e alla sessione
    try {
        this->addSession(acYear, winterSession, _sessionNames[0]);
    } catch (std::exception &err) {
        error.append(err.what());
        isOk = false;
    }
    try {
        this->addSession(acYear, summerSession, _sessionNames[1]);
    } catch (std::exception &err) {
        error.append(err.what());
        isOk = false;
    }
    try {
        this->addSession(acYear, autumnSession, _sessionNames[2]);
    } catch (std::exception &err) {
        error.append(err.what());
        isOk = false;
    }
    if (isOk == false)
        throw std::invalid_argument(error);
}


///aggiunge il periodo delle sessioni
void SessionYear::addSession(std::string &acYear, std::string &sessionDates, std::string &name) {
    _acYear = Parse::getAcStartYear(acYear);
    ///vettore di Date in cui metterò la data di inizio e di fine
    std::vector<Date> dates = Parse::getDates(sessionDates);
    if (name == _sessionNames[2]) {//autunnale
        ///controllo se il periodo di settimane per la sessione autunnale sia esatto
        if (!dates[0].checkGapGiven(4, dates[1]))
            throw std::invalid_argument("la durata della sessione autunnale e' diversa da 4 settimane \n");
    } else if (!dates[0].checkGapGiven(6,
                                       dates[1])) {//controllo se il periodo di settimane per le sessione invernale/estiva sia esatto
        if (name == _sessionNames[1])
            throw std::invalid_argument("la durata della sessione estiva e' diversa da 6 settimane \n");
        else if (name == _sessionNames[0])
            throw std::invalid_argument("la durata della sessione invernale e' diversa da 6 settimane \n");
    }

    session s{name, dates[0], dates[1]};//salvo le info della sessione nome, data di inizio e data di fine

    _yearSessions.insert(std::pair<std::string, session>(name, s));//sessione Invernale, Estiva, Autunnale. Salvo la sessione
    //setto il calendario con le date delle sessioni
    this->setCaldendar(dates);

}

///creo il calendario con i giorni delle sessioni, in cui ogni giorno sarà un examDay
void SessionYear::setCaldendar(std::vector<Date> dates) {
    ///per ogni giorno della sessione setto la mappa YearCalendar con key la data sottoforma di stringa e un oggetto examDay come value
    for (Date d = dates[0]; !d.isEqual(dates[1]++); d = d++) {
        ExamDay examDay(d);
        _yearCalendar.insert(std::pair<std::string, ExamDay>(d.toString(), examDay));
    }
}

///genera le sessioni dell'anno accademico considerato
bool SessionYear::generateNewYearSession(std::string &fout, int relaxPar, University &myUniversity) {
    int gapAppealsSameCourse = _gapAppealsSameCourse;
    bool result = false;
    bool exitloop = false;
    ///get oggetti per COPIA  e li andiamo a modificare nei passi successivi
    std::map<int, Professor> professors = myUniversity.getProfessors();
    std::map<int, Classroom> allUniversityClassrooms = myUniversity.getClassrooms();
    std::map<std::string, Course> courses = myUniversity.getCourses();
    bool sameDayTwoAppealsSameExamAtLeastSixHours = false;
    bool firstIteration = true;
    ///eseguo il ciclo finchè posso rilassare ulteriormente il vincolo dei giorni tra appelli di uno stesso esame (14 giorni,13,12...)
    for (; gapAppealsSameCourse >= 0 && !exitloop; gapAppealsSameCourse--) {
        ///cerco prima di fare la sessione con relaxpar = 0 && cerco di soddisfare min distance
        if (firstIteration) {
            _winter = generateThisSession("winter", courses, professors, allUniversityClassrooms, relaxPar,
                                         gapAppealsSameCourse, sameDayTwoAppealsSameExamAtLeastSixHours, true);
            _summer = generateThisSession("summer", courses, professors, allUniversityClassrooms, relaxPar,
                                         gapAppealsSameCourse, sameDayTwoAppealsSameExamAtLeastSixHours, true);
            _autumn = generateThisSession("autumn", courses, professors, allUniversityClassrooms, relaxPar,
                                         gapAppealsSameCourse, sameDayTwoAppealsSameExamAtLeastSixHours, true);
        } else {
            ///posso rilassare il vincolo del gap tra appelli di uno stesso esame fino a 6 ore tra i due appelli
            if (gapAppealsSameCourse == 0) {
                sameDayTwoAppealsSameExamAtLeastSixHours = true;
            }
            ///generare sessione invernale
            if (_winter == false)
                _winter = generateThisSession("winter", courses, professors, allUniversityClassrooms, relaxPar,
                                             gapAppealsSameCourse, sameDayTwoAppealsSameExamAtLeastSixHours, false);
            ///generare sessione estiva
            if (_summer == false)
                _summer = generateThisSession("summer", courses, professors, allUniversityClassrooms, relaxPar,
                                             gapAppealsSameCourse, sameDayTwoAppealsSameExamAtLeastSixHours, false);
            ///generare sessione autunnale
            if (_autumn == false)
                _autumn = generateThisSession("autumn", courses, professors, allUniversityClassrooms, relaxPar,
                                             gapAppealsSameCourse, sameDayTwoAppealsSameExamAtLeastSixHours, false);
            ///se le tre sessioni sono state generate passo all'output su file
        }

        if (_winter && _summer && _autumn) {
            bool requestChanges = false;
            generateOutputFilesSession(fout, 1, courses, requestChanges);
            generateOutputFilesSession(fout, 2, courses, requestChanges);
            generateOutputFilesSession(fout, 3, courses, requestChanges);
            allExamAppealsWrite(courses);
            ///stampa warnigs
            if (relaxPar != 0 || (relaxPar == 0 && firstIteration == false))
                _sysLog.writeWarnings(fout);

            result = true;
            //posso uscire dal loop, non aspetto che il vincolo sia meno di 14 giorni
            exitloop = true;
        } else {
            if (firstIteration) {
                firstIteration = false;
                gapAppealsSameCourse++;
            } else {
                result = false;
                if (relaxPar < 2 ) {
                    ///se non ci sono vincoli rilassati oppure relaxPar=1 esco
                    exitloop = true;
                }else if(relaxPar == 2){
                    //il vincolo di 0 giorni va mantenuto dopo relaxPar == 2 quindi ultima prova si ha rilassando il vincolo delle indisponibilità dei prof
                    // a 0 giorni tra un appello e l'altro
                    gapAppealsSameCourse++;
                    relaxPar++;
                }
                ///else -> se relaxPar =2 opp =3, allora deve continuare a ciclare fino a quando può
            }
        }
    }
    if (result) {
        ///set oggetti modificati con successo in university
        myUniversity.setClassrooms(allUniversityClassrooms);
        myUniversity.setProfessors(professors);
        myUniversity.setCourses(courses);
    }
    return result;
}


void SessionYear::allExamAppealsWrite(std::map<std::string, Course> &courses) {
    std::fstream fout;
    fout.open("allExamAppealsDb.txt", std::fstream::out | std::fstream::trunc);
    if (!fout.is_open()) {
        throw DbException("file allExamAppealsDb.txt non esistente");
    }
    for (auto iterCourse = courses.begin(); iterCourse != courses.end(); iterCourse++) {
        std::vector<std::string> allAppealsPerCourses = iterCourse->second.getAcYearAppeals();
        for (int i = 0; i < allAppealsPerCourses.size(); i++) {
            fout << allAppealsPerCourses[i] << std::endl;
        }
    }
    fout.close();
}

///programma una sessione in particolare
bool SessionYear::generateThisSession(std::string sessName, std::map<std::string, Course> &courses,
                                      std::map<int, Professor> &profs,
                                      std::map<int, Classroom> &allUniversityClassrooms, int relaxPar, int gapAppeals,
                                      bool sixHours, bool tryToSatisfyProfsMinDistance) {

    ///prendiamo l'intervallo di date della sessione richiesta su cui dobbiamo esguire il ciclo
    session thisSession = _yearSessions.at(sessName);
    Date startDate = thisSession.startDate;
    Date endDate = thisSession.endDate;
    bool continueLoop = true;
    bool pop = false;
    bool requestChanges = false;
    std::string error;
    bool classRoomError = false;

    ///raggruppiamo tutti gli esami specifici di quest'anno
    if (relaxPar == 0 && tryToSatisfyProfsMinDistance == true) {
        std::vector<std::string> allExamAppealsToDo = getAllExamAppealsToDo(sessName, courses);
        _allExamAppealsToDo.insert(std::pair<std::string, std::vector<std::string>>(sessName,
                                                                                    allExamAppealsToDo)); //contiene le stringhe dei codici esame per OGNI appello
    }
    ///else -> nulla, _allExamAppealsToDo già caricato
    ///cicliamo su ogni data della sessione per organizzare le date degli appelli
    for (Date currentExamDay(startDate.getYear(), startDate.getMonth(), startDate.getDay());
         continueLoop && currentExamDay.isEqual(endDate++) == false; currentExamDay = currentExamDay++) {
        bool existRooms = true;
        //se non è domenica
        if (!(currentExamDay.getWeekDay() == "Sunday")) {
            ///ciclo su tutti gli appelli da assegnare
            for (int indexExam = 0; indexExam < _allExamAppealsToDo.at(sessName).size() && continueLoop; indexExam++) {
                //il controllo su pop mi serve perchè se dovessi togliere degli elementi dal vettore potrei rischiare di non contare certi esami che slitterebbero ad una posizione indexExam ornai passata.
                // Quindi per sicurezza, se dovessi togliere degli elementi dal vettore, azzero l'indice e ricomincio da zero, i secondi appelli verranno saltati alla fine del primo controllo
                if (pop) {
                    indexExam = 0;
                    pop = false;
                }
                ///esami raggruppati da considerare in questo giro
                std::string codCurrentAppeal = _allExamAppealsToDo.at(sessName)[indexExam];
                std::vector<std::string> coursesGrouped = getGroupedCourses(courses, codCurrentAppeal);
                std::vector<Course> coursesToConsiderInThisLoop;
                for (int i = 0; i < coursesGrouped.size(); i++) {
                    coursesToConsiderInThisLoop.push_back(courses.at(coursesGrouped[i]));
                }

                ///dobbiamo verificare che la data corrente sia possibile per tutti gli esami da inserire in questo giro
                bool dateIsOk = true;
                if (sessName != "autumn") {
                    for (int i = 0; i < coursesToConsiderInThisLoop.size() && dateIsOk; i++) {
                        Course courseToConsider = coursesToConsiderInThisLoop[i];
                        //la data analizzata rispetta i primi requisiti per l'assegnazione di un esame(specificYearCourse)
                        //requisiti: se non stesso semestre o non attivo o entrambi o secondi appelli va oltre i primi 14 giorni della sessione; se secondo appello va 14 giorni oltre il primo appello
                        if (dateIsOK(currentExamDay, courseToConsider, sessName, gapAppeals, requestChanges,
                                     tryToSatisfyProfsMinDistance) == false) {
                            //se non va bene per uno non va bene per tutti
                            dateIsOk = false;
                        }
                    }
                }
                if (dateIsOk) {
                    ///se i primi requisiti sono rispettati, dobbiamo controllare se i prof sono disponibili e se nei due giorni precedenti non ci siano corsi dello stesso corso di studio e semestre
                    int startHourPerGroupedCourses = -1;
                    std::map<std::string, std::vector<int>> roomsFoundedPerCourse;
                    bool continueLoopPerHourStart = true;
                    bool firstCourseOfThisLoop = true;
                    int startControlExamHourSlot = 8;


                    ///ciclo sull'appello del codCOurse in posizione indexExam di _allExamAppealsToDo e i suoi raggruppati se il primo controllo sulle date è andato bene
                    for (int i = 0; i < coursesToConsiderInThisLoop.size() && continueLoopPerHourStart; i++) {
                        Course courseToConsider = coursesToConsiderInThisLoop[i];
                        std::vector<int> roomsFounded;
                        SpecificYearCourse sp = courseToConsider.getThisYearCourse(getAcYear());
                        int numAppealsAssigned = sp.amIAssignedAlreadyInThisSession(this->getSemester(sessName));
                        int endHourSlot = -1;

                        ///se primo corso del loop indico che è il primo e faccio iniziare il controllo
                        if (i == 0)
                            firstCourseOfThisLoop = true;

                        if (numAppealsAssigned == 1) {//sarà un secondo appello
                            Date lastDateAssignation = sp.lastDateAssignationInGivenSession(
                                    this->getSemester(sessName));//data primo appello
                            ///se è stato segnato il vincolo delle sei ore e la data che sto controllando è la stessa del primo appello
                            ///mi serve sapere l'ultimo slot orario occupato dal primo appello
                            if (sixHours && lastDateAssignation.isEqual(currentExamDay)) {
                                endHourSlot = _yearCalendar.at(
                                        lastDateAssignation.toString()).getEndHourOfThisCourseExam(courseToConsider);
                            }
                        }
                        ///controllo che i prof siano disponibili e che nei due giorni precedenti non ci siano esami già assegnati con stesso corso di studio e stesso anno dell'esame da assegnare
                        /// controllo disponibilità aule e vincolo 6 ore stesso esame stesso giorno
                        int startExamHour;
                        try {
                            startExamHour = checkIfProfsAvailableAndGapSameSemesterCourses(courseToConsider,
                                                                                           currentExamDay, profs,
                                                                                           allUniversityClassrooms,
                                                                                           relaxPar,
                                                                                           getSemester(sessName),
                                                                                           roomsFounded, endHourSlot,
                                                                                           firstCourseOfThisLoop,
                                                                                           startControlExamHourSlot,
                                                                                           requestChanges);
                        } catch (std::invalid_argument &err) {
                            ///dovrò segnalare che non posso trovare le aule e quindi assegnare l'esame per questo esame e i suoi raggruppati
                            error.append(err.what());
                            classRoomError = true;
                            existRooms = false;
                        }
                        if (existRooms) {
                            if (startExamHour == -1 && firstCourseOfThisLoop)
                                //per il primo corso non ho trovato alcuno slot in quel giorno, quindi esco dal loop
                                continueLoopPerHourStart = false;
                            else if (startExamHour == -1 && firstCourseOfThisLoop == false) {
                                //se non è il primo corso ma non trovo gli slot assegnati al primo faccio ripartire il ciclo ma ad uno slot successivo rispetto a quello assegnato al primo corso
                                i = 0;
                                roomsFoundedPerCourse.erase(roomsFoundedPerCourse.begin(), roomsFoundedPerCourse.end());
                                _yearCalendar.at(currentExamDay.toString()).eraseTempGroupedCourseClassrooms();
                                if (startControlExamHourSlot < 18) {
                                    ///controllo finchè ho slot in quel giorno
                                    startControlExamHourSlot = startControlExamHourSlot + 2;
                                 } else {
                                    //sono arrivato a fine giornata, esco
                                    continueLoopPerHourStart = false;
                                }
                            } else {
                                //se trovo gli slot assegnati segno come primo slot di controllo l'ora trovata per il corso
                                startControlExamHourSlot = startExamHour;
                                roomsFoundedPerCourse.insert(std::pair<std::string, std::vector<int>>(courseToConsider.getId(), roomsFounded));
                                if (i == coursesToConsiderInThisLoop.size() - 1)
                                    //se anche l'ultimo raggruppato ha trovato disponibilità per gli stessi slot degli altri
                                    startHourPerGroupedCourses = startExamHour;
                            }
                            firstCourseOfThisLoop = false;

                            bool againOk = startHourPerGroupedCourses != -1;
                            if (againOk) {
                                ///pulisco il vettore di aule temporaneo per i raggruppati
                                _yearCalendar.at(currentExamDay.toString()).eraseTempGroupedCourseClassrooms();
                                pop = true;
                                std::vector<std::pair<std::string,int>> gapProfsNoRespect;
                                ///allora posso assegnare i corsi (facendo pop da _allExamAppealsToDo!!!)
                                for (int i = 0; i < coursesToConsiderInThisLoop.size(); i++) {
                                    std::string idCourse = coursesToConsiderInThisLoop[i].getId();
                                    Course &courseToConsider = courses.at(idCourse);
                                    SpecificYearCourse sp = courseToConsider.getThisYearCourse(getAcYear());
                                    int numAppealYear = sp.getNumNextAppeal();
                                    std::vector<int> rooms = roomsFoundedPerCourse.at(idCourse);

                                    assignTheExamToThisExamDay(startHourPerGroupedCourses, currentExamDay, profs,
                                                               allUniversityClassrooms, courseToConsider, sessName,
                                                               _allExamAppealsToDo.at(sessName), rooms, requestChanges,
                                                               numAppealYear);

                                    std::vector<int> allProfsMatrThisCourse = sp.getAllProfMatr();
                                    if(tryToSatisfyProfsMinDistance == false){
                                        allGapProfsNoRespect(gapProfsNoRespect, allProfsMatrThisCourse,idCourse);
                                    }
                                }

                                _sysLog.generateWarnings(coursesToConsiderInThisLoop, relaxPar, gapAppeals, _acYear,
                                                         gapProfsNoRespect, getSemester(sessName));
                                ///check terminazione funzione
                                if (_allExamAppealsToDo.at(sessName).empty()) {
                                    //se finiti gli appelli, esco
                                    continueLoop = false;
                                }
                            }

                            } else {
                            ///elimino il corso e i raggruppati dal vettore di esami da assegnare
                            for (int j = 0; j < coursesToConsiderInThisLoop.size(); i++) {
                                ///va fatto anche per i doppioni
                                popAppealFromVector(_allExamAppealsToDo.at(sessName), coursesToConsiderInThisLoop[j].getId());
                            }
                            pop = true;
                            continueLoopPerHourStart == false;
                        }
                    }
               }
             }
        }
    }
    if (classRoomError == true) {
        throw std::logic_error(error);
    }
    ///devo controllare che tutti gli appelli siano stati inseriti
    if (_allExamAppealsToDo.at(sessName).empty()) {
        return true;
    } else
        return false;

}

///genera i file di output per le sessioni
void SessionYear::generateOutputFilesSession(std::string &outputFileName, int session,
                                             const std::map<std::string, Course> &courses, bool requestChanges) {
    std::stringstream ssFout;
    std::string key = _sessionNames[session - 1];

    std::string realFileName;
    int numSession;
    if (requestChanges == false) {
        std::vector<std::string> token = Parse::splittedLine(outputFileName, '.');
        ssFout << token[0];
        if (key == "winter") {
            ssFout << "_s1";
            numSession = 1;
        } else if (key == "summer") {
            ssFout << "_s2";
            numSession = 2;
        } else if (key == "autumn") {
            ssFout << "_s3";
            numSession = 3;
        }
        _fileNamePerAcSession.insert(std::pair<int, std::string>(numSession, ssFout.str()));
        ssFout << ".txt";
        realFileName = ssFout.str();
    } else {
        realFileName = outputFileName + ".txt";
    }
    std::fstream outputSession;
    outputSession.open(realFileName, std::fstream::out | std::fstream::trunc);
    if (!outputSession.is_open()) {
        throw std::exception();
    }
    Date dayOne = _yearSessions.at(key).startDate;
    Date lastDay = _yearSessions.at(key).endDate;
    bool continueLoop = true;

    for (auto iterDateCalendar = _yearCalendar.find(dayOne.toString()); continueLoop; iterDateCalendar++) {
        ///giorno della sessione
        if (iterDateCalendar->first == lastDay.toString())
            continueLoop = false;
        ExamDay examDay = iterDateCalendar->second;
        std::string day = iterDateCalendar->first;
        if (examDay.allSLotsAreEmpty() ==
            false) {//se tutti gli slot sono vuoti vuol dire che non ho assegnato alcun esame in quella data, non lo stampo
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
std::vector<std::string>
SessionYear::getAllExamAppealsToDo(std::string sessName, std::map<std::string, Course> &courses) {
    std::vector<std::string> allExamAppealsToDo;
    int semesterOfThisSession = getSemester(sessName); //primo semestre = winter, sec sem = summer, (terzo semestre) = autumn
    for (auto iterCourse = courses.begin(); iterCourse != courses.end(); iterCourse++) {
        int semester;
        bool isActive;
        //se non trovo l'anno specifico prendo l'ultimo
        SpecificYearCourse specificYY = iterCourse->second.getThisYearCourse(_acYear);

        semester = specificYY.getSemester();
        isActive = specificYY.getisActive();
        int acYearOff;
        if (isActive == false)
            acYearOff = Parse::getAcStartYear(specificYY.getAcYearOff());
        ///controllo se è un corso di questo semestre ed è ATTIVO!!!!!!!!!!
        if (semester == semesterOfThisSession && isActive) {
            ///devo fare due appelli se i semestri sono uguali!!!!
            allExamAppealsToDo.push_back(iterCourse->first);
            allExamAppealsToDo.push_back(iterCourse->first);
        } else if (semester == semesterOfThisSession && _acYear < acYearOff) {
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
    if (sessName == "winter")
        return 1;
    else if (sessName == "summer")
        return 2;
    else if (sessName == "autumn")
        return 3;
    return -1;
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
std::vector<std::string>
SessionYear::getGroupedCourses(const std::map<std::string, Course> &courses, std::string idCourseSelected) {
    ///prendo il corso considerato
    Course course = courses.at(idCourseSelected);
    ///prendo corso di questo giro + i suoi esami raggruppati
    SpecificYearCourse sp = course.getThisYearCourse(_acYear);
    ///prendo corsi raggruppati
    std::vector<std::string> groupedCourses = sp.getIdGroupedCourses();
    ///inserisco il corso selezionato inizialmente
    groupedCourses.insert(groupedCourses.begin(), idCourseSelected);
    return groupedCourses;
}

///valuta se la data è accettabile per quell'esame
bool SessionYear::dateIsOK(Date &newDate, const Course &course, std::string &sessName, int gapAppeals, bool requestChanges,
                      bool tryToSatisfyProfsMinDistance) {
    SpecificYearCourse sp = course.getThisYearCourse(getAcYear());//corso per un anno specifico
    std::vector<int> allProfsMatrThisCourse = sp.getAllProfMatr();
    std::string idCorso = course.getId();
    session thisSession = _yearSessions.at(sessName);//prendo il periodo della sessione
    Date startDate = thisSession.startDate;//inizio della sessione
    int howManyTimesIAmAssignedAlreadyInThisSession = sp.amIAssignedAlreadyInThisSession(this->getSemester(sessName));

    ///controllo sulla data della sessione
    bool iCanBeAssigneToFirstTwoWeekOfExamSession = sp.canIBeAssigneToFirstTwoWeekOfExamSession(
            this->getSemester(sessName));//se stesso semestre della sessione ed è attivo
    if (iCanBeAssigneToFirstTwoWeekOfExamSession == false) {
        //se non stesso semestre della sessione o spento non posso assegnarlo alle prime due settimane della sessione ma devo iniziare dalla terza in poi
        //quindi controllo se il giorno analizzato è delle prime due settimane(primi 14 giorni); in quel caso la data non va bene
        /// prime due settimane ok?
        if (startDate.whatIsTheGap(newDate) < 14) {
            if (requestChanges)
                throw std::invalid_argument("Esame non puo' essere inserito nei primi 14 giorni della sessione\n");
            else
                return false;
        }
    } else if (howManyTimesIAmAssignedAlreadyInThisSession == 0 && requestChanges ==
                                                                   false) {//se i canBeAssigneToFirstTwoWeekOfExamSession mi devo chiedere se è un primo appello
        ///se primo appello devo dimostrare che la data appartenga ai primi 14 giorni,
        if (startDate.whatIsTheGap(newDate) > 14) {
            //nel caso della setExam e quindi non requestChanges ritorno solo false
            return false;
        }
    } else if (howManyTimesIAmAssignedAlreadyInThisSession == 1 && requestChanges ==
                                                                   true) {//se i canBeAssigneToFirstTwoWeekOfExamSession mi devo chiedere se è un primo appello
        //l'altro appello
        Date lastDateAssignation = sp.lastDateAssignationInGivenSession(this->getSemester(sessName));
        bool second = isSecondAppeal(newDate, lastDateAssignation);
        ///se primo appello devo dimostrare che la data appartenga ai primi 14 giorni; se secondo appello niente, continuo i controlli successivamente
        if (second == false) {
            if (startDate.whatIsTheGap(newDate) > 14) {
                throw std::invalid_argument(
                        "Esame non puo' essere inserito dopo i primi 14 giorni della sessione essendo un primo appello\n");
            }
        }
    }

    //stesso semestre della sessione ed attivo o siamo oltre i 14 giorni(quindi esame semestre diverso, spento, entrambi o secondo appello di uno dello stesso semestre della sessione ed attivo)


    ///continuo con il controllo se esiste già un appello
    if (howManyTimesIAmAssignedAlreadyInThisSession == 1) {//sarà un secondo appello da assegnare
        ///se howManyTimes... = 1 controllo che new date sia maggiore o minore della data dell'altro appello (request changes)
        //se minore sono in presenza del cambiamento di un primo appello (mi interessa un algoritmo diverso per la requestChanges)
        //se maggiore sono in presenza del cambiamento di un secondo appello o comunque dell'assegnazione di un secondo appello ( algoritmo simile tra setExam e RequestChanges)

        bool second = true;
        //l'altro appello
        Date lastDateAssignation = sp.lastDateAssignationInGivenSession(this->getSemester(sessName));
        if (requestChanges == true) {
            second = isSecondAppeal(newDate, lastDateAssignation);
        }
        ///se è stato già assegnato allora devo controllare le due settimane di scarto a meno di relaxPar
        /// se relaxPar>=2 ho il loop a monte che mi chiama la generateSession con un gap progressivamente più piccolo
        if (second == true) {
            //sono in presenza dell'assegnazione o del cambiamento di un secondo appello
            if (lastDateAssignation.whatIsTheGap(newDate) < gapAppeals) {//dal primo appello sono passati 14 giorni?
                if (requestChanges == true) {
                    throw std::invalid_argument(
                            "Il gap di 14 giorni tra il nuovo secondo appello e il primo non rispettato\n ");
                } else
                    return false;
            }
        } else {
            //controllo gap tra nuovo primo appello e secondo
            if (newDate.whatIsTheGap(lastDateAssignation) < gapAppeals) {//dal primo appello sono passati 14 giorni?
                throw std::invalid_argument("Gap di 14 giorni tra nuovo primo appello e secondo non rispettato\n");
            }
        }
        ///cotrollo aggiuntivo se il gap deve essere più largo (profsGap)
        for (int i = 0; i < allProfsMatrThisCourse.size(); i++) {
            std::stringstream ss;
            std::string profId = Parse::setId('d',6,allProfsMatrThisCourse[i]);
            ss << profId << "_" << idCorso;
            std::string keyToSearchProfsGap = ss.str();
            if (_gapProfs.count(keyToSearchProfsGap) != 0) {
                int requiredGap = _gapProfs.at(keyToSearchProfsGap);
                if (requiredGap > 14) {
                    if (second == true) {
                        if (lastDateAssignation.whatIsTheGap(newDate) < requiredGap) { //dall'ultimo appello sono passati i giorni richiesti dal prof?
                            if (requestChanges == true) {
                                std::string settedId = Parse::setId('d', 6, allProfsMatrThisCourse[i]);
                                throw std::invalid_argument("Gap ulteriore del professore con matricola " + settedId +
                                                            " tra nuovo secondo appello e primo appello non rispettato\n");
                            } else {
                                if (tryToSatisfyProfsMinDistance) {
                                    return false;
                                }
                            }
                        }
                    } else {
                        if (newDate.whatIsTheGap(lastDateAssignation) <
                            requiredGap) { //dall'altro appello sono passati i giorni richiesti dal prof?
                            std::string settedId = Parse::setId('d', 6, allProfsMatrThisCourse[i]);
                            throw std::invalid_argument("Gap ulteriore del professore con matricola " + settedId +
                                                        " tra nuovo primo appello e secondo appello non rispettato\n");

                        }
                    }
                }
            }
        }
    }
    return true;
}

/// controlla se i prof sono disponibili e se l'esame da assegnare è dello stesso corso di studi e dello stesso anno di un altro, nel range di due giorni precedenti alla data controllata
int SessionYear::checkIfProfsAvailableAndGapSameSemesterCourses(Course &course, Date &currentExamDay,
                                                                std::map<int, Professor> &profs,
                                                                std::map<int, Classroom> &classrooms, int relaxPar,
                                                                int session, std::vector<int> &roomsFounded,
                                                                int endHourSlot, bool firstCourseOfThisLoop,
                                                                int startControlExamHourSlot, bool requestChanges) {
    Exam examToAssign = course.getExamSpecificYear(getAcYear());//tempi, aule o lab, modalità
    int numSlots = examToAssign.howManySlots();//numero di slot che servono per l'esame
    return isPossibleToAssignThisExam(course, currentExamDay, profs, classrooms, numSlots, relaxPar, session,
                                      roomsFounded, endHourSlot, firstCourseOfThisLoop, startControlExamHourSlot,
                                      requestChanges);//ora di inizio dello slot per l'esame
}

///controlla se può e dove può essere inserito l'esame ed eventualmente prende l'ora di inizio
int SessionYear::isPossibleToAssignThisExam(Course course, Date currentExamDay, std::map<int, Professor> &profs,
                                            std::map<int, Classroom> &classrooms, int numSlot, int relaxPar,
                                            int session, std::vector<int> &roomsFounded, int endHourSlot,
                                            bool firstCourseOfThisLoop, int startControlExamHourSlot,
                                            bool requestChanges) {
    ///controlliamo che il corso da inserire non abbia in un range di due giorni precedenti alla data analizzata un esame dello stesso corso di studi e dello stesso anno
    ///rilassamento del range da 2 a 0 giorni: faccio il controllo dello stesso corso di studio e dello stesso anno solo se il vincolo non è rilassato
    if (relaxPar < 1) {
        //prendo primo giorno della sessione
        Date dayOne = _yearSessions.at(_sessionNames[session - 1]).startDate;
        // controllo il gap dal dayOne al currentDay
        // se = 1 controllo solo il giorno prima
        // se = 0 non devo controllare
        int pos = dayOne.whatIsTheGap(currentExamDay);
        auto iterCalendar = _yearCalendar.find(currentExamDay.toString());

        //se sono ad un giorno successivo al terzo giorno della sessione devo controllare solo i due giorni precedenti
        if (pos > 2)
            pos = 2;
        //per tre volte
        for (; pos >= 0; pos--) {
            ExamDay dayBefore = iterCalendar->second;//al primo ciclo prendo l'examday corrente, al secondo ciclo l'examDay precedente alla data in cui voglio inserire l'esame. Al secondo ciclo prendo l'examDay di due giorni prima
            bool same = dayBefore.sameStudyCourseAndYear(course, _acYear);
            //iteratore al primo giorno della sessione
            auto iterCalendarFirstDaySession = _yearCalendar.find(dayOne.toString());
            //continuo a decrementare l'iteratore fino a che non sia uguale al primo giorno della sessione
            if (iterCalendar != iterCalendarFirstDaySession) {
                iterCalendar--;
            }
            if (same) {
                //se sameStudyCourseAndYear ha trovato un esame già assegnato
                //che appartenga allo stesso corso di studi e allo stesso anno da assegnare
                if (requestChanges == false) {
                    return -1;
                } else {
                    throw std::logic_error(
                            "Il gap dei due giorni tra corsi dello stesso corso di studio e dello stesso anno non e' rispettato\n");
                }
            }
        }
    }
    ///controlliamo se ci sono slot liberi ed eventualmente se i prof in quegli slot sono liberi
    ///controllo disponibilità di aule
    ExamDay &examCurrentDay = _yearCalendar.at(currentExamDay.toString());
    int start;
    start = examCurrentDay.isPossibleToAssignThisExamToProfs(course, profs, classrooms, numSlot, relaxPar,
                                                             roomsFounded, endHourSlot, firstCourseOfThisLoop,
                                                             startControlExamHourSlot);
    return start;
}

///assegna l'esame nella data considerata
void SessionYear::assignTheExamToThisExamDay(int startExamHour, Date &currentExamDay, std::map<int, Professor> &profs,
                                             std::map<int, Classroom> &allUniversityClassrooms, Course &course,
                                             std::string sessName, std::vector<std::string> &allExamAppealsToDo,
                                             std::vector<int> idRooms, bool requestChanges, int numAppealYear) {
    Exam examToAssign = course.getExamSpecificYear(getAcYear());//tempi, aule o lab, modalità
    int numSlots = examToAssign.howManySlots();//numero di slot che servono per l'esame
    SpecificYearCourse &specificYY = course.getThisYearCourseReference(getAcYear());//corso per un anno specifico

    ///aggiorno strutture dati degli esami dell'anno specifico
    specificYY.assignExamInThisSpecificYearCourse(currentExamDay, getSemester(sessName));
    specificYY.addClassroomsToAppeal(numAppealYear, idRooms);
    specificYY.addStartSlotToAppeal(numAppealYear, startExamHour);
    ///aggiungo l'esame a quelli che i prof devono fare
    _yearCalendar.at(currentExamDay.toString()).assignExamToProf(profs, course, startExamHour, numSlots);
    ///aggiungo l'esame al calendario della sessione
    _yearCalendar.at(currentExamDay.toString()).assignExamToExamDay(startExamHour, course, numSlots);
    ///aggiorno disponibilità aule
    for (int i = 0; i < idRooms.size(); i++) {
        allUniversityClassrooms.at(idRooms[i]).setDisavailability(currentExamDay, startExamHour, numSlots);
    }
    ///elimino appello programmato da vettore di appelli da programmare
    std::string codCurrentCourse = course.getId();
    ///AAA allExamAppealToDo sarebbe un variabile privata, secondo me non c'è bisogno di passarlo
    if (requestChanges == false)
        popAppealFromVector(allExamAppealsToDo, codCurrentCourse);
}

///elimina dal vettore degli esami da effettuare l'esame già assegnato
void SessionYear::popAppealFromVector(std::vector<std::string> &allExamAppealsToDo, std::string codExam) {
    ///se questo appello è stato assegnato, non lo dobbiamo più considerare!!
    auto pos = find(allExamAppealsToDo.begin(), allExamAppealsToDo.end(), codExam);
    allExamAppealsToDo.erase(pos);
}

///ritorna se il periodo delle sessioni è vuoto
bool SessionYear::sessionsPeriodIsEmpty() {
    return _yearSessions.empty();
}

void SessionYear::addProfGap(std::string &matr_idC, int gap) {
    _gapProfs.insert(std::pair<std::string, int>(matr_idC, gap));
   
}

std::vector<std::string> SessionYear::getProfsOfGapProfsString() {
    std::vector<std::string> gapProfsString;
    for (auto iterGap = _gapProfs.begin(); iterGap != _gapProfs.end(); iterGap++) {
        std::stringstream ss;
        std::vector<std::string> id_codCourse = Parse::splittedLine(iterGap->first, '_');
        ss << getAcYear() << "-" << getAcYear() + 1 << ";" << id_codCourse[0] << ";" << id_codCourse[1] << ";"
           << iterGap->second;
        gapProfsString.push_back(ss.str());
    }
    return gapProfsString;
}

void SessionYear::assignAppealsToCalendar(std::string appeal, int startSlotHour, Course &course, int numSlots) {
    _yearCalendar.at(appeal).assignExamToExamDay(startSlotHour, course, numSlots);
}

void SessionYear::removeThisAppealInfoFromCalendar(int numSlots, Date &date, int &startSlot,
                                                   std::string &idCourse) {
    _yearCalendar.at(date.toString()).removeThisAppealInfo(startSlot, numSlots, idCourse);

}

void SessionYear::tryToSetThisExamInThisSession(University &myUniversity, Course &courseToConsider,
                                                int numSession, int numAppeal, Date &tryDate) {
    ///get oggetti per COPIA  e li andiamo a modificare nei passi successivi
    std::map<int, Professor> professors = myUniversity.getProfessors();
    std::map<int, Classroom> allUniversityClassrooms = myUniversity.getClassrooms();
    std::map<std::string, Course> courses = myUniversity.getCourses();
    ///prendiamo l'intervallo di date della sessione richiesta
    session thisSession = _yearSessions.at(_sessionNames[numSession - 1]);
    Date startDate = thisSession.startDate;
    Date endDate = thisSession.endDate;
    int relaxPar = 0; //per compatibilità
    int endHourSlot = -1;
    int gapAppeals = 14; //per compatibilità
    bool requestChanges = true;

    ///check che lo shift sia possibile
    if (tryDate < startDate || tryDate > endDate)
        throw std::invalid_argument("La data non appartiene alla sessione\n");

    ///controllo che il primo appello rimanga il primo appello e che il secondo rimanga il secondo
    SpecificYearCourse specific = courseToConsider.getThisYearCourse(_acYear);
    int numAppealsAssigned = specific.amIAssignedAlreadyInThisSession(this->getSemester(_sessionNames[numSession - 1]));
    //se abbiamo più appelli per quella sessione
    //(diverso da zero perchè l'appello da riassegnare l'abbiamo tolto) quindi o rimarrà l'altro appello o non ci saranno appelli)
    if (numAppealsAssigned != 0) {
        //se primo appello
        Date dateOtherAppeal = specific.lastDateAssignationInGivenSession(numSession);
        if (numAppeal == 1) {
            //data scelta deve essere minore della data del secondo appello
            if (tryDate > dateOtherAppeal)
                throw std::logic_error(
                        "Data scelta per l'assegnazione del primo appello e' maggiore della data del secondo");
        } else {
            //data scelta deve essere minore della data del secondo appello
            if (tryDate < dateOtherAppeal)
                throw std::logic_error(
                        "Data scelta per l'assegnazione del secondo appello e' maggiore della data del primo");
        }
    }

    ///esami raggruppati da considerare in questo giro
    std::string codCurrentAppeal = courseToConsider.getId();
    std::vector<std::string> coursesGrouped = getGroupedCourses(courses, codCurrentAppeal);
    std::vector<Course> coursesToConsiderInThisLoop;
    ///prendo gli oggetti corso dei corsi raggruppati iù quello selezionato
    for (int i = 0; i < coursesGrouped.size(); i++) {
        coursesToConsiderInThisLoop.push_back(courses.at(coursesGrouped[i]));
        //per andrea: non manca codCurrentAppeal nel vettore?
        //la getGroupedCourses tiene conto anche di CodCurrentAppeal
    }

    ///dobbiamo verificare che la data corrente sia possibile per tutti gli esami da inserire in questo giro
    if (numSession != 3) {
        for (int i = 0; i < coursesToConsiderInThisLoop.size(); i++) {
            Course &courseToConsider = coursesToConsiderInThisLoop[i];
            //la data analizzata rispetta i primi requisiti per l'assegnazione di un esame(specificYearCourse)
            //requisiti: se non stesso semestre o non attivo o entrambi o secondi appelli va oltre i primi 14 giorni della sessione; se secondo appello va 14 giorni oltre il primo appello
            if (dateIsOK(tryDate, courseToConsider, _sessionNames[numSession - 1], gapAppeals, requestChanges, false) ==
                false) {
                //se non va bene per uno non va bene per tutti
                //genera un'eccezione
                //se un primo appello deve essere nei primi 14 giorni e lontano altri 14 dal secondo appello
                //se un secondo appello deve essere nei giorni successivi ai primi 14 giorni e a 14 dal primo appello
                //se non attivo deve essere nei giorni successivi ai primi 14 giorni
                //il gap ulteriore tra due appelli di uno o più prof
                throw std::logic_error("la data non rispetta i vincoli");
            }
        }
    }
    //se i primi vincoli rispettati controllo che vengano rispettati:
    //il range dei due giorni per corsi di uno stesso corso di studio e allo stesso anno
    //la disponibilità dei prof e la disponibilità delle aule
    int startHourPerGroupedCourses = -1;
    std::map<std::string, std::vector<int>> roomsFoundedPerCourse;
    bool continueLoopPerHourStart = true;
    bool firstCourseOfThisLoop = true;
    int startControlExamHourSlot = 8;

    //ciclo su tutti i corsi raggruppati a quello richiesto nel file ed esso stesso
    for (int i = 0; i < coursesToConsiderInThisLoop.size() && continueLoopPerHourStart; i++) {
        Course courseToConsider = coursesToConsiderInThisLoop[i];
        std::vector<int> roomsFounded;
        SpecificYearCourse sp = courseToConsider.getThisYearCourse(getAcYear());
        ///se primo corso del loop indico che è il primo e faccio iniziare il controllo
        if (i == 0)
            firstCourseOfThisLoop = true;
        ///controllo che i prof siano disponibili e che nei due giorni precedenti non ci siano esami già assegnati con stesso corso di studio e stesso anno dell'esame da assegnare
        int startExamHour = checkIfProfsAvailableAndGapSameSemesterCourses(courseToConsider, tryDate, professors,
                                                                           allUniversityClassrooms,
                                                                           relaxPar,
                                                                           getSemester(_sessionNames[numSession - 1]),
                                                                           roomsFounded, endHourSlot,
                                                                           firstCourseOfThisLoop,
                                                                           startControlExamHourSlot, requestChanges);
        if (startExamHour == -1 && firstCourseOfThisLoop)
            //per il primo corso non ho trovato alcuno slot in quel giorno, quindi esco dal loop
            continueLoopPerHourStart = false;
        else if (startExamHour == -1) {
            //se non è il primo corso ma non trovo gli slot assegnati al primo faccio ripartire il ciclo ma ad uno slot successivo rispetto a quello assegnato al primo corso
            i = 0;
            roomsFoundedPerCourse.erase(roomsFoundedPerCourse.begin(), roomsFoundedPerCourse.end());
            if (startControlExamHourSlot <= 18) {
                startControlExamHourSlot = startControlExamHourSlot + 2;
                _yearCalendar.at(tryDate.toString()).eraseTempGroupedCourseClassrooms();
            } else {
                continueLoopPerHourStart = false;
            }
        } else {
            //se trovo gli slot assegnati segno come primo slot di controllo l'ora trovata per il corso
            startControlExamHourSlot = startExamHour;
            if (i == coursesToConsiderInThisLoop.size() - 1)
                //se anche l'ultimo raggruppato ha trovato disponibilità per gli stessi slot degli altri
                startHourPerGroupedCourses = startExamHour;
        }
        firstCourseOfThisLoop = false;
        roomsFoundedPerCourse.insert(std::pair<std::string, std::vector<int>>(courseToConsider.getId(), roomsFounded));
    }
    ///pulisco il vettore di aule temporaneo per i raggruppati
    _yearCalendar.at(tryDate.toString()).eraseTempGroupedCourseClassrooms();
    bool dateIsOk = startHourPerGroupedCourses != -1;
    if (dateIsOk) {
        ///allora posso assegnare i corsi
        for (int i = 0; i < coursesToConsiderInThisLoop.size(); i++) {
            std::string idCourse = coursesToConsiderInThisLoop[i].getId();
            Course &courseToConsider = courses.at(idCourse);
            SpecificYearCourse sp = courseToConsider.getThisYearCourse(getAcYear());
            int numAppealYear = sp.getNumAppealFromNumSessNumAppealInSession(numSession, numAppeal);
            std::vector<int> rooms = roomsFoundedPerCourse.at(idCourse);
            assignTheExamToThisExamDay(startHourPerGroupedCourses, tryDate, professors, allUniversityClassrooms,
                                       courseToConsider, _sessionNames[numSession - 1], _allExamAppealsToDo.at(_sessionNames[numSession - 1]), rooms,
                                       requestChanges, numAppealYear);
            myUniversity.setClassrooms(allUniversityClassrooms);
            myUniversity.setProfessors(professors);
            myUniversity.setCourses(courses);
        }
    } else
        throw std::logic_error(
                "Nessuna possibile coincidenza tra disponibilita' dei prof considerati e aule per tutti gli slot di questo giorno\n");

}

bool SessionYear::fileNameIsEmpty() {
    return _fileNamePerAcSession.empty();
}

std::vector<std::string> SessionYear::getSessionAndFileName() {
    std::vector<std::string> fileNamePerSession;
    for (auto iterFileName = _fileNamePerAcSession.begin();
         iterFileName != _fileNamePerAcSession.end(); iterFileName++) {
        std::stringstream ss;
        ss << iterFileName->first << ";" << iterFileName->second;
        fileNamePerSession.push_back(ss.str());
    }
    return fileNamePerSession;
}

std::string SessionYear::getFileName(int numSession) {
    return _fileNamePerAcSession.at(numSession);
}

void SessionYear::setFileNamePerSession(int numSession, std::string fileName) {
    _fileNamePerAcSession.insert(std::pair<int, std::string>(numSession, fileName));
}

bool SessionYear::isSecondAppeal(Date newDate, Date lastDateAssignation) {
    if (newDate < lastDateAssignation) {
        //cambiamento di un primo appello
        return false;
    } else if (newDate > lastDateAssignation) {
        //cambiamento di un secondo appello
        return true;
    }
    return true;
}

void SessionYear::updateExamDayCourse(Course course, std::vector<Date> allAppealPerCourse) {
    for (int i = 0; i < allAppealPerCourse.size(); i++) {
        _yearCalendar.at(allAppealPerCourse[i].toString()).updateSlot(course);
    }
}

void SessionYear::controlSuccessivitySessionPeriod() {
    session winterSession = _yearSessions.at("winter");
    session summerSession = _yearSessions.at("summer");
    Date endWinterSession = winterSession.endDate;
    Date endSummerSession = summerSession.endDate;
    bool isOk = true;
    std::string error;

    for (auto iterSingleSession = _yearSessions.begin();
         iterSingleSession != _yearSessions.end(); iterSingleSession++) {

        if (iterSingleSession->first == "summer") {
            //controllo che la prima data della sessione estiva sia successiva all'ultima data della sessione invernale
            if (iterSingleSession->second.startDate < endWinterSession) {
                error.append("Prima data della sessione estiva precedente all'ultima data della sessione invernale\n");
                isOk = false;
            }
        }
        if (iterSingleSession->first == "autumn") {
            //controllo che la prima data della sessione autunnale sia successiva all'ultima data della sessione estiva e invernale
            if (iterSingleSession->second.startDate < endWinterSession) {
                error.append(
                        "Prima data della sessione autunnale precedente all'ultima data della sessione invernale\n");
                isOk = false;
            }
            if (iterSingleSession->second.startDate < endSummerSession) {
                error.append("Prima data della sessione autunnale precedente all'ultima data della sessione estiva\n");
                isOk = false;
            }
        }
    }
    if (isOk == false)
        throw std::invalid_argument(error);
}

void SessionYear::allGapProfsNoRespect(std::vector<std::pair<std::string, int>> &gapProfsNoRespect,
                                       std::vector<int> allProfsMatrThisCourse, std::string courseId) {
    for (int j = 0; j < allProfsMatrThisCourse.size(); j++) {
        std::stringstream ss;
        std::string profId = Parse::setId('d',6,allProfsMatrThisCourse[j]);
        ss << profId << "_" << courseId;
        std::string keyToSearchProfsGap = ss.str();
        if (_gapProfs.count(keyToSearchProfsGap) != 0) {
            int requiredGap = _gapProfs.at(keyToSearchProfsGap);
            if (requiredGap > 14) {
                gapProfsNoRespect.push_back(std::pair<std::string, int>(keyToSearchProfsGap, requiredGap));
            }
        }
    }
}


std::ostream &operator<<(std::ostream &sessions, const SessionYear &s) {
    sessions << s.getAcYear() << "-" << s.getAcYear() + 1 << ";"
             << s.getSessions();//aaaa-aaaa ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg ; aaaa-mm-gg_aaaa-mm-gg
    return sessions;
}

