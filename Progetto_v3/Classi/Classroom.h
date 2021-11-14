//
// Created by lucam on 11/11/2021.
//

#ifndef PROGETTO_V3_CLASSROOM_H
#define PROGETTO_V3_CLASSROOM_H


#include <cstring>

class Classroom {
public:
Classroom(int , std::string, std::string, int, int);

public:
    int getId() const;
    std::string getName() const;
    bool getLab() const;
    int getNSeats() const;
    int getNExamSeats() const;

private:

    int _id; //codice identificativo
    std::string _name; //nome aula
    bool _lab; //lab o aula?
    int _nSeats; //capienza massima dell'aula
    int _nExamSeats; //capienza all'esame
};


#endif //PROGETTO_V3_CLASSROOM_H
