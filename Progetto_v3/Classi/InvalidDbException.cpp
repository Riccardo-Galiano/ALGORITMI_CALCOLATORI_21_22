//
// Created by lucam on 21/12/2021.
//

#include "InvalidDbException.h"

InvalidDbException::InvalidDbException(std::string m) {
_message=m;
}

const char *InvalidDbException::what() const noexcept {
    return _message.c_str();
}
