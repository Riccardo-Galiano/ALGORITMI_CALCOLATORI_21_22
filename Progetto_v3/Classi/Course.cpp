//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "Course.h"

Course::Course(const std::string &idCorso) {

    _id = idCorso;
    _name = "non inserito";
    _cfu = 0;
    _hours._lec = 0;
    _hours._ex = 0;
    _hours._lab = 0;

}

Course::Course(const std::string &idCorso, const std::string &nomeCorso, const int cfu, const int oreLezione,
               const int oreEsercitazione, const int oreLaboratorio) {

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
bool Course::addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,
                                    std::vector<std::string> exam, std::vector<std::string> idPar) {
    _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(stoi(sY_eY.substr(0, 4)),
                                                               SpecificYearCourse(sY_eY, active, nCrsiPar, prof, exam,
                                                                                  idPar)));
    return true;
}

bool Course::controlActive() {
    auto iterCourseOfTheYear = _courseOfTheYear.rbegin();
    return iterCourseOfTheYear->second.getActive();
}

int Course::controlNumParallel() {
    auto iterCourseOfTheYear = _courseOfTheYear.rbegin();
    return iterCourseOfTheYear->second.getNumParallel();
}

enum{active_empty = 2, num_par_empty=3, prof_empty =4, info_exam_empty=5, par_cod_empty = 6};
bool Course::updateSpecificYearCourse(std::vector<std::string> &specificYearCourse) {
    std::string acYear = specificYearCourse[1];
    std::stringstream ss(acYear);
    int startYear, endYear;
    char c;
    bool isActive;
    int numParallel;
    ss >> startYear >> c >> endYear;
    if (!_courseOfTheYear.count(startYear))
        throw std::invalid_argument("anno già esistente");
    ////
    SpecificYearCourse lastYearSpecificCourse = getLastSpecificYearCourse();
    std::stringstream last;
    last << lastYearSpecificCourse;
    //splittare last
    for( int i = 2; i<specificYearCourse.size();i++){
        std::string token = specificYearCourse[i];
        if(token.empty()) {
            switch (i){
                case active_empty:{
                    //isActive = lastYearSpecificCourse.isActive();
                    break;
                }
                case num_par_empty:{
                    //numParallel = lastYearSpecificCourse.getParalleleCours();
                    break;
                }
                case prof_empty:{
                    break;
                }
                case info_exam_empty:{
                    break;
                }
                case par_cod_empty:{
                    break;
                }

            }

        }
    }
    ////
    _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(startYear, newCourse));


    return false;
}

SpecificYearCourse& Course::getLastSpecificYearCourse() {
    int lastYear = 0, actualYear;
    for (auto iter = _courseOfTheYear.begin(); iter != _courseOfTheYear.end(); iter++) {
        actualYear = iter->second.getStartYear();
        if (actualYear > lastYear)
            lastYear = actualYear;
    }
    return (_courseOfTheYear.at(lastYear));
}




