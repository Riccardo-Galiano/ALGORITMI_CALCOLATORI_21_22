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
    std::stringstream mexAndInfo;
    mexAndInfo<<mex<<line;
    _message=mexAndInfo.str();
}

DbException::DbException(std::string mex, int idProf,std::string mex2, std::string name) {
    std::stringstream mexAndInfo;
    mexAndInfo<<mex<<idProf<<mex2<<name;
    _message=mexAndInfo.str();
}

DbException::DbException(std::string mex1, std::string id1, std::string mex2, std::string id2, std::string mex3, int line) {
    std::stringstream mexAndInfo;
    mexAndInfo<<mex1<<id1<<mex2<<id2<<mex3<<line;
    _message=mexAndInfo.str();
}
