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
    std::stringstream mexAndInfo;
    mexAndInfo<<mex<<id;
    _message=mexAndInfo.str();
}

InvalidDbException::InvalidDbException(std::string mex1, std::string codCourse,int idStudyCourse ) {
    std::stringstream mexAndInfo;
    mexAndInfo<<mex1<<"(codice del corso:"<<codCourse<<");(linea del file: "<<idStudyCourse<<")";
    _message=mexAndInfo.str();
}

InvalidDbException::InvalidDbException(std::string mex1, std::string nameCourse, std::string mex2, std::string codCourse, std::string mex3, std::string codGroupedCourse,
                                       std::string mex4, int line, std::string mex5, int codStudyCourse) {
    std::stringstream mexAndInfo;
    mexAndInfo<<mex1<<nameCourse<<mex2<<codCourse<<mex3<<codGroupedCourse<<mex4<<line<<mex5<<codStudyCourse;
    _message=mexAndInfo.str();

}
