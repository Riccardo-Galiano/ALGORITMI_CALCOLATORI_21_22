//
// Created by antonio_vespa on 27/12/21.
//

#include <fstream>
#include <sstream>
#include "SystemLog.h"
#include "Course.h"

SystemLog::SystemLog(std::string &output_file_name, int session) {
    std::stringstream ss;
    ss << output_file_name << "_s" << session << "_warnings.txt";
    _output_file_name = ss.str();
}

void SystemLog::generateWarningGapAppeals(std::vector<Course> &courses, int gap, int year) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        ss << cdsCodes(sp) << ";";
        ss << courses[i].getId() << ";" << "regola GAP_APPELLI: questo esame è stato programmato con uno scarto di "
                                        << gap << " giorni\n";
    }
    std::string toPass = ss.str();
    appendLog(toPass);
}

void SystemLog::appendLog(std::string &input) {
    std::fstream fout(_output_file_name, std::ios::out);
    fout << input;
}

void SystemLog::generateWarnings(std::vector<Course> &courses, int relaxPar, int gap, int year) {
    switch (relaxPar) {
        case (0): {
            ///no warnings
        }
        case (1): {
            generateWarningGapSameStudyCourse(courses, gap);
        }
        case (2): {
            generateWarningGapSameStudyCourse(courses, gap);
            generateWarningGapAppeals(courses, gap, year);
        }
        case (3): {
            generateWarningGapSameStudyCourse(courses, gap);
            generateWarningGapAppeals(courses, gap, year);
            generateWarningGapAvaibilityProfs(courses, gap);
        }
        default:
            break;
    }
}

void SystemLog::generateWarningGapAvaibilityProfs(std::vector<Course>& courses, int year) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        ss << cdsCodes(sp) << ";";
        ss << courses[i].getId() << ";" << "regola AVAIBILITY_PROFS non rispettata\n";
    }
    std::string toPass = ss.str();
    appendLog(toPass);
}

void SystemLog::generateWarningGapSameStudyCourse(std::vector<Course> &courses, int year) {
    std::stringstream ss;
    ///<id_corso_studi>;<id_esame>;<regola non rispettata>
    for (int i = 0; i < courses.size(); i++) {
        SpecificYearCourse sp = courses[i].getThisYearCourse(year);
        ss << cdsCodes(sp) << ";";
        ss << courses[i].getId() << ";" << "regola GAP_CORSI_STESSO_CORSO_DI_STUDIO non rispettata\n";
    }
    std::string toPass = ss.str();
    appendLog(toPass);
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

