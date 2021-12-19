//
// Created by lucam on 16/11/2021.
//

#include <sstream>
#include "DbException.h"

const char *DbException::what() const noexcept {
        return _message.c_str();
}

DbException::DbException(std::string mess) {
    _message = mess;
}

DbException::DbException(std::string mex, int line) {
    std::stringstream mexAndLine;
    mexAndLine<<mex<<line;
    _message=mexAndLine.str();
}

DbException::DbException(std::string mex, std::string codCourse,std::string mex2, int line) {
    std::stringstream mexAndLine;
    mexAndLine<<mex<<codCourse<<mex2<<line;
    _message=mexAndLine.str();
}

DbException::DbException(std::string mex, std::string codCourse,std::string mex2, std::string name) {
    std::stringstream mexAndLine;
    mexAndLine<<mex<<codCourse<<mex2<<name;
    _message=mexAndLine.str();
}

DbException::DbException(std::string mex, std::string codCourse) {
    std::stringstream mexAndLine;
    mexAndLine<<mex<<codCourse;
    _message=mexAndLine.str();
}

DbException::DbException(std::string mex, int idProf,std::string mex2, std::string name) {
    std::stringstream mexAndLine;
    mexAndLine<<mex<<idProf<<mex2<<name;
    _message=mexAndLine.str();
}

DbException::DbException(std::string mex,  std::vector<std::string>  codGrouppedCourse,std::string mex2,std::vector<std::string> nameGrouppedCourse) {
    std::stringstream mexAndLine;
    mexAndLine<<mex;
    for(int i = 0;i<codGrouppedCourse.size();i++){
        mexAndLine<<codGrouppedCourse[i];
        if(i<codGrouppedCourse.size()-1)
            mexAndLine<<";";
    }
    mexAndLine<< mex2;
    for(int i = 0;i<nameGrouppedCourse.size();i++){
        mexAndLine<<nameGrouppedCourse[i];
        if(i<nameGrouppedCourse.size()-1)
            mexAndLine<<";";
    }
    _message=mexAndLine.str();
}

DbException::DbException(std::string mex1, std::string id1, std::string mex2, std::string id2, std::string mex3, int line) {
    std::stringstream mexAndLine;
    mexAndLine<<mex1<<id1<<mex2<<id2<<mex3<<line;
    _message=mexAndLine.str();
}
