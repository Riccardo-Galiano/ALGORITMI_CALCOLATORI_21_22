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

