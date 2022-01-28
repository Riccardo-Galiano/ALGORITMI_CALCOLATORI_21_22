//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_STUDYCOURSE_H
#define PROGETTO_V3_STUDYCOURSE_H

#include <map>
#include <list>
#include "Course.h"
#include "SpecificYearCourse.h"

class StudyCourse {
public:
    ///constructor
    StudyCourse(const int id, const bool & isBachelor);

    ///StudyCourse management
    void addSemesterCourses(const int year, const int semester, const std::string &semesterCourses,
                            const std::map<int, StudyCourse> &studyCourse, int posFile);
    void addOffCourses(const std::vector<std::string> & offCourses);
    void updateSemestersAndOffCourses(const std::string& idCourse, std::string& acYY, std::vector<std::string>& temp);
    bool assignStudyCourse(std::string course);


    ///getter
    int getId() const;
    bool getIsBachelor() const;
    std::string getSemestersString() const;
    std::string getOffCoursesString() const;
    std::vector<std::string> getAllCoursesOfStudyCourse() const;

    ///control
    bool offCoursesEmpty() const;
    std::string isInWhichSemester(std::string codCourse) const;
    void sameSemester(std::string idCourse, const std::map<int, StudyCourse>& studyCourse, int semester, int posFile);
    std::vector<std::string> getOffCourses() const;

private:
    int _id; //codice del corso di studi
    bool _isBachelor; //magistrale?
    //key: yy-semester
    //value: vettore di Id corsi
    std::map<std::string,std::vector<std::string>> _semesters;
    std::vector<std::string> _offCourses;

};
std::ostream &operator<<(std::ostream &studC, const StudyCourse &s);

#endif //PROGETTO_V3_STUDYCOURSE_H
