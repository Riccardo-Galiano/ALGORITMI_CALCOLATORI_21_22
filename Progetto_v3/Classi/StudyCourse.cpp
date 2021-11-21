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
            std::vector<Course> vect;
            Course primocorso(courses[0]); // creo ogg corso xk courses[0] è una stringa
            vect.push_back(primocorso);//salvo il primo corso del semestre
            _semesters.insert(std::pair<std::string, std::vector<Course>>(key, vect));//inserisco nella mappa il primo corso del semestre
        } else {//se la chiave esiste
            i++;
            Course corsoGenerico(courses[i]);
            _semesters.at(key).push_back(corsoGenerico);//aggiungo nel semestre già esistenete i corsi successivo al primo
        }
    }
    return true;

}

///aggiunge corso spento
bool StudyCourse::addOffCourses(const std::vector<std::string> &corsiSpenti) {
    for (int i=0; i<corsiSpenti.size(); i++) {
        Course corsospento(corsiSpenti[i]);//costruttore provvisorio da definire
        _corsiSpenti.insert(std::pair<std::string, Course>(corsiSpenti[i],corsospento));//per ogni codice del corso spento associo un oggetto corso
    }
    return true;
}

///prende l'_id
const int StudyCourse::getId() const {
    return _id;
}

bool StudyCourse::updateThatCourse(std::vector<std::string>& infoCourse) {
    for(auto currentSemester = _semesters.begin(); currentSemester != _semesters.end(); currentSemester++ ){//analizzo ogni semestre
        int n_courses = (currentSemester->second).size(); //numero di corsi per semestre

        for(int j=0; j < n_courses; j++){ //analizzo tutti i corsi del semestre
            Course* currentcourse = &(currentSemester->second[j]);
            if(currentcourse->getId() == infoCourse[1]){ //se L'id dell'oggetto corso puntato è uguale a quello passato da file
                currentcourse->updateInfoCourse(infoCourse); //aggiorno il corso
                return true;
            }
        }
    }
    return false;
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