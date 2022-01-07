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
