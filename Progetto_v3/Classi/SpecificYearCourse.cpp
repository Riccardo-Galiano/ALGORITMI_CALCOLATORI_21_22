//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "SpecificYearCourse.h"

using namespace std;

SpecificYearCourse::SpecificYearCourse(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,std::vector<std::string> exam, std::vector<std::string> idPar) : _exam(stoi(exam[0]), stoi(exam[1]), stoi(exam[2]), exam[3], exam[4]) {
    stringstream acYY(sY_eY);//manipolo la stringa dell'anno accademico
    char c;
    acYY >> _startYear >> c >> _endYear;//anno iniziale - anno finale
    _active = active;
    _paralleleCours = nCrsiPar;
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
    profSenzaQuadre = profs.substr(found + 2, (profs.size() - found - 2) - 1); //tolgo le [ ] che delimitano i vari prof con le relative informazioni
    vector<int> foundBracket;

    std::size_t posB = profSenzaQuadre.find_first_of("{}");
    bool toContinue = true;
    while (toContinue) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
        if (posB == std::string::npos)
            toContinue = false;
        else {
            foundBracket.push_back( posB);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            posB = profSenzaQuadre.find_first_of("{}", posB + 1);//continuo a controllare la stringa
        }
    }

    ///controllare che legga giusto
    for (int i = 0; i < foundBracket.size(); i += 2) {
        singoliProfDaLeggere.push_back( profSenzaQuadre.substr(foundBracket[i] + 1, foundBracket[i + 1] - foundBracket[i] - 1));

    }
    int id, hlez, hexe, hlab;
    for (int i = 0; i < singoliProfDaLeggere.size(); i++) {//inserisco le info per ogni prof
        stringstream ss(singoliProfDaLeggere[i]);
        ss >> c >> id >> c >> hlez >> c >> hexe >> c >> hlab; //d interoId , oreLezione , oreEsercitazione , oreLaboratorio
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

bool SpecificYearCourse::setProfMap(int numCorsiPar, std::vector<std::string> profsToSplit, std::vector<std::string> idCorso) {

    std::vector<professor> profConOre;
    for (int i = 0; i < numCorsiPar; i++) {//per ogni corso in parallelo vado ad inserire i prof con le loro informazioni
        profConOre = getProfsFromString(profsToSplit[i]);//mi ritorna il vettore in cui ad ogni posizione c'è un prof, con le sue informazioni,per ogni corso in parallelo
        _professors.insert(pair<string, vector<professor>>(idCorso[i], profConOre));//ad ogni key (id del corso in parallelo) verrà associato un vettore con i prof che ne fano parte
    }
    return false;
}
