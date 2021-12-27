
#ifndef PROGETTO_V3_SYSTEMLOG_H
#define PROGETTO_V3_SYSTEMLOG_H


#include <vector>
#include <string>
#include "Course.h"

class SystemLog {
public:
    explicit SystemLog(std::string&, int);
    void generateWarnings(std::vector<Course>&,int,int,int);
    void generateWarningGapAppeals(std::vector<Course>&,int,int);
    void generateWarningGapSameStudyCourse(std::vector<Course>&,int);
    void generateWarningGapAvaibilityProfs(std::vector<Course>&,int);
    void appendLog(std::string&);

private:
    std::string _output_file_name;
    std::string cdsCodes(SpecificYearCourse&);
};


#endif //PROGETTO_V3_SYSTEMLOG_H
