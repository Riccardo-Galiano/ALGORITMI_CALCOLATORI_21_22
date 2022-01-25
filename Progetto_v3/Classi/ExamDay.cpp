//
// Created by Andrea on 06/12/2021.
//

#include <sstream>
#include "ExamDay.h"
#include "StudyCourse.h"
#include "Classroom.h"
#include "Parse.hpp"
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
    char labOrClass = specificCourse.getPlaceExam();
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
            if (!classroomIsOk) {
                ///num aule max = num corsi par
                int maxNumClassrooms = specificCourse.getNumParallelCourses();

                thereAreEnoughClassrooms = searchAvailableClassroomsInThisSlot(allUniversityClassrooms, numStuds,
                                                                                   idRoomsFounded, slotHour,
                                                                                   numSlotsRequired, labOrClass,
                                                                                   maxNumClassrooms);

            }

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
        ///non ho trovato un buco abbastanza grande
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
    //prendo l'anno specifico del corso da assegnare
    SpecificYearCourse specificYearCourseToAssign = course.getThisYearCourse( year);
    for (auto iterExamDay = _slots.begin(); iterExamDay !=  _slots.end(); iterExamDay++) {//su uno degli examDay dei giorni precedenti da controllare ciclo su tutti gli slot
        std::vector<Course> examDayVect = iterExamDay->second; //estraggo per ogni slot il vettore di corsi
        for (int i = 0; i < examDayVect.size(); i++) {//ciclo sul vettore di corsi/esami da fare in quello slot
            SpecificYearCourse specificYearCourseAssigned = examDayVect[i].getThisYearCourse(year);//prendo per quel corso/esame dello slot l'intero corso per un anno accademico specifico
            if (specificYearCourseAssigned.getStudyCourseAssigned() == specificYearCourseToAssign.getStudyCourseAssigned()) {//controllo se l'esame da assegnare e quello già assegnato sono dello stesso corso di studio
                if (specificYearCourseAssigned.getYearOfTheSemester() == specificYearCourseToAssign.getYearOfTheSemester())//controllo se l'esame da assegnare e quello già assegnato sono dello stesso anno
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
std::string ExamDay::getFormattedCoursesPerSlot(std::vector<Course> &coursesOfThisSlot, std::vector<Course> &CoursesPrintedSoFar) {
    std::stringstream singleSlotSS;
    for (int i = 0; i < coursesOfThisSlot.size(); i++) {
        SpecificYearCourse sp = coursesOfThisSlot[i].getThisYearCourse(_date.getYear() - 1);
        std::vector<int> rooms = sp.getRoomsAppeal();
        ///se questo corso è iniziato a uno slot precedente, dobbiamo scrivere una stringa vuota
        bool firstTime = firstSlotCourses(coursesOfThisSlot[i], CoursesPrintedSoFar);
        std::string settedIdStudyCourse = Parse::setId('C',3,sp.getStudyCourseAssigned()[0]);
        std::string settedIdRooms;

        if (firstTime) {
            //non mi serve più
            //std::string classrooms =  classroomString(rooms);

            int numVersion = sp.getNumParallelCourses();
            if (numVersion != 1) {
                ///Se ci sono più versioni distinguo 2 casi
                ///A] ci sono tante aule quante versioni
                ///B] aule < num versioni
                if(numVersion == rooms.size()){
                    //A
                    for (int j = 1; j <= numVersion; j++) {
                        settedIdRooms = Parse::setId('A',3,rooms[j-1]);
                        singleSlotSS << coursesOfThisSlot[i].getId() << "[" << j << "]" << "(" <<settedIdStudyCourse << ")|"<< settedIdRooms;
                        if (j < numVersion)
                            singleSlotSS << ";";
                    }
                }
                else{
                    //B ... metto sempre la stessa aula alla fine
                    for (int j = 1; j <= numVersion; j++) {
                        int room;
                        if(j>rooms.size()){
                            room = rooms[rooms.size()-1];
                        }
                        else{
                            room = rooms[j-1];
                        }
                        std::string settedIdRoom = Parse::setId('A',3,room);
                        singleSlotSS << coursesOfThisSlot[i].getId() << "[" << j << "]" << "(" << settedIdStudyCourse << ")|" <<  settedIdStudyCourse;
                        if (j < numVersion)
                            singleSlotSS << ";";
                    }
                }
            } else {
                ///se c'è solo una versione
                settedIdRooms = Parse::setId('A',3,rooms[0]);
                singleSlotSS << coursesOfThisSlot[i].getId() << "(" << settedIdStudyCourse << ")|" << settedIdRooms;
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

bool comparatorFunction (Classroom i,Classroom j) {
    return (i.getNExamSeats() < j.getNExamSeats());
}

bool ExamDay::searchAvailableClassroomsInThisSlot(std::map<int, Classroom> &allUniversityClassrooms, int numSeatsToSeach, std::vector<int> &idRoomsFounded, int slotHour, int numSLotsRequired, char labOrClass, int maxNumRooms) {
    bool roomIsOk = true;
    bool examInLab;
    std::vector<Classroom> potentialRooms;
    if(labOrClass == 'A')
        examInLab = false;
    else
        examInLab = true;
    ///per tutte le aule controllo se è libera per il numero di slot richiesti e quanti posti ci siano
    for (int i = 1; i < allUniversityClassrooms.size(); i++){
        Classroom &room = allUniversityClassrooms.at(i);
        ///controllo se la classRoom considerata è dello stesso tipo di examInLab (aula o lab? dove faccio l'esame?)
        bool isLab = room.isThisLab();
        bool thisRoomIsNotUsedByAGroupedCourse = std::find(_tempGroupedCourseClassrooms.begin(), _tempGroupedCourseClassrooms.end(), i) == _tempGroupedCourseClassrooms.end();
            ///tra corsi raggruppati non può essere assegnata la stessa POSSSIBILE aula(possibile perchè non è ancora sicuro, ci sono altri controlli da fare prima dell'assegnazione)
            if (examInLab == isLab && thisRoomIsNotUsedByAGroupedCourse) {
                ///controllo che per numSlotsRequired consecutivi l'aula sia disponibile
                for (int j = 0; j < numSLotsRequired && roomIsOk; j++) {
                    int slot = slotHour + 2 * j;
                    if (!room.checkAvailability(_date, slot)) {
                        roomIsOk = false;
                    }
                }
                ///se per il numSlotsRquired l'aula è disponibile allora la aggiungo al vettore di aule disponibili
                if (roomIsOk) {
                    potentialRooms.push_back(allUniversityClassrooms.at(i));
                }
            }
    }

    ///prendo dalle aule trovate potentialRooms un numero di aule <= maxNumRooms && numSeatsToSeach <= totSeats in queste aule
    bool iFoundSomething = pickSomeOfTheseClassrooms(potentialRooms,idRoomsFounded,numSeatsToSeach,maxNumRooms);
    //se le trovo esco con true
    if(iFoundSomething)
        return true;
    else {
        ///check se non esisteranno mai queste aule -> error!
        std::vector<Classroom> allRooms;
        std::vector<int> inutile;
        bool canExist;// è un problema di disponibilità aule per quel giorno o è un problema che esisterebbe anche se tutte le aule in db fossero libere?
        //prendo tutte le aule del db
        for (auto iterClass = allUniversityClassrooms.begin(); iterClass != allUniversityClassrooms.end(); iterClass++) {
             allRooms.push_back(iterClass->second);
        }

        if(pickSomeOfTheseClassrooms(allRooms,inutile,numSeatsToSeach,maxNumRooms)){
            canExist = true;
        }
        else
            canExist = false;
        if(canExist == false){
            throw std::invalid_argument("Aula introvabile");
        } else
            return false;
    }
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

///prende l'ora finale dell'esame di questo corso
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

void ExamDay::removeThisAppealInfo(int startSlot,int numSlots,std::string& idCourse){

    for(int i = 0; i < numSlots; i++){
        int slot = startSlot + 2*i;
        std::vector<Course> newCourses;
        std::vector<Course>& old = _slots.at(slot);
        for(int j=0;j<old.size(); j++){
            if(old[j].getId()!=idCourse)
                newCourses.push_back(old[j]);
        }
        _slots.erase(slot);
        _slots.insert(std::pair<int,std::vector<Course>>(slot,newCourses));
    }
}

bool ExamDay::pickSomeOfTheseClassrooms(std::vector<Classroom> &potentialRooms, std::vector<int> &idRoomsFounded,
                                        int numSeatsToSeach, int maxNumRooms) {
    //ordino le aule in senso crescente di #posti
    std::sort (potentialRooms.begin(), potentialRooms.end(), comparatorFunction);
    //cerco di massimizzare numero aule (finestra più grande possibile) -> ciclo all'indietro
    for(int numTotRoomsToSearch = maxNumRooms; numTotRoomsToSearch >= 1; numTotRoomsToSearch--){
        //cerco per finestra (sliding window)
        for(int index = numTotRoomsToSearch - 1; index < potentialRooms.size(); index++){
            int totSeats = 0;
            std::vector<int> tempRooms;
            for(int j = index + 1 - numTotRoomsToSearch; j <= index ; j++){
                totSeats += potentialRooms[j].getNExamSeats();
                tempRooms.push_back(potentialRooms[j].getId());
            }
            if(totSeats >= numSeatsToSeach){
                ///ho trovato ottimo = prima occorrenza
                idRoomsFounded = tempRooms;
                return true;
            }
        }
    }
    return false;
}

void ExamDay::updateSlot(Course course) {
    for(auto iterSlot = _slots.begin(); iterSlot != _slots.end(); iterSlot++){
        std::string id = course.getId();
        //per ogni corso del vector mi chiedo se l'id è lo stesso, se si cancello l'intero corso vecchio e metto il nuovo
        for(auto iterCourse = iterSlot->second.begin(); iterCourse != iterSlot->second.end(); iterCourse++) {
            std::string idOldCourse = iterCourse->getId();
            if (id == idOldCourse) {
                iterSlot->second.erase(iterCourse);
                iterSlot->second.push_back(course);
            }
        }
    }
}







