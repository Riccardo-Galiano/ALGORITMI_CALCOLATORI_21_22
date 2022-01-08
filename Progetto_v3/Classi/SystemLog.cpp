//
// Created by antonio_vespa on 27/12/21.
//

#include <fstream>
#include <sstream>
#include "SystemLog.h"
#include "Course.h"

SystemLog::SystemLog(std::string &output_file_name) {
    _output_file_name = output_file_name;
}

void SystemLog::generateWarningGapAppeals(std::vector<Course> &courses, int gap, int year,int session) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        ss << cdsCodes(sp) << ";";
        ss << courses[i].getId() << ";" << "regola GAP_APPELLI: questo esame è stato programmato con uno scarto di "
                                        << gap << " giorni\n";
    }
    std::string toPass = ss.str();
    appendLogPerSession(session,toPass);
}

void SystemLog::writeWarnings() {
    std::stringstream ss;
    for(int i=1; i<=3; i++) {
        ss << _output_file_name << "_s" << i << "_warnings.txt";
        std::fstream fout(_output_file_name, std::ios::out);
        fout << _logPerSession.at(i);
    }
}

void SystemLog::generateWarnings(std::vector<Course> &courses, int relaxPar, int gap, int year,std::map<std::string,int> gapProfs, int session) {
    generateWarningGapProfs(gapProfs,year,session);
    switch (relaxPar) {
        case (1): {
            generateWarningGapSameStudyCourse(courses, year,session);
            break;
        }
        case (2): {
            generateWarningGapSameStudyCourse(courses, year,session);
            generateWarningGapAppeals(courses, gap, year,session);
            break;
        }
        case (3): {
            generateWarningGapSameStudyCourse(courses, year,session);
            generateWarningGapAppeals(courses, gap, year,session);
            generateWarningGapAvaibilityProfs(courses, year,session);
            break;
        }
        default:
            break;
    }
}

void SystemLog::generateWarningGapAvaibilityProfs(std::vector<Course>& courses, int year, int session) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        ss << cdsCodes(sp) << ";";
        ss << courses[i].getId() << ";" << "regola AVAIBILITY_PROFS non rispettata\n";
    }
    std::string toPass = ss.str();
    appendLogPerSession(session,toPass);
}

void SystemLog::generateWarningGapSameStudyCourse(std::vector<Course> &courses, int year, int session) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        ss << cdsCodes(sp) << ";";
        ss << courses[i].getId() << ";" << "regola GAP_CORSI_STESSO_CORSO_DI_STUDIO non rispettata\n";
    }
    std::string toPass = ss.str();
    appendLogPerSession(session,toPass);
}

std::string SystemLog::cdsCodes(SpecificYearCourse &sp) {
    std::stringstream ss;
    std::vector<int> cds = sp.getStudyCourseAssigned();
    ss << "{";
    for (int j = 0; j < cds.size(); j++) {
        ss << "C" << cds[j];
        if (j != cds.size() - 1)
            ss << ",";
    }
    ss << "}";
    std::string toReturn = ss.str();
    return toReturn;
}

void SystemLog::generateWarningGapProfs(std::map<std::string,int> gapProfs, int year, int session) {
    std::stringstream ss;
    ss << year << ";{";
    for (auto iter = gapProfs.begin(); iter != gapProfs.end(); iter++) {
        ss << iter->first << " GAP_PROFESSORE di " << iter->second << " non rispettata\n";
    }
    ss << "}";
    std::string toReturn = ss.str();
    appendLogPerSession(session,toReturn);
}

void SystemLog::appendLogPerSession(int session, std::string& output) {
    if(_logPerSession.count(session)==0){
        _logPerSession.insert(std::pair<int,std::string>(session,output));
    }
    else{
        _logPerSession.at(session).append(output);
    }
}

