//
// Created by Andrea on 06/12/2021.
//

#include <sstream>
#include "ExamDay.h"
#include "StudyCourse.h"


///se trova un buco disponibile ritorna l'ora di inizio, altrimenti ritorna -1
int ExamDay::isPossibleToAssignThisExamToProf(Course course, std::map<int, Professor>& allUniversityProfs, int numSlotsRequired) {
    SpecificYearCourse specificCourse = course.getThisYearCourse( _date.getYear()); //prendiamo corso specifico dell'anno di questo Exam Day
    ///cerchiamo un numSlotsRequired vuoti (ovviamente consecutivi)
    int startHourSlot = -1;
    int numSlotsFoundedSoFar = 0;
    int foundedStartHourSlot = -1;  //rimarrà -1 se non riesco a trovare nulla, altrimenti l'orario di inizio
    for (int i = 0; i < 6; i++ ) {
        if (_slots.count(8 + 2 * i) == 0) {//controllo sugli slot; 8-10-12....
            //slot i-esimo non occupato
            numSlotsFoundedSoFar++;
            if (numSlotsFoundedSoFar == 1)
                startHourSlot = 8 + 2 * i; //se dovessi trovare io numero di slot necessari partirei da lì
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


    return foundedStartHourSlot; //se arrivato qui, tutti i prof sono disponibili
}
/*
bool ExamDay::controlYear(std::string codCourseAssigned, std::string codCourseToBeAssigned) {
    int yearCCA = 0;
    int yearCCTBA = 0;

    for (auto iterSemester = _semesters.begin();
         iterSemester != _semesters.end(); iterSemester++) {//controllo tutti i semestri dei corsi di studio
        auto iterCourseAssigned = find(iterSemester->second.begin(), iterSemester->second.end(),
                                       codCourseAssigned);//punto ai corsi di un semestre e cerco tra questi il corso passato e di cui ho già assegnato l'esame
        auto iterCourseToBeAssigned = find(iterSemester->second.begin(), iterSemester->second.end(),
                                           codCourseToBeAssigned);//punto ai corsi di un semestre e cerco tra questi il corso passato e di cui dovrò assegnare l'esame
        if (iterCourseAssigned != iterSemester->second.end()) {//se lo trova
            std::stringstream CCA(iterSemester->first);
            CCA >> yearCCA;
        }
        if (iterCourseToBeAssigned != iterSemester->second.end()) {//se non lo trova
            std::stringstream CCTBA(iterSemester->first);
            CCTBA >> yearCCTBA;
        }
    }
    return yearCCA == yearCCTBA;
}*/

///aggiorna la mappa di slot occupati per i professori
bool ExamDay::assignExamToProf(std::vector<Professor> &allUniversityProfs, Course course, int hhStart, int num_slots) {
    ///dobbiamo marcare come "occupati" gli slots negli oggetti professore interessati (tutti quelli di un corso specifico)
    SpecificYearCourse specificCourse = course.getThisYearCourse( _date.getYear()); //prendiamo corso specifico dell'anno di questo Exam Day
    std::vector<int> profsMatr = specificCourse.getAllProfMatr(); //tutti i professori di tutti i corsi paralleli
    std::string dateAsString = _date.toString();
    for (int matr = 0; matr < profsMatr.size(); matr++) {
        for(int num_slot = 0; num_slot < num_slots; num_slot++) {
            allUniversityProfs.at(matr).addNewExam(dateAsString, hhStart + (num_slot * 2), course.getId());//al prof con matricola matr vado ad aggiungere l'esame
        }
    }
    return true;
}
/*
ExamDay::ExamDay(Date date) {

}*/







