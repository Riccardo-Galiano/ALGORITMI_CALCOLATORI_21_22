//
// Created by lucam on 05/11/2021.
//

#include "Course.h"

Course::Course(const std::string &idCorso) {

    _id = idCorso;
    _name = "non inserito";
    _cfu = 0;
    _hours._lec = 0;
    _hours._ex = 0;
    _hours._lab = 0;

}

Course::Course(const std::string &idCorso, const std::string &nomeCorso, const int cfu, const int oreLezione,const int oreEsercitazione, const int oreLaboratorio) {

    _id = idCorso;
    _name = nomeCorso;
    _cfu = cfu;
    _hours._lec = oreLezione;
    _hours._ex = oreEsercitazione;
    _hours._lab = oreLaboratorio;

}

///aggiorna quelle che erano i parametri dello Study Course inizializzati con valori di default
bool Course::updateInfoCourse(const std::vector<std::string> &infoCorso) {
    //_id = infoCorso[1];
    _name = infoCorso[2];
    _cfu = stoi(infoCorso[3]);
    _hours._lec = stoi(infoCorso[4]);
    _hours._ex = stoi(infoCorso[5]);
    _hours._lab = stoi(infoCorso[6]);
    return true;
}

const std::string Course::getId() const {
    return _id;
}

//aggiunge per ogni anno accademico il corso con le sue informazioni
bool Course::addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,  std::vector<std::string> exam, std::vector<std::string> idPar) {
    _courseOfTheYear.insert(std::pair<int,SpecificYearCourse>(stoi(sY_eY.substr(0,4)),SpecificYearCourse(sY_eY,active,nCrsiPar,prof,exam,idPar)));
    return true;
}

bool Course::controlActive() {
    auto iterCourseOfTheYear = _courseOfTheYear.rbegin();
    return iterCourseOfTheYear->second.getActive();
}

int Course::controlNumParallel() {
    auto iterCourseOfTheYear = _courseOfTheYear.rbegin();
    return  iterCourseOfTheYear->second.getNumParallel();
}



