//
// Created by lucam on 11/11/2021.
//

#ifndef PROGETTO_V3_CLASSROOM_H
#define PROGETTO_V3_CLASSROOM_H


#include <string>

class Classroom {
public:
Classroom(int,char,std::string,int,int);
private:
    int _id;
public:
    int getId() const;

private:
    std::string _name;
    bool _lab;
    int _nSeats;
    int _nExamSeats;
};


#endif //PROGETTO_V3_CLASSROOM_H
