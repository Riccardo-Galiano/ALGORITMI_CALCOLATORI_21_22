//
// Created by antonio_vespa on 27/12/21.
//

#include <fstream>
#include <sstream>
#include "SessionLog.h"
#include "Course.h"
#include "Parse.hpp"

SessionLog::SessionLog(std::string &output_file_name) {
    _output_file_name = output_file_name;
}

void SessionLog::generateWarningGapAppeals(std::vector<Course> &courses, int gap, int year, int session) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        std::vector<int> cds = sp.getStudyCourseAssigned();
        for(int j = 0; j < cds.size(); j++) {
            std::string setIdStudyCourse = Parse::setId('C', 3, cds[j]);
            ss << setIdStudyCourse << courses[i].getId() << ";" << "regola GAP_APPELLI: questo esame è stato programmato con uno scarto di "
               << gap << " giorni\n";
        }
    }
    std::string toPass = ss.str();
    appendLogPerSession(session,toPass);
}

void SessionLog::writeWarnings(std::string &output_name_file) {

    std::vector<std::string> nameWithoutExtension = Parse::splittedLine(output_name_file, '.');
    _output_file_name = nameWithoutExtension[0];
    for(int i=1; i<=3; i++) {
        std::stringstream ss;
        ss << _output_file_name << "_s" << i << "_warnings.txt";
        std::fstream fout(ss.str(), std::ios::out);
        fout << _logPerSession.at(i);
        fout.close();
    }
}

void SessionLog::generateWarnings(std::vector<Course> &courses, int relaxPar, int gap, int year, std::vector<std::pair<std::string,int>> gapProfsNoRespect, int session) {
    generateWarningGapProfs(gapProfsNoRespect,session);
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

void SessionLog::generateWarningGapAvaibilityProfs(const std::vector<Course>& courses, int year, int session) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        std::vector<int> cds = sp.getStudyCourseAssigned();
        for(int j = 0; j < cds.size(); j++) {
            std::string setIdStudyCourse = Parse::setId('C', 3, cds[j]);
            ss << setIdStudyCourse <<";"<< courses[i].getId() << ";" << "regola AVAIBILITY_PROFS non rispettata\n";
        }
    }
    std::string toPass = ss.str();
    appendLogPerSession(session,toPass);
}

void SessionLog::generateWarningGapSameStudyCourse(const std::vector<Course> &courses, int year, int session) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        std::vector<int> cds = sp.getStudyCourseAssigned();
        for(int j = 0; j < cds.size(); j++) {
            std::string setIdStudyCourse = Parse::setId('C',3,cds[j]);
            ss << setIdStudyCourse << ";" << courses[i].getId() << ";" << "regola GAP_CORSI_STESSO_CORSO_DI_STUDIO non rispettata\n";
        }
    }
    std::string toPass = ss.str();
    appendLogPerSession(session,toPass);
}


void SessionLog::generateWarningGapProfs(const std::vector<std::pair<std::string,int>>& gapProfsNoRespect, int session) {
    std::stringstream ss;

        for (auto iter = gapProfsNoRespect.begin(); iter != gapProfsNoRespect.end(); iter++) {
            ss << iter->first << ";" << " GAP_PROFESSORE di " << iter->second << " non rispettata\n";
        }
        std::string toReturn = ss.str();
        appendLogPerSession(session, toReturn);

}

void SessionLog::appendLogPerSession(int session, const std::string& output) {
    if(_logPerSession.count(session)==0){
        _logPerSession.insert(std::pair<int,std::string>(session,output));
    }
    else{
        _logPerSession.at(session).append(output);
    }
}

