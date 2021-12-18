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

DbException::DbException(std::string mex, int idProf,std::string mex2, std::string cod) {
    std::stringstream mexAndLine;
    mexAndLine<<mex<<idProf<<mex2<<cod;
    _message=mexAndLine.str();
}
