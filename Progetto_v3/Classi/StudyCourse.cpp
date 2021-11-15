//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "StudyCourse.h"
#include "Course.h"

StudyCourse::StudyCourse(int id, bool isBachelor) : _id{id}, _isBachelor{isBachelor} {}
std::vector<std::string> splittedLine2(const std::string &s,char delimiter );

bool StudyCourse::addSemesterCourses(int year, int semester,std::string SemesterCourses) {
    int i=0;
    std::stringstream ss;
    ss << year << "-" << semester;
    std::string key = ss.str();//creo la chiave con anno e semestre
    std::vector<std::string> courses;

    courses = splittedLine2(SemesterCourses, ',');//adesso ho i corsi del semestre passati alla funzione che non erano divisi

   for(auto iter = courses.begin();iter != courses.end(); iter++){//analizzo tutti i componenti del vettore corsi
        if (!_semesters.count(key)) {//se la chiave non esiste
             std::vector<std::string> vect;
             vect.push_back(courses[0]);//salvo il primo corso del semestre
            _semesters.insert(std::pair<std::string,std::vector<std::string>>(key, vect));//inserisco nella mappa l primo corso del semestre
         }else {//se la chiave esiste
             i++;
            _semesters.at(key).push_back(courses[i]);//aggiungo i corsi successivo al primo
         }
     }
    return true;

}

bool StudyCourse::addOffCourses(std::vector<std::string> corsiSpenti) {
    int i = 0;

    for(auto iter = corsiSpenti.begin();iter != corsiSpenti.end(); iter++) {

        Course corsospento;//da definire

        _corsiSpenti.insert(std::pair<std::string, Course>(corsiSpenti[i],corsospento));//per ogni codice del corso spento associo un oggetto corso
         i++;//sarebbe il caso di usare l'iteratore ma non riesco
    }
    return true;
}

///per ogni riga del file in input scinde le varie info delimitate da ";"
std::vector<std::string> splittedLine2(const std::string &s,char delimiter ) {


    std::vector<std::string> toReturn; //conterrà la riga splittata nelle sue informazioni necessarie e indicizzate con vector
    std::istringstream line(s); // mi serve per poter manipolare le stringhe
    std::string token; //buffer di appoggio per salvare l'informazione appena ricavata


    //fin quando la riga non è finita prende l'intera riga e salva in un vettore di string l'informazione fino al prossimo delimitatore
    while (std::getline(line, token, delimiter)) {
        toReturn.push_back(token);
    }

    return toReturn;
}