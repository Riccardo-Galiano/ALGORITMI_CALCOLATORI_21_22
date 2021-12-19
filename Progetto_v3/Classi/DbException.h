//
// Created by lucam on 16/11/2021.
//

#ifndef PROGETTO_V3_DBEXCEPTION_H
#define PROGETTO_V3_DBEXCEPTION_H


#include <exception>
#include <string>
#include <vector>

class DbException : public std::exception{
public:
    explicit DbException(std::string);
    explicit DbException(std::string,int);
    explicit DbException(std::string,std::string);
    explicit DbException(std::string,std::vector<std::string>,std::string,std::vector<std::string>);
    explicit DbException(std::string,std::string,std::string,int);
    explicit DbException(std::string,int,std::string,std::string);
    explicit DbException(std::string,std::string ,std::string,std::string,std::string,int);
    explicit DbException(std::string,std::string,std::string,std::string);

    const char * what() const noexcept override;
private:
    std::string _message;
};


#endif //PROGETTO_V3_DBEXCEPTION_H
