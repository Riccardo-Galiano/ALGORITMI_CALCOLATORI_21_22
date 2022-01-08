
#ifndef PROGETTO_V3_SYSTEMLOG_H
#define PROGETTO_V3_SYSTEMLOG_H


#include <vector>
#include <string>
#include "Course.h"

class SystemLog {
public:
    explicit SystemLog(std::string&);
    void generateWarnings(std::vector<Course>&,int,int,int,std::map<std::string,int>,int session);
    void generateWarningGapAppeals(std::vector<Course>&,int,int,int);
    void generateWarningGapSameStudyCourse(std::vector<Course>&,int,int);
    void generateWarningGapAvaibilityProfs(std::vector<Course>&,int,int);
    void generateWarningGapProfs(std::map<std::string,int>, int,int);
    void writeWarnings();
    void appendLogPerSession(int session, std::string&);

private:
    std::string _output_file_name;
    std::string cdsCodes(SpecificYearCourse&);
    //key: sessione
    //value: tutti i warning relativi
    std::map<int,std::string> _logPerSession;
};


#endif //PROGETTO_V3_SYSTEMLOG_H
