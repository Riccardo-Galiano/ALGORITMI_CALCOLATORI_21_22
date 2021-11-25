//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "Course.h"

std::vector<std::string> splittedLine3(const std::string &s, const char &delimiter) {


    std::vector<std::string> toReturn; //conterrà la riga splittata nelle sue informazioni necessarie e indicizzate
    std::istringstream line(s); // mi serve per poter manipolare le stringhe
    std::string token; //buffer di appoggio per salvare l'informazione appena ricavata


    //fin quando la riga non è finita prende l'intera riga(line) e salva in una stringa del vettore di stringhe(tokens) l'informazione fino al prossimo delimitatore
    while (std::getline(line, token, delimiter)) {
        toReturn.push_back(token);
    }

    return toReturn;
}

Course::Course(const std::string &idCorso, const std::string &nomeCorso, const int cfu, const int oreLezione,const int oreEsercitazione, const int oreLaboratorio) {

    _id = idCorso;
    _name = nomeCorso;
    _cfu = cfu;
    _hours._lec = oreLezione;
    _hours._ex = oreEsercitazione;
    _hours._lab = oreLaboratorio;

}

//aggiunge per ogni anno accademico il corso con le sue informazioni
bool Course::addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,std::vector<std::string> exam, std::vector<std::string> idPar) {

    _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(stoi(sY_eY.substr(0, 4)),SpecificYearCourse(sY_eY, active, nCrsiPar, prof, exam,idPar)));

    return true;
}

bool Course::fillSpecificYearCourse(std::vector<std::string> &specificYearCourse) {
    std::string acYear = specificYearCourse[1];
    std::stringstream ss(acYear);
    int startYear, endYear;
    char c;

    ss >> startYear >> c >> endYear;
    if (_courseOfTheYear.find(startYear) != _courseOfTheYear.end())
        throw std::invalid_argument("anno già esistente");

    SpecificYearCourse lastYearSpecificCourse = getLastSpecificYearCourse();
    std::stringstream last;
    std::vector<std::string> lastYearSpecificYearCourseSplitted;
    last << lastYearSpecificCourse;

    lastYearSpecificYearCourseSplitted = splittedLine3(last.str(), ';');

    for (int i = 2; i < specificYearCourse.size(); i++) {
        if (specificYearCourse[i].empty()) {
            specificYearCourse[i] = lastYearSpecificYearCourseSplitted[i - 1];
        }
    }
    return true;
}

SpecificYearCourse &Course::getLastSpecificYearCourse() {
    int lastYear = 0, actualYear;

    for (auto iter = _courseOfTheYear.begin(); iter != _courseOfTheYear.end(); iter++) {
        actualYear = iter->second.getStartYear();
        if (actualYear > lastYear)
            lastYear = actualYear;
    }

    return _courseOfTheYear.at(lastYear);
}




