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
