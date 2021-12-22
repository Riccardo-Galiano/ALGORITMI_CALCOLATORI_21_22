//
// Created by lucam on 21/12/2021.
//

#include "InvalidDbException.h"
#include <sstream>

const char *InvalidDbException::what() const noexcept {
    return _message.c_str();
}

InvalidDbException::InvalidDbException(std::string m) {
    _message=m;
}

InvalidDbException::InvalidDbException(std::string mex, std::string id) {
    std::stringstream mexAndLine;
    mexAndLine<<mex<<id;
    _message=mexAndLine.str();
}

InvalidDbException::InvalidDbException(std::string mex1, std::string idCourse, std::string mex2, int idStudyCourse) {
    std::stringstream mexAndLine;
    mexAndLine<<mex1<<idCourse<<mex2<<idStudyCourse;
    _message=mexAndLine.str();
}
