//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include "Course.h"
#include "Parse.hpp"
#include "DbException.h"
#include "InvalidDbException.h"
#include <algorithm>

Course::Course(const std::string &idCorso, const std::string &nomeCorso, const int cfu, const int oreLezione,
               const int oreEsercitazione, const int oreLaboratorio) {

    _id = idCorso;
    _name = nomeCorso;
    _cfu = cfu;
    _hours._lec = oreLezione;
    _hours._ex = oreEsercitazione;
    _hours._lab = oreLaboratorio;


}

///aggiunge per ogni anno accademico il corso con le sue informazioni
bool Course::addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof,
                                    std::vector<std::string> exam, std::vector<std::string> idGrouped,
                                    std::string yy_semester, std::vector<int> studyCourse) {
///key: l'anno di inizio dell'anno accademico. Value:: un oggetto SpecificYearCourse che conterrà le varie info specifiche per ogni anno accademico per ogni corso
    _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(stoi(sY_eY.substr(0, 4)),SpecificYearCourse(sY_eY, active, nCrsiPar, prof, exam,idGrouped, yy_semester,studyCourse)));

    return true;
}

///riempie il vettore di stringhe specificYearcourse
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
    std::string posLastOffCourses;
    std::vector<std::string> lastYearSpecificYearCourseSplitted;
    last << lastYearSpecificCourse;

    lastYearSpecificYearCourseSplitted = Parse::splittedLine(last.str(), ';');

    //se l'utente usasse la possibilità di lasciare vuoto il campo invariato rispetto all'anno precedente sull'ultimo campo di informazioni(id corsi)
    //specificYearCourse avrebbe soltanto 6 campi quindi dovremmo aggiungere un settimo campo inizialmente vuoto che verrà poi riempito dalle info
    //dell'anno precedente
    if (specificYearCourse.size() == 6) {
        specificYearCourse.push_back(posLastOffCourses);
    }

    ///se il campo è vuoto lo riempio con le info dell'anno precedente
    for (int i = 2; i < specificYearCourse.size(); i++) {
        if (specificYearCourse[i].empty()) {
            specificYearCourse[i] = lastYearSpecificYearCourseSplitted[i - 1];
        }
    }

    std::string profSenzaQuadre;
    int new_num_par_courses = stoi(specificYearCourse[3]);
    //check correttezza

    profSenzaQuadre = specificYearCourse[4].substr(1, specificYearCourse[4].size() -
                                                      2);//estraggo gli id di tutti i prof di tutti i corsi in parallelo
    //tornerà errore se non congruenti
    std::vector<std::string> profCorsoPar = Parse::getProfPar(profSenzaQuadre,
                                                              new_num_par_courses);//divido i vari corsi in parallelo

    return true;
}

///prende l'ultimo oggetto SpecificYearCourse dalla map _coursOfTheYear
SpecificYearCourse &Course::getLastSpecificYearCourse() {
    int lastYear = 0, actualYear;

    for (auto iter = _courseOfTheYear.begin();
         iter != _courseOfTheYear.end(); iter++) {//per ogni anno accademico di un corso
        actualYear = iter->second.getStartYear();//punta all'oggetto di tipo SpecificYearCourse e ne prende l'anno di inizio(che poi sarebbe la key)
        //potrei scrivere actualYear = iter->first
        if (actualYear > lastYear)
            lastYear = actualYear;//prendo l'ultimo anno


    }
/*
 potrei scrivere al posto del for
 auto iter = _courseOfTheYear.rbegin(); mi da direttamente l'ultimo anno perchè la map è organizzata con le key in ordine crescente quindi l'ultimo in coda sarà sicuramente l'anno accademico più frequente
 int lastYear = iter->first;
 */
    return _courseOfTheYear.at(lastYear);
}

///legge l'Id
const std::string &Course::getId() const {
    return _id;
}

///lette il nome
const std::string &Course::getName() const {
    return _name;
}

///legge i cfu
int Course::getCfu() const {
    return _cfu;
}

///legge le ore
const hours &Course::getHours() const {
    return _hours;
}

///ogni stringa è costituita dalle info di uno specifico anno accademico del corso analizzato
std::vector<SpecificYearCourse> Course::getSpecificYearsCourse() {

    std::vector<SpecificYearCourse> specificYearsCourse;

    for (auto iterSpecificYearCourse = _courseOfTheYear.begin();
         iterSpecificYearCourse != _courseOfTheYear.end(); iterSpecificYearCourse++) {//per ogni anno accademico
        //std::stringstream tokens;
        SpecificYearCourse specific = iterSpecificYearCourse->second;//salvo le info puntate da iterSpecificYearCourse che contiene le info dello specifico anno accademico
        //tokens << "a;" << specific; //overload dell'operatore. (si rimanda all'overload di << in SpecificYearCourse.cpp)
        specificYearsCourse.push_back(specific);//salvo la stringa con le info dell'anno
    }

    return specificYearsCourse;
}

///prendo la dimensione della mappa di anni accademici
int Course::getSpecificYearCourseSize() const {
    return _courseOfTheYear.size();
}

bool Course::addStudentToSpecYearCourse(int acYear, Student stud, std::string enrolYear, int mark) {
    return _courseOfTheYear.at(acYear).addStudent(stud, enrolYear, mark);
}

///prende il corso con le sue info ad uno specifico anno
const SpecificYearCourse& Course::getThisYearCourse(int year) const {
        return _courseOfTheYear.at(year);
}
///controllo che per quell'anno esista il corso
bool Course::controlExistenceSpecificYear(std::string codCourse, int year) {
    if(_courseOfTheYear.count(year)==0){
        ///non ci sono corsi per quell'anno
        throw InvalidDbException ("il seguente corso non presenta informazioni relative alla'anno accademico richiesto:",codCourse);
    }
    return true;
}

///il corso è vuoto?
bool Course::courseOfTheYearIsEmpty() {
    if (_courseOfTheYear.empty()) {
        return true;
    } else {
        return false;
    }
}

///riempi i buchi tra anni accademici di un corso
bool Course::fillAcYearsEmpty() {
    for (auto iterCourseOfTheYear = _courseOfTheYear.begin();
         iterCourseOfTheYear != _courseOfTheYear.end(); iterCourseOfTheYear++) {//per ogni anno accademico
        auto token = iterCourseOfTheYear;//iteratore all'elemento corrente
        auto iterSuccessiveCourse = ++token;//iteratore al successivo elemento in memoria

        if ((iterSuccessiveCourse->first - iterCourseOfTheYear->first > 1) && iterSuccessiveCourse !=
                                                                              _courseOfTheYear.end()) {//se c'è un gap di almeno 2 anni (mancano degli anni accademici) e non sono alla fine di _courseOfTheYear
            int range = iterSuccessiveCourse->first - iterCourseOfTheYear->first -
                        1;//quanti anni accademici mancano e che devo aggiungere
            int lastYear = iterCourseOfTheYear->first;//ultimo anno prima del gap
            for (int i = 0; i < range; i++) {//per il numero di anni accademici da aggiungere
                //riscrivo l'anno precedente
                lastYear++;//il nuovo _courseOfTheYear avrà l'anno subuto successivo all'ultimo anno accademico prima del gap
                //prendo l'ultimo SpecificYearCourse prima del gap
                SpecificYearCourse sC = iterCourseOfTheYear->second;
                sC.setYear();//setta l'anno (aggiungerà 1 all'anno di inizio e all'anno di fine)
                //insert dell'anno nuovo
                _courseOfTheYear.insert(std::pair<int, SpecificYearCourse>(lastYear, sC));//aggiungo l'anno accademico
                iterCourseOfTheYear++;//per il prossimo ciclo devo prendere l'ultimo prima del gap(che sarà quello appena aggiunto)
            }
        }
    }
    return true;
}

///prende le regole e gli orari dell'esame per un anno specifico di un corso
const Exam Course::getExamSpecificYear(int acYear) const {
    return _courseOfTheYear.at(acYear).getExam();
}

///controllo che un corso raggruppato esista nel database e in quel caso controllo che non appartenga allo stesso corso di studio dei suoi stessi raggruppati
bool Course::controlOfGroupedCourses(const std::map<std::string, Course> &courses,int year) {
    //faccio i controlli per l'anno specifico
    SpecificYearCourse sp = _courseOfTheYear.at(year);
        std::vector<std::string> grouppedCourses = sp.getIdGroupedCourses();//predo tutti i codici dei corsi raggruppati del corso considerato
        //controllo nel database se esistono i corsi raggruppati
        controlTheExistenceOfGroupedCourse(grouppedCourses,courses,year);


    return true;
}
///controllo che esistano i professori e che le ore in totale per lezioni, lab ed esercitazioni combacino con quelle del corso
bool Course::controlTheExistenceAndHoursOfProfessors(const std::map<int, Professor> &professors,int year) {
        SpecificYearCourse sp = _courseOfTheYear.at(year);
        hours hoursCourse = _hours;
        std::map<int, std::vector<professor>> profsOfParallelCourses = sp.getProfsOfParallelCourses();
        for (int i = 0; i < profsOfParallelCourses.size(); i++) {
            std::vector<professor> profsOfSingleCourse = profsOfParallelCourses.at(i);
            hours hourProfs = controlProfsOfSingleCourse(profsOfSingleCourse, professors);
            if(hoursCourse._lec != hourProfs._lec)
                throw InvalidDbException("le ore delle lezioni non sono coerenti con le ore del corso: ",getId());
            else if (hoursCourse._lab != hourProfs._lab)
                throw  InvalidDbException("le ore dei laboratori non sono coerenti con le ore del corso: ",getId());
            else if (hoursCourse._ex != hourProfs._ex)
                throw  InvalidDbException("le ore delle esercitazioni non sono coerenti con le ore del corso:",getId());
        }
    return true;
}

hours Course::controlProfsOfSingleCourse(std::vector<professor> profsOfSingleCourse,const std::map<int, Professor> &professors) {
    hours h{0,0,0};
    ///per ogni prof del corso verifico se esista nel db_professori, in tal caso prendo le sue ore e le sommo
    for (int i = 0; i < profsOfSingleCourse.size(); i++) {
        if (professors.find(profsOfSingleCourse[i].prof_id) == professors.end()) {
            throw DbException("Il seguente professore non è stato trovato nel database:",profsOfSingleCourse[i].prof_id, ". Controllare il seguente corso che si vuole inserire:",getName());
        } else
        {
            h._lec = h._lec + profsOfSingleCourse[i].hLez;
            h._lab = h._lab + profsOfSingleCourse[i].hLab;
            h._ex = h._ex + profsOfSingleCourse[i].hExe;
        }
    }
    return h;
}

bool Course::controlTheExistenceOfGroupedCourse(std::vector<std::string> grouppedCourses, const std::map<std::string, Course> &courses, int year) {
    for (int i = 0; i < grouppedCourses.size(); i++) {//per tutti i corsi raggruppati
        auto iterGrouppedCourse = courses.find(grouppedCourses[i]);//cerco nel database dei corsi se esiste
        if (iterGrouppedCourse == courses.end()) {//se non esiste nel database
            throw DbException("Il seguente corso raggruppato non è presente tra i corsi da inserire:",grouppedCourses[i], ". Ricontrollare il seguente corso da inserire:", getName());
        }
    }
    return true;
}

bool Course::controlItsGroupedCourse(std::vector<std::string>allCourses,std::vector<std::string>allGroupedCourses,int idCourse,int year) const {
       SpecificYearCourse sp = _courseOfTheYear.at(year);
       std::vector<std::string> groupedCourse = sp.getIdGroupedCourses();
       for(int i = 0; i<groupedCourse.size(); i++){
           ///controllo che il corso raggruppato non sia all'interno del corso di studio
           auto foundIntoStudyCourse = std::find(allCourses.begin(),allCourses.end(),groupedCourse[i]);
           if(foundIntoStudyCourse != allCourses.end())
               throw DbException("stesso corso di studio tra:",getId()," e ",groupedCourse[i]," relativamente al corso di studio: ",idCourse);
           ///controllo che il corso raggruppato non sia anche il raggruppato di un altro corso dello stesso corso di studio
           int myCount = std::count(allGroupedCourses.begin(), allGroupedCourses.end(),groupedCourse[i]);
           if(myCount > 1)
               throw DbException("il corso raggruppato",groupedCourse[i],"è anche un corso raggruppato di un altro corso dello stesso corso di studio:",idCourse);
       }

    return true;
}

bool Course::courseOfTheYearFounded(int year) {
    return _courseOfTheYear.find(year) != _courseOfTheYear.end();//ritorno se esistono info per quel corso in quel'anno
}

SpecificYearCourse &Course::getThisYearCourseReference(int year){
    if(_courseOfTheYear.count(year)==0){
        ///non ci sono corsi per quell'anno
        throw std::invalid_argument("Non ci sono corsi selezionabili nell'anno accademico richiesto");
    }
    return _courseOfTheYear.at(year);
}

std::ostream &operator<<(std::ostream &course, Course &s) {
    course << "c;" << s.getId() << ";" << s.getName() << ";" << s.getCfu() << ";" << s.getHours()._lec << ";"
           << s.getHours()._ex << ";" << s.getHours()._lab << std::endl;
    int size = s.getSpecificYearCourseSize();
    std::vector<SpecificYearCourse> SYCourse = s.getSpecificYearsCourse();
    for (int i = 0; i < size; i++) {
        course << "a;" << SYCourse[i] << std::endl;
    }
    return course;
}
