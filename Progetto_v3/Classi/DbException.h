//
// Created by lucam on 16/11/2021.
//

#ifndef PROGETTO_V3_DBEXCEPTION_H
#define PROGETTO_V3_DBEXCEPTION_H


#include <exception>
#include <string>

class DbException : public std::exception{
public:
    explicit DbException(std::string);
    explicit DbException(std::string,int);
    const char * what() const noexcept override;
private:
    std::string _message;
};


#endif //PROGETTO_V3_DBEXCEPTION_H
