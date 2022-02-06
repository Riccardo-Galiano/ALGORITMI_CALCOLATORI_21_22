#include <iostream>
#include "University.h"
#include <vector>
#include "SystemLog.h"

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
    if (paramInput == "-a:s")
        return add_student;
    else if (paramInput == "-a:d")
        return add_professor;
    else if (paramInput == "-a:a")
        return add_classroom;
    else if (paramInput == "-a:c")
        return add_course;
    else if (paramInput == "-a:f")
        return add_studyCourse;
    else if (paramInput == "-u:s")
        return update_student;
    else if (paramInput == "-u:d")
        return update_professor;
    else if (paramInput == "-u:a")
        return update_classroom;
    else if (paramInput == "-i:c")
        return insert_course;
    else if(paramInput == "-s" && secondParamInput == "current_a")
        return set_session_period;
    else if(paramInput == "-s" && secondParamInput == "set_availability")
        return set_availability;
    else if (paramInput == "-g")
        return set_exam_date;
    else if (paramInput == "-up")
        return versioning;
    else if (paramInput == "-a:p")
        return add_study_plan_student;
    else if (paramInput == "-u:p")
        return update_study_plan_student;
    else if (paramInput == "-i:v")
        return insert_students_grades;
    else if(paramInput == "-s" && secondParamInput == "set_min_distance")
        return set_min_distance;
    else if(paramInput == "-s" && secondParamInput == "request_changes")
        return request_changes;
    return -1;
}

std::vector<std::string> program(University &uni, char **argv,int argc) {
    int code = returnCode(argv);
    std::vector<std::string> errorString;
    switch (code) {
        case add_student: {
            std::string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.addStuds(argv[2]);
                }
                catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -a:s per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -a:s e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
                break;
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -a:s errato.\nNon e' stato possibile effettuare il comando -a:s per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
        } // O(n)
        case add_professor: {
            std::string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.addProfessors(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -a:d per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -a:d e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -a:d errato.\nNon e' stato possibile effettuare il comando -a:s per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
                break;
        } // O(n)
        case add_classroom: {
            std::string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.addClassrooms(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -a:a per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    break;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -a:a e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -a:a errato.\nNon e' stato possibile effettuare il comando -a:a per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        } // O(n)
        case add_course: {
            std::string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.addCourses(argv[2]);
                }
                catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -a:c per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -a:c e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -a:c errato.\nNon e' stato possibile effettuare il comando -a:c per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }

            break;
        } // O(n^2)
        case add_studyCourse: {
            std::string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.addStudyCourses(argv[2]);
                }
                catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -a:f per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -a:f e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -a:f errato.\nNon e' stato possibile effettuare il comando -a:c per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        } //O(n)
        case update_student: {
            std:: string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.updateStuds(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -u:s per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -u:s e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -u:s errato.\nNon e' stato possibile effettuare il comando -u:s per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case update_professor: {
            std:: string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.updateProfessors(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -u:d per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -u:d e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -u:d errato.\nNon e' stato possibile effettuare il comando -u:d per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case update_classroom: {
            std:: string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.updateClassroom(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -u:a per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -u:a e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -u:a errato.\nNon e' stato possibile effettuare il comando -u:a per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case insert_course: {
            std::string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.insertCourses(argv[2]);
                }
                catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append("Non e' stato possibile effettuare il comando -i:c per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -i:c e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -i:c errato.\nNon e' stato possibile effettuare il comando -a:c per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case set_session_period: {
            std::string strToAddToLog;
            if(argc == 7) {
                std::string acyear(argv[3]);
                std::string one(argv[4]);
                std::string two(argv[5]);
                std::string three(argv[6]);
                bool isOk = true;
                try {
                    uni.setSessionPeriod(acyear, one, two, three);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -s current_a per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -s current_a e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -s current_a errato.\nNon e' stato possibile effettuare il comando -s current_a per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case set_availability: {
            std:: string strToAddToLog;
            if(argc == 5) {
                bool isOk = true;
                try {
                    uni.setProfsNoAvailability(argv[3], argv[4]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -s set_availability per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -s set_availability e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            } else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -s set_availability errato.\nNon e' stato possibile effettuare il comando -s set_availability per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case set_exam_date: {
            std:: string strToAddToLog;
            if(argc == 4) {
                std::string year(argv[2]);
                bool isOk = true;
                try {
                    uni.setExamDate(argv[2], argv[3]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -g per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -g e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -g errato.\nNon e' stato possibile effettuare il comando -g per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case versioning:{
            std:: string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.versioning(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -up per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -up e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -up errato.\nNon e' stato possibile effettuare il comando -up per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case add_study_plan_student:{
            std:: string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.addStudyPlan(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -a:p per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -a:p e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -a:p errato.\nNon e' stato possibile effettuare il comando -a:p per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case update_study_plan_student:{
            std:: string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.updateStudyPlan(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -u:p per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -u:p e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -u:p errato.\nNon e' stato possibile effettuare il comando -u:p per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case insert_students_grades:{
            std:: string strToAddToLog;
            if(argc == 3) {
                bool isOk = true;
                try {
                    uni.insertStudentsGrades(argv[2]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -i:v per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -i:v e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -i:v errato.\nNon e' stato possibile effettuare il comando -i:v per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case set_min_distance:{
            std:: string strToAddToLog;
            if(argc == 5) {
                bool isOk = true;
                try {
                    uni.setMinDistance(argv[3], argv[4]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append(
                            "Non e' stato possibile effettuare il comando -s set_min_distance per gli errori elencati precedentemente\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -s set_min_distance e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -s set_min_distance errato.\nNon e' stato possibile effettuare il comando -s set_min_distance per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        case request_changes:{
            std:: string strToAddToLog;
            if(argc == 5) {
                bool isOk = true;
                try {
                    uni.requestChanges(argv[3], argv[4]);
                } catch (std::exception &occurredException) {
                    strToAddToLog = occurredException.what();
                    strToAddToLog.append("Non e' stato possibile effettuare il comando -s request_changes per gli errori elencati precedentemente o descritti nei warnings relativi alla sessione richiesta\n");
                    sysLog.appendToLog(strToAddToLog, true);
                    isOk = false;
                }
                if (isOk) {
                    strToAddToLog.append("Il comando -s request_changes e' stato eseguito correttamente\n");
                    sysLog.appendToLog(strToAddToLog, false);
                }
            }else{
                strToAddToLog.append("Numero di argomenti da terminale per il comando -s request_changes errato.\nNon e' stato possibile effettuare il comando -s request_changes per gli errori elencati precedentemente\n ");
                sysLog.appendToLog(strToAddToLog,true);
            }
            break;
        }
        default:
            throw std::invalid_argument("Comando non trovato");
    }
    return errorString;
};

int main(int argc, char *argv[]){
    if(argc<3){
        cerr<<"Errore numero parametri \n";
    }else {
        University poliTo;
        try {
            program(poliTo, argv, argc);
        } catch (std::invalid_argument &err) {
            std::cerr << err.what();
        }
        if (sysLog.errorsOccurred())
            std::cerr << sysLog.getLog();
        else
            std::cout << sysLog.getLog();
    }
    return 0;
}
