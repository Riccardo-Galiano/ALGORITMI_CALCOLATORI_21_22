//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "Course.h"
std::vector<std::string> getProfPar1(std::string &input, int num_parallel_courses, std::vector<int> &posCBrackets) {
    std::vector<std::string> profCorsiPar;
    std::vector<int> posFinCorsiPar;
    int found = 0;
    for (int i = 0; i < posCBrackets.size() - 1 && found <
                                                   num_parallel_courses; i++) {//fino a quando non finiscono le parentesi graffe o il numero di corsi in parallelo
        if (input[posCBrackets[i] + 1] ==
            ']') {//la ricorrenza }] si ha alla fine di ogni corso in parallelo; se dopo una parentesi graffa si ha una quadra il corso in parallelo è finito
            posFinCorsiPar.push_back(
                    posCBrackets[i] + 2); //salva la pos della graffa successiva a quella presa in analisi: }] } <--
            found++;
        }
    }

    int lastPosFin = 0;

    ///divido le info dei vari corsi in parallelo
    for (int i = 0; i < num_parallel_courses; i++) {
        profCorsiPar.push_back(input.substr(1 + lastPosFin, posFinCorsiPar[i] - lastPosFin -
                                                            1));//inserisco nel vettore di stringhe le info del corso in parallelo
        lastPosFin = posFinCorsiPar[i] + 2; //salva la pos della terza graffa: }]}, { <--
    }
    return profCorsiPar;
};

std::vector<int> posCurlyBrackets1(std::string &input) {
    std::vector<int> output;
    std::size_t found = input.find_first_of("{}");
    while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
        output.push_back(
                found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
        found = input.find_first_of("{}", found + 1);//continuo a controllare la stringa
    }
    return output;
}
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


enum {
    active_empty = 2, num_par_empty = 3, prof_empty = 4, info_exam_empty = 5, par_cod_empty = 6
};

bool Course::fillSpecificYearCourse(std::vector<std::string> &specificYearCourse) {
    std::string acYear = specificYearCourse[1];
    std::stringstream ss(acYear);
    int startYear, endYear;
    char c;
    ss >> startYear >> c >> endYear;
    if (_courseOfTheYear.find(startYear) != _courseOfTheYear.end())
        throw std::invalid_argument("anno già esistente");
    ////
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




