//
// Created by Andrea on 06/12/2021.
//

#include <sstream>
#include "ExamDay.h"

int ExamDay::howManySlots(Exam exam) {
    int sum = exam.getTime() + exam.getEnterTime() + exam.getLeaveTime();
    int num_slots;
    int duration = 2; //hours
    num_slots = sum / duration + 1;
    return num_slots;
}

int
ExamDay::isPossibleToAssignThisExam(Course course, std::vector<Professor> &allUniversityProfs, int numSlotsRequired) {
    SpecificYearCourse specificCourse = course.getThisYearCourse(
            _date.getYear()); //prendiamo corso specifico dell'anno di questo Exam Day
    ///cerchiamo un numSlotsRequired vuoti (ovviamente consecutivi)
    int startHourSlot = -1;
    int numSlotsFoundedSoFar = 0;
    int foundedStartHourSlot = -1;  //rimarrà -1 se non riesco a trovare nulla, altrimenti l'orario di inizio
    for (int i = 0; _slots.size(); i++) {
        if (_slots.count(8 + 2 * i) == 0) {
            //slot i-esimo non occupato
            numSlotsFoundedSoFar++;
            if (numSlotsFoundedSoFar == 1)
                startHourSlot = 8 + 2 * i;
            if (numSlotsFoundedSoFar == numSlotsRequired)
                foundedStartHourSlot = startHourSlot;
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
    std::string dateAsString = _date.toString();
    for (int matr = 0; matr < profsMatr.size(); matr++) {
        if (!allUniversityProfs.at(matr).amIavailable(dateAsString, foundedStartHourSlot)) {
            //se questo prof non è disponibile, torno false
            return -1;
        }
    }
    return foundedStartHourSlot; //se arrivato qui, tutti i prof sono disponibili
}

bool ExamDay::assignExam(std::vector<Professor> &allUniversityProfs, Course course, int hhStart, int num_slots) {
    ///dobbiamo marcare come "occupati" gli slots negli oggetti professore interessati (tutti quelli di un corso specifico)
    SpecificYearCourse specificCourse = course.getThisYearCourse(
            _date.getYear()); //prendiamo corso specifico dell'anno di questo Exam Day
    std::vector<int> profsMatr = specificCourse.getAllProfMatr(); //tutti i professori di tutti i corsi paralleli
    std::string dateAsString = _date.toString();
    for (int matr = 0; matr < profsMatr.size(); matr++) {
        for(int num_slot = 0; num_slot < num_slots; num_slot++) {
            allUniversityProfs.at(matr).addNewExam(dateAsString, hhStart + (num_slot * 2), course.getId());
        }
    }
    return true;
}
