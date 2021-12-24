//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "SpecificYearCourse.h"
#include "InvalidDbException.h"


SpecificYearCourse::SpecificYearCourse(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,
                                       std::vector<std::string> exam, std::vector<std::string> idGrouped, std::string yy_semester,std::vector<int> studyCourse, int line_counter) :
        _exam(stoi(exam[0]), stoi(exam[1]), stoi(exam[2]), exam[3], exam[4]) {
    std::stringstream acYY(sY_eY);//manipolo la stringa dell'anno accademico
    char c;
    acYY >> _startYear >> c >> _endYear;//anno iniziale - anno finale
    _active = active;
    _parallelCourses = nCrsiPar;
    _idGroupedCourses = idGrouped;
    _yy_semester = yy_semester;
    _studyCourseAssigned = studyCourse;
    setProfMap(nCrsiPar, prof, line_counter);//setto la mappa dei prof per ogni corso

}


///setta la map dei prof con relativi id e ore
//ogni anno accademico per ogni corso presenta una map che contiene tutte le info raccolte per i prof di ogni corso in parallelo
bool SpecificYearCourse::setProfMap(int numCorsiPar, std::vector<std::string> profsToSplit, int line_counter) {

    std::vector<professor> profConOre;
    for (int i = 0; i < numCorsiPar; i++) {//per ogni corso in parallelo vado ad inserire i prof con le loro informazioni
        profConOre = getProfsFromString(profsToSplit[i],line_counter);//mi ritorna il vettore in cui ad ogni posizione c'è un prof, con le sue informazioni,per ogni corso in parallelo
        _professors.insert(std::pair<int, std::vector<professor>>(i,profConOre));//ad ogni key (id del corso in parallelo) verrà associato un vettore con i prof che ne fano parte
    }

    return false;
}

///setta l'Id aggiungendo degli 0 all'inizio, dove necessario
std::string SpecificYearCourse::setId(int nMatr) const {
    std::stringstream output;
    output << std::setfill('0') << std::setw(6) << nMatr;
    return output.str();
}

///set dell'anno al successivo
bool SpecificYearCourse::setYear() {
    this->_startYear++;
    this->_endYear++;
    return true;
}

///anno di inizio dell'anno accademico
int SpecificYearCourse::getStartYear() const {
    return _startYear;
}

///anno di fine dell'anno accademico
int SpecificYearCourse::getEndYear() const {
    return _endYear;
}

///ritorna se il corso è attivo
bool SpecificYearCourse::getisActive() const {
    return _active;
}

/// ritorna il numero di corsi in parallelo
int SpecificYearCourse::getParalleleCours() const {
    return _parallelCourses;
}

///scinde le varie info per ogni prof e li mette in un vettore di struct professor
std::vector<professor> SpecificYearCourse::getProfsFromString(std::string profs,int line_counter) {
    int mainProf; //titolare del corso
    std::vector<std::string> singoliProfDaLeggere;
    std::vector<professor> profToReturn;
    std::string profSenzaQuadre;
    std::string idMainProf;
    char c;
    int found = profs.find_first_of(","); //cerca la prima virgola
    idMainProf = profs.substr(0, found);//salva l'id del titolare
    std::stringstream ssIdProf(idMainProf);//manipolo l'id per togliere la "d" e avere solo un intero
    ssIdProf >> c >> mainProf;
    profSenzaQuadre = profs.substr(found + 2, (profs.size() - found - 2) -
                                              1); //tolgo le [ ] che delimitano i vari prof con le relative informazioni
    std::vector<int> foundBracket;

    ///cerco e salvo le posizioni di { o } in profSenzaQuadre
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
    ///estrae ogni prof con le relative info
    for (int i = 0; i < foundBracket.size(); i += 2) {
        singoliProfDaLeggere.push_back(
                profSenzaQuadre.substr(foundBracket[i] + 1, foundBracket[i + 1] - foundBracket[i] - 1));

    }
    int id=-1, hlez=-1, hexe=-1, hlab=-1;
    bool mainProfFound=false;
    for (int i = 0; i < singoliProfDaLeggere.size(); i++) {//inserisco le info per ogni prof
        std::stringstream ss(singoliProfDaLeggere[i]);
        ss >> c >> id >> c >> hlez >> c >> hexe >> c >> hlab; //d interoId , oreLezione , oreEsercitazione , oreLaboratorio
        if(id == -1 || hlez == -1 || hexe == -1 || hlab == -1)
            throw InvalidDbException("ore professore non valide alla riga:",line_counter);
        professor p{};//struct
        p.prof_id = id;
        p.hLez = hlez;
        p.hExe = hexe;
        p.hLab = hlab;
        if (p.prof_id == mainProf) {//se l'id analizzato uguale a quello del titolare letto in precedenza
            p.mainProf = true;
            mainProfFound=true;
        }
        profToReturn.push_back(p);//aggiunge una struct professor al vettore di struct professor
    }
    if(mainProfFound==false){
        throw InvalidDbException("manca il professore titolare alla riga: ", line_counter);
    }

    return profToReturn;
}

///ritorna sottoforma di stringa i professori e le relative info in un corso
const std::string SpecificYearCourse::getProfParString() const {
    std::stringstream output;

    output << "[";
    int matrTit = 0;
    std::vector<std::string> profsString;
    for (int i = 0; i < _parallelCourses; i++) {
        output << "{";
        std::vector<professor> profs = _professors.at(i);
        for (int j = 0; j < profs.size(); j++) {
            if (profs[j].mainProf)
                matrTit = profs[j].prof_id;
        }
        output << "d" << setId(matrTit) << ",[";
        for (int j = 0; j < profs.size(); j++) {
            output << "{d" << setId(profs[j].prof_id) << "," << profs[j].hLez << "," << profs[j].hExe << ","
                   << profs[j].hLab << "}";
            if (j < profs.size() - 1)
                output << ",";
        }
        output << "]}";
        if (i < _parallelCourses - 1)
            output << ",";
    }
    output << "]";
    return output.str();
}

///prende tutte le matricole di tutti i professori per un corso di uno specifico anno per tutti i corsi paralleli
std::vector<int> SpecificYearCourse::getAllProfMatr() {
    std::vector<int> professors;
    std::vector<professor> profs;
    for (int i = 0; i < _professors.size(); i++) {
        profs = _professors.at(i);//prende il vettore di prof per l'iesimo corso parallelo
        for (int j = 0; j < profs.size(); j++){
            professors.push_back(profs[j].prof_id);//prende l'id di ogni prof dell'iesimo corso parallelo
        }
    }
    return professors;
}

///ritorna i prof per i corsi paralleli
const std::map<int, std::vector<professor>> SpecificYearCourse::getProfsOfParallelCourses() const {
    return _professors;
}

///ritorna gli id dei corsi raggruppati
const std::vector<std::string> &SpecificYearCourse::getIdGroupedCourses() const {
    return _idGroupedCourses;
}

///ritorna sottoforma di stringa i codici dei corsi raggruppati ad un corso
const std::string SpecificYearCourse::getGroupedCoursesIdString() const {
    std::stringstream output;
    output << "{";
    for (int i = 0; i < _idGroupedCourses.size(); i++) {
        output << _idGroupedCourses[i];
        if (i < _idGroupedCourses.size() - 1)
            output << ",";
    }
    output << "}";
    return output.str();
}

///ritorna sottoforma di stringa le info per l'esame di un corso
const std::string SpecificYearCourse::getExamString() const {

    std::stringstream output;
    output << "{" << _exam.getTime() << "," << _exam.getEnterTime() << "," << _exam.getLeaveTime() << ","
           << _exam.getMode() << "," << _exam.getPlace() << "}";

    return output.str();
}

///prende l'esame
const Exam &SpecificYearCourse::getExam() const {
    return _exam;
}

///ritorna il semestre di un corso di studio a cui è associato il corso
int SpecificYearCourse::getSemester() const {
    return stoi(_yy_semester.substr(2,1));
}

///ritorna l'anno di un corso di studio a cui è associato il corso
int SpecificYearCourse::getYearOfTheSemester() const {
    return stoi(_yy_semester.substr(0,1));
}

///ritorna i corsi di studio a cui un corso è associato
std::vector<int> SpecificYearCourse::getStudyCourseAssigned() const {
    return _studyCourseAssigned;
}

///ritorna se è dello stesso semestre ed è attivo
bool SpecificYearCourse::canIBeAssigneToFirstTwoWeekOfExamSession(int semesterGiven) const {
    if(semesterGiven == this->getSemester() && _active)
        return true;
    else
        return false;
}

///ritorna quanti appelli ho già assegnato
int SpecificYearCourse::amIAssignedAlreadyInThisSession(int session) {
    if(_howManyTimesIAmAssignedInASession.count(session)==0)
        return 0;
    return _howManyTimesIAmAssignedInASession.at(session).size();
}

///ritorna la data dell'appello precedentemente assegnato per un corso
Date SpecificYearCourse::lastDateAssignationInGivenSession(int session) {
    //ATTENZIONE: non può essere chiamata se amIAssignedAlreadyInThisSession(session) == 0
    if(amIAssignedAlreadyInThisSession(session) == 0){
        throw std::exception();
    }
    return _howManyTimesIAmAssignedInASession.at(session)[0];
}

/// Segna che è stato assegnato un esame per questo corso ad una certa data
bool SpecificYearCourse::assignExamInThisSpecificYearCourse(Date examDay,int session) {
    std::vector<Date>vectorOfExamDays;
    vectorOfExamDays.push_back(examDay);
    if(_howManyTimesIAmAssignedInASession.count(session)==0){
        _howManyTimesIAmAssignedInASession.insert(std::pair <int, std::vector<Date>> (session,vectorOfExamDays));
    }else if(_howManyTimesIAmAssignedInASession.count(session)==1){
        _howManyTimesIAmAssignedInASession.at(session).push_back(examDay);
    }
    return true;
}

///aggiunge uno studente
bool SpecificYearCourse::addStudent(Student stud, std::string enrolYear, int mark) {
    student studToAdd;
    studToAdd._grade = mark;
    studToAdd._startEnrolYear = stoi(enrolYear.substr(0, 4));
    studToAdd._endEnrolYear = stoi(enrolYear.substr(5, 4));
    studToAdd.stud = stud;
    std::pair<int, student> pair(stud.getId(), studToAdd);
    _student.insert(pair);
    totStudentsEnrolled++;
    if (mark == -1) //da controllare da stringa vuota a int cosa succede
        totStudentsNotPassed++;
    return true;
}


std::ostream &operator<<(std::ostream &output, const SpecificYearCourse &s) {
    output << s.getStartYear() << "-" << s.getEndYear() << ";";
    if (s.getisActive())
        output << "attivo";
    else
        output << "non_attivo";
    output << ";" << s.getParalleleCours() << ";";
    output << s.getProfParString() << ";";
    output << s.getExamString() << ";";
    output << s.getGroupedCoursesIdString();
    return output;
}