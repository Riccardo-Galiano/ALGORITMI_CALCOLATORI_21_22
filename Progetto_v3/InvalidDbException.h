//
// Created by lucam on 21/12/2021.
//
#include <exception>
#include <string>
#ifndef PROGETTO_V3_INVALIDDBEXCEPTION_H
#define PROGETTO_V3_INVALIDDBEXCEPTION_H


class InvalidDbException : public std::exception {
public:
    explicit InvalidDbException(std::string m);
    const char * what() const noexcept override;
private:
    std::string _message;

};


#endif //PROGETTO_V3_INVALIDDBEXCEPTION_H
