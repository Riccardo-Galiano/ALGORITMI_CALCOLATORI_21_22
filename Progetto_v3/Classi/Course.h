//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_COURSE_H
#define PROGETTO_V3_COURSE_H


#include <string>
#include <vector>
#include <map>
#include "SpecificYearCourse.h"



typedef struct {
    int _lec;///ore lezione
    int _lab;///ore laboratori
    int _ex;///ore esercitazione
} hours;

class Course {
public:
    ///costruttore
    Course(const std::string& idCourse, const std::string& courseName, int cfu, int lessonHours, int exerciseHours, int labHours);

    ///management SpecificYear
    void addSpecificYearCourses(std::string sY_eY, bool active, int nCrsiPar, std::vector<std::string> prof, std::vector<std::string> exam, std::vector<std::string> idGrouped, std::string yy_semester, std::vector<int> studyCourse, int line_counter );
    void modifyStudentAsPassedToSpecYearCourse(int acYear, Student& stud, int mark, std::string appealsDate);
    void registerStudentsToSpecificYear(int acYearRegistration, Student& stud);
    void fillSpecificYearCourse(std::vector<std::string>& specificYearCourse, int line_counter);
    void fillAcYearsEmpty();
    SpecificYearCourse &getLastSpecificYearCourse();
    std::vector<SpecificYearCourse>  getSpecificYearsCourse();
    const SpecificYearCourse & getThisYearCourse(int year) const;
    SpecificYearCourse & getThisYearCourseReference(int year);
    void assignYY_Sem(std::string& acYYoff, std::string& yy_semester);
    void assignStudToAppealPerYear(std::string acYear, std::string appealDate, std::string allStudsPassedExamString);
    std::vector<std::pair<std::string, int>> splittAllStudPassedExamString(std::string allStudsPassedExamString);
    void updateYYSemesterInAllSpecYearCourse(std::string& yy_semester);
    void fillAcYearsUntilStartAcYear(int startAcYear, int lastYear, bool tempData);
    void reassignAppealToSpecificYear(int acYear,int numAppeal,int numSession,Date date,int startSlot, std::vector<int>classroomsPerCourse);
    void updateStudyCourseInAllSpecYearCourse(int idStudyCourse);
    bool courseExistInThisSpecificYear(int year);
    void eraseThisSpecificYear(int year);

    ///getter
    const std::string &getId() const;
    const std::string &getName() const;
    int getCfu() const;
    const Exam getExamSpecificYear(int acYear) const;
    std::vector<std::string>getAcYearStudExam()const;
    int getExamSlotPerYear(std::string acYear)const;
    const hours &getHours() const;
    int getSpecificYearCourseSize() const;
    int getSemesterAtYear(int acStartYear,std::string name)const;
    std::string getFirstAcYearOff()const;
    std::vector<std::string>getIdGroupedCourseFromYear(int acYear)const;
    int getFirstYearOfActivity()const;
    std::vector<int> getProfsPerYear(std::string acYear)const;

    ///control
    void controlTheExistenceAndHoursOfProfessors(const std::map<int, Professor> &professors, int year, int line_counter);
    hours controlProfsOfSingleCourse(std::vector<professor> profsOfSingleCourse,
                                     const std::map<int, Professor> &professors, int line_counter);
    void notActive();
    void sameSemesterGrouped(std::map<std::string,Course> courses);
    void controlAppeal(std::string appealDate);
    bool courseExistInThisYear(int year);
    bool profHaveThisCourse(int matr, int acStartYear);






private:
    std::string _id;///codice identificativo del corso
    std::string _name;///nome del corso
    int _cfu;/// numero di crediti
    hours _hours;///ore impiegate nel corso; suddivise in ore di lezione,laboratori ed esame(struct hours)
    std::map<int, SpecificYearCourse> _courseOfTheYear;//key: accademic yy

};
std::ostream &operator<<(std::ostream &course, Course&c);


#endif //PROGETTO_V3_COURSE_H
