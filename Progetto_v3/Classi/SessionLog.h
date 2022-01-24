
#ifndef PROGETTO_V3_SESSIONLOG_H
#define PROGETTO_V3_SESSIONLOG_H


#include <vector>
#include <string>
#include "Course.h"

class SessionLog {
public:
    explicit SessionLog(std::string& output_file_name);
    void generateWarnings(std::vector<Course>& courses,int relaxPar,int gap,int year,std::map<std::string,int> gapProfs,int session);
    void generateWarningGapAppeals(std::vector<Course>& courses,int gap,int year,int session);
    void generateWarningGapSameStudyCourse(std::vector<Course>& courses,int year,int session);
    void generateWarningGapAvaibilityProfs(std::vector<Course>& courses,int year,int session);
    void generateWarningGapProfs(std::map<std::string,int> gapProfs, int year,int session);
    void writeWarnings();
    void appendLogPerSession(int session, std::string& output);
    std::string cdsCodes(SpecificYearCourse&);
private:
    std::string _output_file_name;
    //key: sessione
    //value: tutti i warning relativi
    std::map<int,std::string> _logPerSession;
};


#endif //PROGETTO_V3_SESSIONLOG_H
