//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include <algorithm>
#include "SpecificYearCourse.h"
#include "InvalidDbException.h"
#include "Parse.hpp"


SpecificYearCourse::SpecificYearCourse(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,
                                       std::vector<std::string> exam, std::vector<std::string> idGrouped,
                                       std::string yy_semester, std::vector<int> studyCourse, int line_counter) :
    _exam(stoi(exam[0]), stoi(exam[1]), stoi(exam[2]), exam[3], exam[4]) {
    std::stringstream acYY(sY_eY);//manipolo la stringa dell'anno accademico
    char c;
    acYY >> _startYear >> c >> _endYear;//anno iniziale - anno finale
    _active = active;
    _parallelCourses = nCrsiPar;
    _idGroupedCourses = idGrouped;
    _yy_semester = yy_semester;
    _studyCourseAssigned = studyCourse;
    _numAppeal = 0;
    _totStudentsEnrolled = 0;
    _totStudentsNotPassed = 0;
    setProfMap(nCrsiPar, prof, line_counter);//setto la mappa dei prof per ogni corso

}


///setta la map dei prof con relativi id e ore
//ogni anno accademico per ogni corso presenta una map che contiene tutte le info raccolte per i prof di ogni corso in parallelo
bool SpecificYearCourse::setProfMap(int numCorsiPar, std::vector<std::string> profsToSplit, int line_counter) {

    std::vector<professor> profConOre;
    for (int i = 0;
         i < numCorsiPar; i++) {//per ogni corso in parallelo vado ad inserire i prof con le loro informazioni
        profConOre = getProfsFromString(profsToSplit[i],line_counter);//mi ritorna il vettore in cui ad ogni posizione c'è un prof, con le sue informazioni,per ogni corso in parallelo
        _professors.insert(std::pair<int, std::vector<professor>>(i,profConOre));//ad ogni key (id del corso in parallelo) verrà associato un vettore con i prof che ne fano parte
    }

    return false;
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
int SpecificYearCourse::getNumParallelCourses() const {
    return _parallelCourses;
}

///scinde le varie info per ogni prof e li mette in un vettore di struct professor
std::vector<professor> SpecificYearCourse::getProfsFromString(std::string profs, int line_counter) {
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
    int id = -1, hlez = -1, hexe = -1, hlab = -1;
    bool mainProfFound = false;
    for (int i = 0; i < singoliProfDaLeggere.size(); i++) {//inserisco le info per ogni prof
        std::stringstream ss(singoliProfDaLeggere[i]);
        ss >> c >> id >> c >> hlez >> c >> hexe >> c
           >> hlab; //d interoId , oreLezione , oreEsercitazione , oreLaboratorio
        if (id == -1 || hlez == -1 || hexe == -1 || hlab == -1)
            throw std::invalid_argument("ore professore non valide alla riga:" + std::to_string(line_counter));
        professor p{};//struct
        p.prof_id = id;
        p.hLez = hlez;
        p.hExe = hexe;
        p.hLab = hlab;
        if (p.prof_id == mainProf) {//se l'id analizzato uguale a quello del titolare letto in precedenza
            p.mainProf = true;
            mainProfFound = true;
        }
        profToReturn.push_back(p);//aggiunge una struct professor al vettore di struct professor
    }
    if (mainProfFound == false) {
         throw std::invalid_argument("manca il professore titolare alla riga: " + std::to_string(line_counter));
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

        output << Parse::setId('d',6,matrTit) << ",[";
        for (int j = 0; j < profs.size(); j++) {
            output << "{" << Parse::setId('d',6,profs[j].prof_id) << "," << profs[j].hLez << "," << profs[j].hExe << ","
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
        for (int j = 0; j < profs.size(); j++) {
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
    if (_yy_semester.empty())
        //non ancora assegnato ad un semestre
        return -1;
    return stoi(_yy_semester.substr(2, 1));
}

///ritorna l'anno di un corso di studio a cui è associato il corso
int SpecificYearCourse::getYearOfTheSemester() const {
    return stoi(_yy_semester.substr(0, 1));
}

///ritorna i corsi di studio a cui un corso è associato
std::vector<int> SpecificYearCourse::getStudyCourseAssigned() const {
    return _studyCourseAssigned;
}

///ritorna se è dello stesso semestre ed è attivo
bool SpecificYearCourse::canIBeAssigneToFirstTwoWeekOfExamSession(int semesterGiven) const {
    if (semesterGiven == this->getSemester() && _active)
        return true;
    else
        return false;
}

///ritorna quanti appelli ho già assegnato
int SpecificYearCourse::amIAssignedAlreadyInThisSession(int session) {
    if (_howManyTimesIAmAssignedInASession.count(session) == 0)
        return 0;
    return _howManyTimesIAmAssignedInASession.at(session).size();
}

///ritorna la data dell'appello precedentemente assegnato per un corso
Date SpecificYearCourse::lastDateAssignationInGivenSession(int session) {
    //ritorna 1900-01-01 se non trova la data del primo appello
    if (amIAssignedAlreadyInThisSession(session) == 0) {
        Date dateDefault = Date();
        return dateDefault;
    }
    return _howManyTimesIAmAssignedInASession.at(session)[0];
}

/// Segna che è stato assegnato un esame per questo corso ad una certa data
bool SpecificYearCourse::assignExamInThisSpecificYearCourse(Date examDay, int session) {
    std::vector<Date> vectorOfExamDays;
    vectorOfExamDays.push_back(examDay);
    if (_howManyTimesIAmAssignedInASession.count(session) == 0) {
        _howManyTimesIAmAssignedInASession.insert(std::pair<int, std::vector<Date>>(session, vectorOfExamDays));
    } else if (_howManyTimesIAmAssignedInASession.count(session) == 1) {
        _howManyTimesIAmAssignedInASession.at(session).push_back(examDay);
    }
    return true;
}

///aggiunge il voto ad uno studente
bool SpecificYearCourse::addGradeToStudent(Student &stud, int passYear, int mark, std::string appealsDate, std::string idCourse) {

    student &studToUpdate = _studentsEnrolled.at(stud.getId());
    studToUpdate._grade = mark;
    if (mark >= 18 && mark <= 32) {
        studToUpdate._passYear = passYear;
        _totStudentsNotPassed--;
        studToUpdate._passed = true;
        studToUpdate._appealPassed = appealsDate;
    }
    return true;
}

bool SpecificYearCourse::assignYY_SemToAllYear(std::string &acYYoff, std::string &yy_semester) {
    _yy_semester = yy_semester;
    _acYearOff = acYYoff;
    return true;
}

void SpecificYearCourse::setNewYear(int newStartYear) {
    _startYear = newStartYear;
    _endYear = newStartYear + 1;
    _howManyTimesIAmAssignedInASession.erase(_howManyTimesIAmAssignedInASession.begin(),_howManyTimesIAmAssignedInASession.end());
    _roomsEachAppeal.erase(_roomsEachAppeal.begin(),_roomsEachAppeal.end());
    _startSlotPerEachAppeal.erase(_startSlotPerEachAppeal.begin(),_startSlotPerEachAppeal.end());
    _numAppeal = 0;
}

std::string &SpecificYearCourse::getAcYearOff() {
    return _acYearOff;
}

int SpecificYearCourse::getTotStudentsExam() {
    return _totStudentsNotPassed; //(tot enroled - (tot enroled-totNot))
}

bool SpecificYearCourse::addClassroomsToAppeal(int numAppeal, std::vector<int> &rooms) {
    _roomsEachAppeal.insert(std::pair<int, std::vector<int>>(numAppeal, rooms));
    return true;
}

int SpecificYearCourse::getNumNextAppeal() {
    int tot = 0;
    auto start = _howManyTimesIAmAssignedInASession.begin();
    auto end = _howManyTimesIAmAssignedInASession.end();
    for (auto iter = start; iter != end; iter++) {
        tot += iter->second.size();
    }
    return tot;
}

bool SpecificYearCourse::addStudent(int acYearRegistration, Student &stud) {
    std::string appealsInitialization = "1900-01-01";

    student studToAdd;
    studToAdd._studId = stud.getId();
    studToAdd._grade = -1;
    studToAdd._startEnrolYear = acYearRegistration;
    studToAdd._passYear = -1;
    studToAdd._appealPassed = appealsInitialization;
    _totStudentsEnrolled++;
    _totStudentsNotPassed++;
    studToAdd._passed = false;
    std::pair<int, student> pair(stud.getId(), studToAdd);
    _studentsEnrolled.insert(pair);
    return true;
}


int SpecificYearCourse::getTotStudentsEnrolled() const {
    return _totStudentsEnrolled;
}

const std::map<int, std::vector<Date>> &SpecificYearCourse::getHowManyTimesIAmAssignedInASession() const {
    return _howManyTimesIAmAssignedInASession;
}


std::vector<Date> SpecificYearCourse::getAllAppeals() const {
    std::vector<Date> allAppeals;
    std::map<int, std::vector<Date>> allSessionPerYear = getHowManyTimesIAmAssignedInASession();
    for (auto iterSession = allSessionPerYear.begin(); iterSession != allSessionPerYear.end(); iterSession++) {
        allAppeals.insert(allAppeals.begin(), iterSession->second.begin(), iterSession->second.end());
    }
    return allAppeals;
}

bool SpecificYearCourse::assignAllStudsPassedExam(std::vector<std::pair<std::string, int>> allStudPassedExam,
                                                  std::string appealDate) {
    for (int i = 0; i < allStudPassedExam.size(); i++) {
        int id = Parse::getMatr(allStudPassedExam[i].first);
        int passYear = stoi(appealDate.substr(0, 4));
        student &stud = _studentsEnrolled.at(id);
        stud._passed = true;
        stud._grade = allStudPassedExam[i].second;
        stud._appealPassed = appealDate;
        stud._passYear = passYear;
    }
    return false;
}

std::string SpecificYearCourse::getAppealsForAllSession() {
    std::stringstream ss;
    int numAppeals = 0;
    for (auto iterSessionAppeals = _howManyTimesIAmAssignedInASession.begin();
         iterSessionAppeals != _howManyTimesIAmAssignedInASession.end(); iterSessionAppeals++) {
        std::vector<Date> appeals = iterSessionAppeals->second;
        switch (iterSessionAppeals->first) {
            case 1: {
                ss << "winter[";
                break;
            }
            case 2: {
                ss << "summer[";
                break;
            }
            case 3: {
                ss << "autumn[";
                break;
            }
            default:
                break;
        }
        for (int i = 0; i < appeals.size(); i++) {
            ss << "{";
            ss << appeals[i] << "," << getStartHourAppeal(numAppeals) << "," << getRoomsPerAppealsString(numAppeals);
            ss << "}";
            if (i < appeals.size() - 1)
                ss << ",";
            numAppeals++;
        }
        ss << "]";
        if (iterSessionAppeals->first < 3)
            ss << "%";
    }

    return ss.str();
}

bool SpecificYearCourse::assignAppeals(std::string session, std::vector<Date> appealPerSession,std::vector<int> startSlotPerAppeal,std::vector<std::string> classroomsPerAppeal) {


    for(int i = 0; i<startSlotPerAppeal.size(); i++) {
        _startSlotPerEachAppeal.insert(std::pair<int, int>(getNumAppeal(), startSlotPerAppeal[i]));
        std::vector<std::string> allClassroomsPerAppeal = Parse::splittedLine(classroomsPerAppeal[i], '|');
        std::vector<int> allClassroomsPerAppealInt;
        //trasformo il vettore di aule string in un vettore di aule interi
        for(int j = 0; j<allClassroomsPerAppeal.size(); j++){
            int room = Parse::checkedStoi(allClassroomsPerAppeal[j],"Errore id Aula");
            allClassroomsPerAppealInt.push_back(room);
        }
        _roomsEachAppeal.insert(std::pair<int,std::vector<int>>(getNumAppeal(),allClassroomsPerAppealInt));
        _numAppeal++;
    }
    if (session == "winter")
        _howManyTimesIAmAssignedInASession.insert(std::pair<int, std::vector<Date>>(1, appealPerSession));
    else if (session == "summer")
        _howManyTimesIAmAssignedInASession.insert(std::pair<int, std::vector<Date>>(2, appealPerSession));
    else if (session == "autumn")
        _howManyTimesIAmAssignedInASession.insert(std::pair<int, std::vector<Date>>(3, appealPerSession));
    return true;
}

std::vector<int> SpecificYearCourse::getRoomsAppeal() {
    std::vector<int> rooms = _roomsEachAppeal.at(getNumAppeal());
    _numAppeal++;
    return rooms;
}

bool SpecificYearCourse::notExamsAssigned() {
    return _howManyTimesIAmAssignedInASession.empty();
}

std::map<int, student> SpecificYearCourse::getStudentsPassedInThisAppeal(Date dateAppeal) {
    std::map<int, student> allStudentsPassed;
    for (auto iterStud = _studentsEnrolled.begin(); iterStud != _studentsEnrolled.end(); iterStud++) {
        student currentStud = iterStud->second;
        if (currentStud._passed == true) {
            Date appealPassed = currentStud._appealPassed;
            if (appealPassed == dateAppeal) {
                allStudentsPassed.insert(std::pair<int, student>(currentStud._studId, currentStud));
            }
        }
    }
    return allStudentsPassed;
}

///inserisco tutti i corsi raggruppati nello spYY e cancello il suo id
void
SpecificYearCourse::assignGrouped(std::vector<std::string> &idGrouped, std::string &idCourse, std::string &thisCourse) {
    _idGroupedCourses = idGrouped;
    _idGroupedCourses.push_back(idCourse);
    auto pos = std::find(_idGroupedCourses.begin(), _idGroupedCourses.end(), thisCourse);
    _idGroupedCourses.erase(pos);
}

std::string SpecificYearCourse::getRoomsPerAppealsString(int numAppeals) {
    std::stringstream ss;
    std::vector<int> roomsPerAppeals = _roomsEachAppeal.at(numAppeals);
    for (int i = 0; i < roomsPerAppeals.size(); i++) {
        ss << roomsPerAppeals[i];
        if (i < roomsPerAppeals.size() - 1)
            ss << "|";
    }
    return ss.str();
}

bool SpecificYearCourse::addStartSlotToAppeal(int numAppeal, int startExamHour) {
    _startSlotPerEachAppeal.insert(std::pair<int, int>(numAppeal, startExamHour));
    return true;
}

int SpecificYearCourse::getStartHourAppeal(int numAppeals) {
    return _startSlotPerEachAppeal.at(numAppeals);
}

void SpecificYearCourse::setYySemester(const std::string &yySemester) {
    _yy_semester = yySemester;
}

char SpecificYearCourse::getPlaceExam() {
    return _exam.getPlace();
}

Date &SpecificYearCourse::dateAssignationInGivenSession(int numSession, int numAppeal) {
    return _howManyTimesIAmAssignedInASession.at(numSession)[numAppeal-1];
}

int SpecificYearCourse::startSlotAssignationInGivenSession(int numSession, int numAppeal) {
    int numAppealPerYear = getNumAppealFromNumSessNumAppealInSession(numSession,numAppeal);
    return _startSlotPerEachAppeal.at(numAppealPerYear);
}

std::vector<int> SpecificYearCourse::classroomsAssignedInGivenSession(int numSession, int numAppeal) {
    int numAppealPerYear = getNumAppealFromNumSessNumAppealInSession(numSession,numAppeal);
    return _roomsEachAppeal.at(numAppealPerYear);
}

int SpecificYearCourse::getNumAppealFromNumSessNumAppealInSession(int numSession, int numAppeal){
    int numAppealYear = 0;
    if(_active) {
        if (numSession == 3)
            //se autunno sarà l'ultimo appello dell'anno (3)
            numAppealYear = 3;
        else if (numSession == 1)
            //se inverno sarà o primo (0) o, se c'è un secondo appello (1)
            //numAppealYear: 1-1 = 0 ; 2-1 = 1
            numAppealYear = numAppeal - 1;
        else if (numSession == 2) {
            //se estiva bisogna distinguere il caso in cui la sessione abbia 1 o 2 appelli
            //se ne ha uno vuol dire che numAppealYear = 2 (perchè quella invernale avr
            //se ne ha due vuol dire che numAppealYear può essere 1 o 2
            int sizeSession = _howManyTimesIAmAssignedInASession.count(numSession - 1); //1 oppure 2
            if (sizeSession == 1) {
                numAppealYear = 2;
            } else
                numAppealYear = numAppeal;
        }
    } else
        numAppealYear = numSession;
    return numAppealYear;
}

void SpecificYearCourse::removeInfoThisAppealFromSpecificYear(int numSession, int numAppeal) {
    ///pop da vettore date
    if(numAppeal == 2)
        ///se secondo appello tolgo l'ultimo del vettore date
        _howManyTimesIAmAssignedInASession.at(numSession).pop_back();
    else {
        ///se primo appello
        std::vector<Date> dateInThisSession = _howManyTimesIAmAssignedInASession.at(numSession);
        std::vector<Date> newDateInThisSession;
        _howManyTimesIAmAssignedInASession.erase(numSession);
        ///se c'è un solo appello in questa sessione
        if(dateInThisSession.size() != 1){
            ///se ci sono più appelli riprendo il secondo appello
            newDateInThisSession.push_back(dateInThisSession[1]);
        }
        _howManyTimesIAmAssignedInASession.insert(std::pair<int,std::vector<Date>>(numSession,newDateInThisSession));
    }
    ///pop da slots
    int numAppealPerYear = getNumAppealFromNumSessNumAppealInSession(numSession, numAppeal);
    _startSlotPerEachAppeal.erase(numAppealPerYear);
    ///pop da rooms
    _roomsEachAppeal.erase(numAppealPerYear);
}

std::vector<int> SpecificYearCourse::getRoomsAppealInSession(int numSession, int numAppeal) {
    return _roomsEachAppeal.at(getNumAppealFromNumSessNumAppealInSession(numSession, numAppeal));
}

void SpecificYearCourse::reassignAppeal(int numAppeal,int numSession, Date date, int startSlot, std::vector<int> classroomsPerCourse) {

    int numAppealPerYear = getNumAppealFromNumSessNumAppealInSession(numSession, numAppeal);

    _startSlotPerEachAppeal.insert(std::pair<int, int>(numAppealPerYear, startSlot));
    _roomsEachAppeal.insert(std::pair<int, std::vector<int>>(numAppealPerYear, classroomsPerCourse));
    std::vector<Date> vectDate;
    vectDate.push_back(date);
    ///se non esiste la chiave
    if (_howManyTimesIAmAssignedInASession.find(numSession) == _howManyTimesIAmAssignedInASession.end()) {
        _howManyTimesIAmAssignedInASession.insert(std::pair<int, std::vector<Date>>(numSession, vectDate));
    } else if (numAppeal == 2) {
        //se esiste ed è un secondo appello da inserire lo metto alla fine
        _howManyTimesIAmAssignedInASession.at(numSession).push_back(date);
    } else {
        //se esiste ed è un primo appello da inserire lo metto all'inizio
        std::vector<Date> &vectD = _howManyTimesIAmAssignedInASession.at(numSession);
        vectD.insert(vectD.begin(), date);
    }
}

void SpecificYearCourse::eraseNumAppeal() {
 _numAppeal = 0;
}

int SpecificYearCourse::getNumAppeal() const {
    return _numAppeal;
}


std::ostream &operator<<(std::ostream &output, const SpecificYearCourse &s) {
    output << s.getStartYear() << "-" << s.getEndYear() << ";";
    if (s.getisActive())
        output << "attivo";
    else
        output << "non_attivo";
    output << ";" << s.getNumParallelCourses() << ";";
    output << s.getProfParString() << ";";
    output << s.getExamString() << ";";
    output << s.getGroupedCoursesIdString();
    return output;
}