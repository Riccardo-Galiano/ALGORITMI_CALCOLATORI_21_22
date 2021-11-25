//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "StudyCourse.h"

StudyCourse::StudyCourse(const int id, const bool &isBachelor) : _id{id}, _isBachelor{isBachelor} {}

std::vector<std::string> splittedLine2(const std::string &s, const char &delimiter);

///aggiunge un semsetre con i relativi corsi al corso di studio
bool StudyCourse::addSemesterCourses(const int year, const int semester, const std::string &SemesterCourses) {
    ///key
    int i = 0;
    std::stringstream ss;
    ss << year << "-" << semester;
    std::string key = ss.str(); //creo la chiave con anno e semestre

    ///create values
    std::vector<std::string> courses;
    courses = splittedLine2(SemesterCourses,',');//adesso ho i corsi del semestre passati alla funzione che non erano divisi

    for (auto iter = courses.begin(); iter != courses.end(); iter++) {//analizzo tutti i componenti del vettore corsi
        if (!_semesters.count(key)) {//se la chiave non esiste
            std::vector<std::string> vect;

            vect.push_back(courses[0]);//salvo il primo corso del semestre
            _semesters.insert(std::pair<std::string, std::vector<std::string>>(key, vect));//inserisco nella mappa il primo corso del semestre
        } else {//se la chiave esiste
            i++;
            _semesters.at(key).push_back(courses[i]);//aggiungo nel semestre già esistenete i corsi successivi al primo
        }
    }
    return true;

}

///aggiunge corso spento
bool StudyCourse::addOffCourses(const std::vector<std::string> &corsiSpenti) {
    for (int i=0; i<corsiSpenti.size(); i++) {
        _corsiSpenti.push_back(corsiSpenti[i]);//per ogni codice del corso spento associo un oggetto corso
    }
    _corsiSpenti.sort();//se li vogliamo in ordine crescente;
    return true;
}

///prende l'_id
const int StudyCourse::getId() const {
    return _id;
}



///funzione che splitta una riga considerando un delimitatore passate by reference
std::vector<std::string> splittedLine2(const std::string &s, const char &delimiter) {


    std::vector<std::string> toReturn; //conterrà la riga splittata nelle sue informazioni necessarie e indicizzate con vector
    std::istringstream line(s); // mi serve per poter manipolare le stringhe
    std::string token; //buffer di appoggio per salvare l'informazione appena ricavata


    //fin quando la riga non è finita prende l'intera riga e salva in un vettore di string l'informazione fino al prossimo delimitatore
    while (std::getline(line, token, delimiter)) {
        toReturn.push_back(token);
    }

    return toReturn;
}