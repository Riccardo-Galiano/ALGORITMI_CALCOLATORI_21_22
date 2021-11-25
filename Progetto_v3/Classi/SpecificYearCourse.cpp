//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "SpecificYearCourse.h"

using namespace std;

std::ostream &operator<<(std::ostream &output, const SpecificYearCourse &s) {
    output << s.getStartYear() << "-" << s.getEndYear() << ";";
    if (s.getisActive())
        output << "attivo";
    else
        output << "non_attivo";
    output << ";" << s.getParalleleCours() << ";";
    output << s.getProfParSTring() << ";";
    output << s.getExamString() << ";";
    output << s.getParCourseIdString() << ";";
    return output;
}

SpecificYearCourse::SpecificYearCourse(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,
                                       std::vector<std::string> exam, std::vector<std::string> idPar) :
        _exam(stoi(exam[0]), stoi(exam[1]), stoi(exam[2]), exam[3], exam[4]) {
    stringstream acYY(sY_eY);//manipolo la stringa dell'anno accademico
    char c;
    acYY >> _startYear >> c >> _endYear;//anno iniziale - anno finale
    _active = active;
    _paralleleCours = nCrsiPar;
    _idPar = idPar;
    setProfMap(nCrsiPar, prof, idPar);//setto la mappa dei prof per ogni corso
    //_exam = Exam(stoi(exam[0]),stoi(exam[1]),stoi(exam[2]),exam[3],exam[4]);
}

std::vector<professor> SpecificYearCourse::getProfsFromString(std::string profs) {
    int mainProf;
    vector<string> singoliProfDaLeggere;
    vector<professor> profToReturn;
    std::string profSenzaQuadre;
    std::string idMainProf;
    char c;
    int found = profs.find_first_of(","); //cerca la prima virgola
    idMainProf = profs.substr(0, found);//salva l'id del titolare
    std::stringstream ssIdProf(idMainProf);//manipolo l'id per togliere la "d" e avere solo un intero
    ssIdProf >> c >> mainProf;
    profSenzaQuadre = profs.substr(found + 2, (profs.size() - found - 2) -
                                              1); //tolgo le [ ] che delimitano i vari prof con le relative informazioni
    vector<int> foundBracket;

    std::size_t posB = profSenzaQuadre.find_first_of("{}");
    bool toContinue = true;
    while (toContinue) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
        if (posB == std::string::npos)
            toContinue = false;
        else {
            foundBracket.push_back(
                    posB);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            posB = profSenzaQuadre.find_first_of("{}", posB + 1);//continuo a controllare la stringa
        }
    }

    ///controllare che legga giusto
    for (int i = 0; i < foundBracket.size(); i += 2) {
        singoliProfDaLeggere.push_back(
                profSenzaQuadre.substr(foundBracket[i] + 1, foundBracket[i + 1] - foundBracket[i] - 1));

    }
    int id, hlez, hexe, hlab;
    for (int i = 0; i < singoliProfDaLeggere.size(); i++) {//inserisco le info per ogni prof
        stringstream ss(singoliProfDaLeggere[i]);
        ss >> c >> id >> c >> hlez >> c >> hexe >> c
           >> hlab; //d interoId , oreLezione , oreEsercitazione , oreLaboratorio
        professor p{};//struct
        p.prof_id = id;
        p.hLez = hlez;
        p.hExe = hexe;
        p.hLab = hlab;
        if (p.prof_id == mainProf) {
            p.mainProf = true;
        }
        profToReturn.push_back(p);
    }

    return profToReturn;
}

bool SpecificYearCourse::setProfMap(int numCorsiPar, std::vector<std::string> profsToSplit,
                                    std::vector<std::string> idCorso) {

    std::vector<professor> profConOre;
    for (int i = 0;
         i < numCorsiPar; i++) {//per ogni corso in parallelo vado ad inserire i prof con le loro informazioni
        profConOre = getProfsFromString(
                profsToSplit[i]);//mi ritorna il vettore in cui ad ogni posizione c'è un prof, con le sue informazioni,per ogni corso in parallelo
        _professors.insert(pair<string, vector<professor>>(idCorso[i],
                                                           profConOre));//ad ogni key (id del corso in parallelo) verrà associato un vettore con i prof che ne fano parte
    }
    return false;
}

const int SpecificYearCourse::getNumParallel() const {
    return _paralleleCours;
}

int SpecificYearCourse::getStartYear() const {
    return _startYear;
}

int SpecificYearCourse::getEndYear() const {
    return _endYear;
}

bool SpecificYearCourse::getisActive() const {
    return _active;
}

int SpecificYearCourse::getParalleleCours() const {
    return _paralleleCours;
}

const Exam &SpecificYearCourse::getExam() const {
    return _exam;
}

const map<std::string, std::vector<professor>> &SpecificYearCourse::getProfessors() const {
    return _professors;
}

const map<std::string, std::vector<student>> &SpecificYearCourse::getStudent() const {
    return _student;
}

const std::string SpecificYearCourse::getProfParSTring() const {
    std::stringstream output;

    output << "[";
    int matrTit;
    std::vector<std::string> profsString;
    for (int i = 0; i < _paralleleCours; i++) {
        output << "{";
        std::vector<professor> profs = _professors.at(_idPar[i]);
        for (int j = 0; j < profs.size(); j++) {
            if (profs[j].mainProf)
                matrTit = profs[j].prof_id;
        }
        output << "d" << matrTit << ",[";
        for (int j = 0; j < profs.size(); j++) {
            output << "{d" << profs[j].prof_id << "," << profs[j].hLez << "," << profs[j].hExe << "," << profs[j].hLab
                   << "}";
            if (j < profs.size() - 1)
                output << ",";
        }
        output << "]}";
        if (i < _paralleleCours - 1)
            output << ",";
    }
    output << "]";
    return output.str();
}

const std::string SpecificYearCourse::getParCourseIdString() const {
    std::stringstream output;
    output << "{";
    for (int i = 0; i < _idPar.size(); i++) {
        output << _idPar[i];
        if (i < _idPar.size() - 1)
            output << ",";
    }
    output << "}";
    return output.str();
}

const std::string SpecificYearCourse::getExamString() const {

    std::stringstream output;
    output << "{" << _exam.getTime() << "," << _exam.getEnterTime() << "," << _exam.getLeaveTime() << ","
           << _exam.getMode() << "," << _exam.getPlace() << "}";

    return output.str();
}


