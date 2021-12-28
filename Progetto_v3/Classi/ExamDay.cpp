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
int ExamDay::isPossibleToAssignThisExamToProf(Course course, std::map<int, Professor>& allUniversityProfs, int numSlotsRequired, int relaxPar) {
    SpecificYearCourse specificCourse = course.getThisYearCourse( _date.getYear() - 1); //prendiamo corso specifico dell'anno di questo Exam Day (-1)
    ///cerchiamo un numSlotsRequired vuoti (ovviamente consecutivi)

    //!!! esami non raggruppati dovrebbero poter essere messi nello stesso slot in base alla disponibilità di aule!!!!!!!!

    int startHourSlot = -1;
    int numSlotsFoundedSoFar = 0;
    int foundedStartHourSlot = -1;  //rimarrà -1 se non riesco a trovare nulla, altrimenti l'orario di inizio

   //questo for non va bene. cerca solo slot vuoti!!
   //per ogni slot controllare disponibilità di aule!!!
   //da costruire funzione di ricerca aule!!!!

    for (int i = 0; i < 6 && foundedStartHourSlot == -1; i++ ) {

         if (_slots.at(8 + 2 * i).empty()) {//controllo sugli slot; 8-10-12....
             //slot i-esimo non occupato
             numSlotsFoundedSoFar++;
             if (numSlotsFoundedSoFar == 1)
                 startHourSlot = 8 + 2 * i; //se dovessi trovare il numero di slot necessari partirei da lì
             if (numSlotsFoundedSoFar == numSlotsRequired)//se il numero di slot trovati coincide con il numero di slot necessari
                 foundedStartHourSlot = startHourSlot;//ho lo slot di inizio
         } else
             //slot già occupato
             numSlotsFoundedSoFar = 0; //reset, perchè ho bisogno di slot consecutivi

    }
    if (foundedStartHourSlot == -1)
        //non trovato un buco abbastanza grande
        return -1;

    ///dobbiamo ora controllare se i prof sono disponibili in questi slot per essere sicuri che questo esame è possibile in questo giorno
   if (relaxPar<3){
       std::vector<int> profsMatr = specificCourse.getAllProfMatr(); //tutti i professori di tutti i corsi paralleli
       //ci servono gli oggetti Professore per usare funzione amIavailable: tutti i professori DEVONO ESSERE DISPONBILI

       std::string dateAsString = _date.toString();//data sottoforma di stringa
       for (int i = 0; i < profsMatr.size(); i++) { //controllo se i prof del corso sono liberi. Considerato il fatto che corsi paralleli hanno l'esame lo stesso giorno alla stessa ora devo controllare che tutti i prof di tutti i corsi in parallelo siano disponibili
           for(int n_slot = 0; n_slot < numSlotsRequired; n_slot++) {
               //devo controllare tutti gli slot
               if (!allUniversityProfs.at(profsMatr[i]).amIavailable(dateAsString, foundedStartHourSlot + (2 * n_slot))) {
                   //se questo prof non è disponibile, torno false
                   return -1;
               }
           }
       }
   }
    return foundedStartHourSlot; //se arrivato qui, tutti i prof sono disponibili
}

///aggiorna la mappa di slot occupati per i professori
bool ExamDay::assignExamToProf(std::map<int, Professor> &allUniversityProfs, Course course, int hhStart, int num_slots) {
    ///dobbiamo marcare come "occupati" gli slots negli oggetti professore interessati (tutti quelli di un corso specifico)
    SpecificYearCourse specificCourse = course.getThisYearCourse( _date.getYear()-1); //prendiamo corso specifico dell'anno di questo Exam Day
    std::vector<int> profsMatr = specificCourse.getAllProfMatr(); //tutti i professori di tutti i corsi paralleli
    std::string dateAsString = _date.toString();
    for (int matr = 0; matr < profsMatr.size(); matr++) {
        for(int num_slot = 0; num_slot < num_slots; num_slot++) {
            allUniversityProfs.at(profsMatr[matr]).addNewExam(dateAsString, hhStart + (num_slot * 2), course.getId());//al prof con matricola matr vado ad aggiungere l'esame
        }
    }
    return true;
}

///un corso da assegnare è dello stesso corso di studio e dello stesso anno di uno degli esami già assegnati per una certa data?
bool ExamDay::sameStudyCourseAndYear(Course course,int year) {

  SpecificYearCourse specificYearCourseToAssign = course.getThisYearCourse(year);//prendo l'anno specifico del corso da assegnare
   for(auto iterExamDay = _slots.begin(); iterExamDay != _slots.end(); iterExamDay++){//su uno degli examDay dei giorni precedenti da controllare ciclo su tutti gli slot
       std::vector<Course> examDayVect = iterExamDay->second; //estraggo per ogni slot il vettore di corsi
       for(int i = 0; i<examDayVect.size(); i++){//ciclo sul vettore di corsi/esami da fare in quello slot
           SpecificYearCourse specificYearCourseAssigned = examDayVect[i].getThisYearCourse(year);//prendo per quel corso/esame dello slot l'intero corso per un anno accademico specifico
           if(specificYearCourseAssigned.getStudyCourseAssigned() == specificYearCourseToAssign.getStudyCourseAssigned()) {//controllo se l'esame da ssegnare e quello già assegnato sono dello stesso corso di studio
               if (specificYearCourseAssigned.getYearOfTheSemester() == specificYearCourseToAssign.getYearOfTheSemester())//controllo se l'esame da assegnare e quello già assegnato sono dello stesso anno
                   return true;//se stesso corso di studio e stesso anno ritorno true
           }
       }
   }
    return false;
}

///assegna l'esame negli slot accettabili in una particolare data
bool ExamDay::assignExamToExamDay(int hhStart, Course course, int numSlot) {
    for(int slot = hhStart; slot < hhStart + (numSlot * 2); slot = slot + 2 ) {
        _slots.at(slot).push_back(course);
    }
    return true;
}

///set degli slot con interi da 8 a 18
bool ExamDay::setSlot() {
    for(int i = 0; i < 6; i++){
        std::vector<Course> vectCourse;
        _slots.insert(std::pair<int,std::vector<Course>>(8 + i*2,vectCourse));
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
std::string ExamDay::getFormattedCoursesPerSlot(std::vector<Course>& coursesOfThisSlot, std::vector<Course>& CoursesPrintedSoFar) {
    std::stringstream singleSlotSS;
    for (int i = 0; i < coursesOfThisSlot.size(); i++){
        SpecificYearCourse sp = coursesOfThisSlot[i].getThisYearCourse(_date.getYear()-1);
        ///se questo corso è iniziato a uno slot precedente, dobbiamo scrivere una stringa vuota
        bool firstTime = firstSlotCourses(coursesOfThisSlot[i], CoursesPrintedSoFar);

        if (firstTime){

            int numVersion = sp.getParalleleCours();
            if (numVersion != 1) {
                for (int j = 0; j < numVersion; j++) {
                    singleSlotSS << coursesOfThisSlot[i].getId() << "[" << j << "]" << "(C"<< std::setfill('0') << std::setw(3)<< sp.getStudyCourseAssigned()[0] << ")";
                    if(j < numVersion-1)
                        singleSlotSS<<";";
                }
                std::string ciao = singleSlotSS.str();
            } else {
                singleSlotSS << coursesOfThisSlot[i].getId() << "(C"<< std::setfill('0') << std::setw(3)<< sp.getStudyCourseAssigned()[0] << ")";

            }
            if(i<coursesOfThisSlot.size()-1)
                singleSlotSS << ";";
            ///push nel vettore di corsi finora considerati
            CoursesPrintedSoFar.push_back(coursesOfThisSlot[i]);
        } else ///se non è la prima volta scrivo ';'
            singleSlotSS<<";";



    }
    return singleSlotSS.str();
}

///è il primo slot a cui assegno quell'esame o nel precedente è stato già assegnato?
bool ExamDay::firstSlotCourses(Course courseToFind, std::vector<Course>& CoursesPrintedSoFar) {
    for(int i = 0; i < CoursesPrintedSoFar.size(); i++){
        if(courseToFind.getId() == CoursesPrintedSoFar[i].getId())
            return false;
    }
    return true;
}

bool ExamDay::allSLotsAreEmpty() {
    for(auto iterSLots = _slots.begin(); iterSLots != _slots.end();iterSLots++){
        if(iterSLots->second.empty() == false)
            return false;
    }
    return true;
}







