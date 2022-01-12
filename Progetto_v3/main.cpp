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
            try {
                uni.addStuds(argv[2]);
            }
            catch(std::exception& occurredException){
                strToAddToLog = occurredException.what();
                strToAddToLog.append("Non ho potuto ....\n");
                sysLog.appendToLog(strToAddToLog,true);
                break;
            }
            strToAddToLog.append("Ho eseguito correttamente...\n");
            sysLog.appendToLog(strToAddToLog,false);
            break;
        }
        case add_professor: {
            errorString = uni.addProfessors(argv[2]);
            break;
        }
        case add_classroom: {
            errorString = uni.addClassrooms(argv[2]);
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
            errorString = uni.updateStuds(argv[2]);
            break;
        }
        case update_professor: {
            errorString = uni.updateProfessors(argv[2]);
            break;
        }
        case update_classroom: {
            errorString = uni.updateClassroom(argv[2]);
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
            errorString = uni.setSessionPeriod(acyear, one, two, three);
            break;
        }
        case set_availability: {
            errorString = uni.setProfsNoAvailability(argv[3], argv[4]);
            break;
        }
        case set_exam_date: {
            std::string year(argv[2]);
            errorString = uni.setExamDate(argv[2], argv[3]);
            break;
        }
        case versioning:{
            errorString = uni.versioning(atoi(argv[2]));
            break;
        }
        case add_study_plan_student:{
            errorString = uni.addStudyPlan(argv[2]);
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
            errorString = uni.setMinDistance(argv[3],argv[4]);
            break;
        }
        case request_changes:{
            errorString = uni.requestChanges(argv[3],argv[4]);
            break;
        }
        default:
            throw std::invalid_argument("comando non trovato");
    }
    return errorString;
};

int main(int argc, char *argv[]) {
    /*
    try {
        std::map<int, Student> students = readMembers<Student>(argv[1]);
        std::map<int, Professor> professors = readMembers<Professor>(argv[2]);
        return 2;
    } catch (logic_error &) {
        cerr << "not valid input!!";

        return -1;
    }
*/
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
