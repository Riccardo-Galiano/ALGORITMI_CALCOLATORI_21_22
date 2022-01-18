//
// Created by Andrea on 06/12/2021.
//

#include <sstream>
#include "ExamDay.h"
#include "StudyCourse.h"
#include "Classroom.h"
#include <algorithm>
#include <iomanip>
#include <unordered_set>

//costructor
ExamDay::ExamDay(Date date) {
    _date = date;
    this->setSlot();
}

///se trova un buco disponibile ritorna l'ora di inizio, altrimenti ritorna -1
int ExamDay::isPossibleToAssignThisExamToProfs(Course course, std::map<int, Professor> &allUniversityProfs,
                                               std::map<int, Classroom> &allUniversityClassrooms, int numSlotsRequired,
                                               int relaxPar, std::vector<int>& idRoomsFounded, int endHourSlot,bool firstCourseOfThisLoop, int startControlExamHourSlot) {
    SpecificYearCourse specificCourse = course.getThisYearCourse( _date.getYear() - 1); //prendiamo corso specifico dell'anno di questo Exam Day (-1)
    std::string labOrClass = specificCourse.getPlaceExam();
    ///cerchiamo un numSlotsRequired vuoti (ovviamente consecutivi)
    //!!! esami non raggruppati dovrebbero poter essere messi nello stesso slot in base alla disponibilità di aule!!!!!!!!
    ///esami raggruppati vanno messi alla stessa ora
    /// (se ho già controllato il primo corso e ho trovato i prof disponibili e le aule disponibili devo far si che anche per i corsi raggruppati ci siano aule e i prof disponibili in quelle ore
    int startHourSlot = -1;
    int numSlotsFoundedSoFar = 0;
    int foundedStartHourSlot = -1;  //rimarrà -1 se non riesco a trovare nulla, altrimenti l'orario di inizio
    int numStuds = specificCourse.getTotStudentsExam(); //tot posti DA TROVARE
    int numLoop = (20/2)-(startControlExamHourSlot/2);
    bool classroomIsOk = false;
    bool thereAreEnoughClassrooms;
    ///ricerca slot->aule->profs
    bool toContinue = true;
    for (int i = 0; i < numLoop && foundedStartHourSlot == -1; i++) {
        toContinue=true;
        int slotHour = startControlExamHourSlot + 2 * i;
        ///se endHourSlot è diverso da -1 vuol dire che il corso che stiamo cercando di assegnare ha un primo appello e che
        ///il vincolo del gap tra appelli è rilassato fino a 6 ore di gap tra gli appelli; quindi inutile cercare in slot che
        ///non rispettino il gap delle 6 ore
        if (endHourSlot != -1) {
            if (endHourSlot - slotHour < 6) {
                toContinue = false;
            }
        }
        if (toContinue) {
            ///cerchiamo delle aule con la giusta capienza
            //se ho già effettuato la search dovrò rieffettuarla solo se per gli stessi slot i prof non sono disponibili
            if (!classroomIsOk)
                thereAreEnoughClassrooms = searchAvailableClassroomsInThisSlot(allUniversityClassrooms, numStuds,
                                                                               idRoomsFounded, slotHour,
                                                                               numSlotsRequired,labOrClass);

            ///controlliamo se abbiamo trovato ABBASTANZA posti
            if (thereAreEnoughClassrooms) {
                classroomIsOk = true;
                if (checkProfsAvaibility(specificCourse, allUniversityProfs, relaxPar, slotHour) == true)
                    numSlotsFoundedSoFar++;
                else {
                    classroomIsOk = false;
                    numSlotsFoundedSoFar = 0;
                }
                if (numSlotsFoundedSoFar == 1)
                    startHourSlot = slotHour; //se dovessi trovare il numero di slot necessari partirei da lì
                if (numSlotsFoundedSoFar == numSlotsRequired) {
                    ///se il numero di slot trovati coincide con il numero di slot necessari
                    foundedStartHourSlot = startHourSlot;//ho lo slot di inizio
                }
            } else {
                ///reset, perchè ho bisogno di slot consecutivi
                classroomIsOk = false;
                numSlotsFoundedSoFar = 0;
            }
        }
    }
    if (foundedStartHourSlot == -1)
        ///non trovato un buco abbastanza grande
        return -1;
        _tempGroupedCourseClassrooms.insert(_tempGroupedCourseClassrooms.begin(),idRoomsFounded.begin(),idRoomsFounded.end());

    return foundedStartHourSlot;
}

///aggiorna la mappa di slot occupati per i professori
bool ExamDay::assignExamToProf(std::map<int, Professor> &allUniversityProfs, Course course, int hhStart, int num_slots) {
    ///dobbiamo marcare come "occupati" gli slots negli oggetti professore interessati (tutti quelli di un corso specifico)
    SpecificYearCourse specificCourse = course.getThisYearCourse(_date.getYear() - 1); //prendiamo corso specifico dell'anno di questo Exam Day
    std::vector<int> profsMatr = specificCourse.getAllProfMatr(); //tutti i professori di tutti i corsi paralleli
    std::string dateAsString = _date.toString();
    for (int matr = 0; matr < profsMatr.size(); matr++) {
        for (int num_slot = 0; num_slot < num_slots; num_slot++) {
            allUniversityProfs.at(profsMatr[matr]).addNewExam(dateAsString, hhStart + (num_slot * 2), course.getId());//al prof con matricola matr vado ad aggiungere l'esame
        }
    }
    return true;
}

///un corso da assegnare è dello stesso corso di studio e dello stesso anno di uno degli esami già assegnati per una certa data?
bool ExamDay::sameStudyCourseAndYear(Course course, int year) {

    SpecificYearCourse specificYearCourseToAssign = course.getThisYearCourse(
            year);//prendo l'anno specifico del corso da assegnare
    for (auto iterExamDay = _slots.begin(); iterExamDay !=
                                            _slots.end(); iterExamDay++) {//su uno degli examDay dei giorni precedenti da controllare ciclo su tutti gli slot
        std::vector<Course> examDayVect = iterExamDay->second; //estraggo per ogni slot il vettore di corsi
        for (int i = 0; i < examDayVect.size(); i++) {//ciclo sul vettore di corsi/esami da fare in quello slot
            SpecificYearCourse specificYearCourseAssigned = examDayVect[i].getThisYearCourse(
                    year);//prendo per quel corso/esame dello slot l'intero corso per un anno accademico specifico
            if (specificYearCourseAssigned.getStudyCourseAssigned() ==
                specificYearCourseToAssign.getStudyCourseAssigned()) {//controllo se l'esame da ssegnare e quello già assegnato sono dello stesso corso di studio
                if (specificYearCourseAssigned.getYearOfTheSemester() ==
                    specificYearCourseToAssign.getYearOfTheSemester())//controllo se l'esame da assegnare e quello già assegnato sono dello stesso anno
                    return true;//se stesso corso di studio e stesso anno ritorno true
            }
        }
    }
    return false;
}

///assegna l'esame negli slot accettabili in una particolare data
bool ExamDay::assignExamToExamDay(int hhStart, Course &course, int numSlot) {
    for (int slot = hhStart; slot < hhStart + (numSlot * 2); slot = slot + 2) {
        _slots.at(slot).push_back(course);
    }
    return true;
}

///set degli slot con interi da 8 a 18
bool ExamDay::setSlot() {
    for (int i = 0; i < 6; i++) {
        std::vector<Course> vectCourse;
        _slots.insert(std::pair<int, std::vector<Course>>(8 + i * 2, vectCourse));
    }
    return true;
}

///ritorna gli slot sottoforma di stringa
std::vector<std::string> ExamDay::getSlotsToString() {
    std::vector<std::string> slotsToReturn;
    std::vector<Course> CoursesPrintedSoFar;

    for (auto iterSlots = _slots.begin(); iterSlots != _slots.end(); iterSlots++) {
        std::stringstream genericSlot;

        int hStart = iterSlots->first;
        int hFinish = hStart + 2;
        //set hh_slot
        genericSlot << std::setfill('0') << std::setw(2) << hStart << ":00-" << hFinish << ":00;";

        //set coursesSlot
        std::vector<Course> coursesOfThisSlot = iterSlots->second;
        std::string formattedCoursesPerSlot = getFormattedCoursesPerSlot(coursesOfThisSlot, CoursesPrintedSoFar);
        genericSlot << formattedCoursesPerSlot;
        slotsToReturn.push_back(genericSlot.str());
    }
    return slotsToReturn;
}

///ritorna sottoforma di stringa i corsi con esame assegnato ad un determinato slot
std::string
ExamDay::getFormattedCoursesPerSlot(std::vector<Course> &coursesOfThisSlot, std::vector<Course> &CoursesPrintedSoFar) {
    std::stringstream singleSlotSS;
    for (int i = 0; i < coursesOfThisSlot.size(); i++) {
        SpecificYearCourse sp = coursesOfThisSlot[i].getThisYearCourse(_date.getYear() - 1);

        std::vector<int> rooms = sp.getRoomsAppeal();
        ///se questo corso è iniziato a uno slot precedente, dobbiamo scrivere una stringa vuota
        bool firstTime = firstSlotCourses(coursesOfThisSlot[i], CoursesPrintedSoFar);

        if (firstTime) {
            std::string classrooms =  classroomString(rooms);
            int numVersion = sp.getParalleleCours();
            if (numVersion != 1) {
                for (int j = 0; j < numVersion; j++) {
                    singleSlotSS << coursesOfThisSlot[i].getId() << "[" << j << "]" << "(C" << std::setfill('0') << std::setw(3) << sp.getStudyCourseAssigned()[0] << ")"<<classrooms;
                    if (j < numVersion - 1)
                        singleSlotSS << ";";
                }

            } else {
                singleSlotSS << coursesOfThisSlot[i].getId() << "(C" << std::setfill('0') << std::setw(3) << sp.getStudyCourseAssigned()[0] << ")"<<classrooms;
            }
            if (i < coursesOfThisSlot.size() - 1)
                singleSlotSS << ";";
            std::string ciao = singleSlotSS.str();
            ///push nel vettore di corsi finora considerati
            CoursesPrintedSoFar.push_back(coursesOfThisSlot[i]);
        } else ///se non è la prima volta scrivo ';'
            singleSlotSS << ";";
    }
    return singleSlotSS.str();
}

///è il primo slot a cui assegno quell'esame o nel precedente è stato già assegnato?
bool ExamDay::firstSlotCourses(Course courseToFind, std::vector<Course> &CoursesPrintedSoFar) {
    for (int i = 0; i < CoursesPrintedSoFar.size(); i++) {
        if (courseToFind.getId() == CoursesPrintedSoFar[i].getId())
            return false;
    }
    return true;
}

bool ExamDay::allSLotsAreEmpty() {
    for (auto iterSLots = _slots.begin(); iterSLots != _slots.end(); iterSLots++) {
        if (iterSLots->second.empty() == false)
            return false;
    }
    return true;
}

bool ExamDay::searchAvailableClassroomsInThisSlot(std::map<int, Classroom> &allUniversityClassrooms,int numSeatsToSeach, std::vector<int> &idRoomsFounded, int slotHour, int numSLotsRequired, std::string labOrClass) {
    int totSeatsFoundedSoFar = 0;
    bool roomIsOk = true;
    bool labExam;

    if(labOrClass == "A")
        labExam = false;
    else
        labExam = true;
    ///per tutte le aule controllo se è libera per il numero di slot richiesti e quanti posti ci siano
    for (int i = 1; i < allUniversityClassrooms.size(); i++) {
        Classroom &room = allUniversityClassrooms.at(i);
        ///controllo se la classRoom considerata è dello stesso tipo di labExam (aula o lab? dove faccio l'esame?)
        bool labClassroom = room.getLab();
            ///tra corsi raggruppati non può essere assegnata la stessa POSSSIBILE aula(possibile perchè non è ancora sicuro, ci sono altri controlli da fare prima dell'assegnazione)
            if (labExam == labClassroom && std::find(_tempGroupedCourseClassrooms.begin(), _tempGroupedCourseClassrooms.end(), i) == _tempGroupedCourseClassrooms.end()) {
                ///controllo che per numSlotsRequired consecutivi l'aula sia disponibile
                for (int j = 0; j < numSLotsRequired && roomIsOk; j++) {
                    int slot = slotHour + 2 * j;
                    if (room.checkAvailability(_date, slot) == false) {
                        roomIsOk == false;
                    }
                }
                ///se per il numSlotsRquired l'aula è disponibile segno che quell'aula va bene ma devo capire se come posti disponibili ci siamo
                if (roomIsOk) {
                    totSeatsFoundedSoFar += room.getNExamSeats();
                    idRoomsFounded.push_back(i);
                    if (numSeatsToSeach <= totSeatsFoundedSoFar)
                        return true;
                }
            }
        }

    return false;
}

bool ExamDay::checkProfsAvaibility(SpecificYearCourse &specificCourse, std::map<int, Professor> &allUniversityProfs,
                                   int relaxPar, int slotHour) {

        std::vector<int> profsMatr = specificCourse.getAllProfMatr(); //tutti i professori di tutti i corsi paralleli
        ///ci servono gli oggetti Professore per usare funzione amIavailable: tutti i professori DEVONO ESSERE DISPONBILI
        std::string dateAsString = _date.toString();//data sottoforma di stringa
        for (int i = 0; i < profsMatr.size(); i++) {
            ///controllo se i prof del corso sono liberi.
            // Considerato il fatto che corsi paralleli hanno l'esame lo stesso giorno alla
            // stessa ora devo controllare che tutti i prof di tutti i corsi in parallelo siano disponibili
            if (allUniversityProfs.at(profsMatr[i]).amIavailable(dateAsString, slotHour, relaxPar) == false) {
                return false;
            }
        }

    ///se tutti i prof sono disponibili in questo slot, ho trovato un nuovo slot potenzialmente valido per l'esame
    return true;
}

void ExamDay::eraseTempGroupedCourseClassrooms() {
    _tempGroupedCourseClassrooms.erase(_tempGroupedCourseClassrooms.begin(),_tempGroupedCourseClassrooms.end());
}

std::string ExamDay::classroomString(std::vector<int> rooms) {
    std::stringstream classroomsString;
    classroomsString <<"{";
    for(int i = 0; i<rooms.size();i++){
        classroomsString<<"A"<<std::setfill('0')<<std::setw(3)<<rooms[i];
        if(i<rooms.size()-1)
            classroomsString<<",";
    }
    classroomsString<<"}";
    return classroomsString.str();
}

int ExamDay::getEndHourOfThisCourseExam(const Course &course) {
    int lastSlotFounded=0;
    for (auto iterSlots = _slots.begin(); iterSlots != _slots.end(); iterSlots++){
        std::vector<Course> allExamInThisSlot = iterSlots->second;
        for(int i=0;i<allExamInThisSlot.size();i++){
            if(allExamInThisSlot[i].getId()==course.getId()){
                lastSlotFounded=iterSlots->first;
            }
        }
    }
    //ora di fine esame
    return lastSlotFounded+2;
}

void ExamDay::removeThisAppealInfo(int startSlot,std::string& idCourse){
    ///ciclare per numslots!!!!!!!!!!
    std::vector<Course> newCourses;
    std::vector<Course>& old = _slots.at(startSlot);
    for(int i=0;i<old.size(); i++){
        if(old[i].getId()!=idCourse)
            newCourses.push_back(old[i]);
    }
    _slots.erase(startSlot);
    _slots.insert(std::pair<int,std::vector<Course>>(startSlot,newCourses));
}









