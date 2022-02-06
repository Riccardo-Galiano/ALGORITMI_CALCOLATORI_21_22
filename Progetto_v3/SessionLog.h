//
// Created by andre on 10/12/2021.
//
#ifndef PROGETTO_V3_SESSIONLOG_H
#define PROGETTO_V3_SESSIONLOG_H


#include <vector>
#include <string>
#include "Course.h"

class SessionLog {
public:
    explicit SessionLog(std::string& output_file_name);
    void generateWarnings(std::vector<Course>& courses,int relaxPar,int year,int gap,std::vector<std::pair<std::string,int>> gapProfsNoRespect,int session);
    void generateWarningGapAppeals(const std::vector<Course> &courses, int gap, int year, int session);
    void generateWarningGapSameStudyCourse(const std::vector<Course>& courses,int year,int session);
    void generateWarningGapAvaibilityProfs(const std::vector<Course>& courses,int year,int session);
    void generateWarningGapProfs(const std::vector<std::pair<std::string,int>>& gapProfsNoRespect,int session);
    void writeWarnings(std::string &output_name_file);
    void appendLogPerSession(int session, const std::string& output);

private:
    std::string _output_file_name;
    //key: sessione
    //value: tutti i warning relativi
    std::map<int,std::string> _logPerSession;
};


#endif //PROGETTO_V3_SESSIONLOG_H
