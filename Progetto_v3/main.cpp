#include <iostream>
#include "./Classi/University.h"
#include <fstream>
#include <sstream>
#include "Classi/Student.h"
#include <vector>
#include "Classi/Professor.h"
#include "Classi/DbException.h"
#include "Classi/SystemLog.h"

using namespace std;

enum {
    add_student,
    add_professor,
    add_classroom,
    add_course,
    add_studyCourse,
    update_student,
    update_professor,
    update_classroom,
    insert_course,
    set_session_period,
    set_availability,
    set_exam_date,
    versioning,
    add_study_plan_student,
    update_study_plan_student,
    insert_students_grades,
    set_min_distance,
    request_changes
};

SystemLog sysLog;

int returnCode(char *argv[]) {
    std::string paramInput = argv[1];
    std::string secondParamInput = argv[2];
    if (paramInput.compare("-a:s") == 0)
        return add_student;
    else if (paramInput.compare("-a:d") == 0)
        return add_professor;
    else if (paramInput.compare("-a:a") == 0)
        return add_classroom;
    else if (paramInput.compare("-a:c") == 0)
        return add_course;
    else if (paramInput.compare("-a:f") == 0)
        return add_studyCourse;
    else if (paramInput.compare("-u:s") == 0)
        return update_student;
    else if (paramInput.compare("-u:d") == 0)
        return update_professor;
    else if (paramInput.compare("-u:a") == 0)
        return update_classroom;
    else if (paramInput.compare("-i:c") == 0)
        return insert_course;
    else if(paramInput.compare("-s") == 0 && secondParamInput.compare("current_a")==0)
        return set_session_period;
    else if(paramInput.compare("-s") == 0 && secondParamInput.compare("set_availability")==0)
        return set_availability;
    else if (paramInput.compare("-g")==0)
        return set_exam_date;
    else if (paramInput.compare("-up")==0)
        return versioning;
    else if (paramInput.compare("-a:p")==0)
        return add_study_plan_student;
    else if (paramInput.compare("-u:p")==0)
        return update_study_plan_student;
    else if (paramInput.compare("-i:v")==0)
        return insert_students_grades;
    else if(paramInput.compare("-s") == 0 && secondParamInput.compare("set_min_distance")==0)
        return set_min_distance;
    else if(paramInput.compare("-s") == 0 && secondParamInput.compare("request_changes")==0)
        return request_changes;
    return -1;
}

std::vector<std::string> program(University &uni, char **argv) {
    int code = returnCode(argv);
    std::vector<std::string> errorString;
    switch (code) {
        case add_student: {
            std::string strToAddToLog;
            bool isOk = true;
            try {
                uni.addStuds(argv[2]);
            }
            catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -a:s per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -a:s e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        case add_professor: {
            std::string strToAddToLog;
            bool isOk = true;
            try {
                uni.addProfessors(argv[2]);
            }catch(std::exception& occurredException){
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append("Non e' stato possibile effettuare il comando -a:d per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog,true);
                    isOk = false;
                }
            if(isOk) {
                strToAddToLog.append("Il comando -a:d e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
                break;
        }
        case add_classroom: {
            std::string strToAddToLog;
            bool isOk = true;
            try{
            uni.addClassrooms(argv[2]);
            }catch(std::exception& occurredException){
                 strToAddToLog = occurredException.what();
                 strToAddToLog.append("Non e' stato possibile effettuare il comando -a:a per gli errori elencati precedentemente\n");
                 sysLog.appendToLog(strToAddToLog,true);
             break;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -a:a e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        case add_course: {
            errorString = uni.addCourses(argv[2]);
            break;
        }
        case add_studyCourse: {
            errorString = uni.addStudyCourses(argv[2]);
            break;
        }
        case update_student: {
            std:: string strToAddToLog;
            bool isOk = true;
            try{
            uni.updateStuds(argv[2]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -u:s per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -u:s e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        case update_professor: {
            std:: string strToAddToLog;
            bool isOk = true;
            try{
            uni.updateProfessors(argv[2]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -u:d per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk){
                strToAddToLog.append("Il comando -u:d e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        case update_classroom: {
            std:: string strToAddToLog;
            bool isOk = true;
            try {
                uni.updateClassroom(argv[2]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -u:a per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -u:a e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        case insert_course: {
            errorString = uni.insertCourses(argv[2]);
            break;
        }
        case set_session_period: {
            std::string acyear(argv[3]);
            std::string one(argv[4]);
            std::string two(argv[5]);
            std::string three(argv[6]);
            std:: string strToAddToLog;
            bool isOk = true;
            try {
                uni.setSessionPeriod(acyear, one, two, three);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -s current_a per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -s current_a e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }

            break;
        }
        case set_availability: {
            std:: string strToAddToLog;
            bool isOk = true;
            try {
                uni.setProfsNoAvailability(argv[3], argv[4]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -s set_availability per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -s set_availability e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        case set_exam_date: {
            std::string year(argv[2]);
            std:: string strToAddToLog;
            bool isOk = true;
            try {
                uni.setExamDate(argv[2], argv[3]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -g per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -g e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        case versioning:{
            uni.versioning(argv[2]);
            break;
        }
        case add_study_plan_student:{
            std:: string strToAddToLog;
            bool isOk = true;
            try {
                uni.addStudyPlan(argv[2]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -a:p per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -a:p e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }

            break;
        }
        case update_study_plan_student:{
            errorString = uni.updateStudyPlan(argv[2]);
            break;
        }
        case insert_students_grades:{
            errorString = uni.insertStudentsGrades(argv[2]);
            break;
        }
        case set_min_distance:{
            std:: string strToAddToLog;
            bool isOk = true;
            try {
                uni.setMinDistance(argv[3],argv[4]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -s set_min_distance per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -s set_min_distance e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }

            break;
        }
        case request_changes:{
            std:: string strToAddToLog;
            bool isOk = true;
            try {
                uni.requestChanges(argv[3],argv[4]);
            }catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non e' stato possibile effettuare il comando -s request_changes per gli errori elencati precedentemente\n");
                sysLog.appendToLog(strToAddToLog,true);
                isOk = false;
            }
            if(isOk) {
                strToAddToLog.append("Il comando -s request_changes e' stato eseguito correttamente\n");
                sysLog.appendToLog(strToAddToLog, false);
            }
            break;
        }
        default:
            throw std::invalid_argument("comando non trovato");
    }
    return errorString;
};

int main(int argc, char *argv[]){
    University poliTo;
    if (argc < 3) {
        throw std::invalid_argument("errore numero parametri linea di comando");
    }
    program(poliTo, argv);
    if(sysLog.errorsOccurred())
        std::cerr << sysLog.getLog();
    else
        std::cout << sysLog.getLog();
    return 0;
}
