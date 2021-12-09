//
// Created by Andrea on 06/12/2021.
//

#include <sstream>
#include "ExamDay.h"

///quanti slot servono per un esame
int ExamDay::howManySlots(Exam exam) {
    int sum = exam.getTime() + exam.getEnterTime() + exam.getLeaveTime();//sommo le ore per l'esame per entrare e uscire dall'aula
    int num_slots;
    int duration = 2; //slot da due ore
    num_slots = sum / duration + 1;//+1 perchè se venisse un numero razionale il compilatore raggionerebbe per difetto ma a noi interessa ragionare per eccesso
    return num_slots;
}

///
int ExamDay::isPossibleToAssignThisExam(Course course, std::vector<Professor> &allUniversityProfs, int numSlotsRequired) {
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
    std::string matr;
    std::string dateAsString = _date.toString();//data sottoforma di stringa
    for (int matr = 0; matr < profsMatr.size(); matr++) { //controllo se i prof del corso sono liberi. Considerato il fatto che corsi paralleli hanno l'esame lo stesso giorno alla stessa ora devo controllare che tutti i prof di tutti i corsi in parallelo siano disponibili
        for(int n_slot = 0; n_slot < numSlotsRequired; n_slot++) {
            //devo controllare tutti gli slot
            if (!allUniversityProfs.at(matr).amIavailable(dateAsString, foundedStartHourSlot + (2 * n_slot))) {
                //se questo prof non è disponibile, torno false
                return -1;
            }
        }
    }
    return foundedStartHourSlot; //se arrivato qui, tutti i prof sono disponibili
}

///segna che uno slot del prof è occupato da un esame
bool ExamDay::assignExam(std::vector<Professor> &allUniversityProfs, Course course, int hhStart, int num_slots) {
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
