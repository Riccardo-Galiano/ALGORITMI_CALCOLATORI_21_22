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
    const int getId() const;
    const std::string getName() const;
    const bool getLab() const;
    const int getNSeats() const;
    const int getNExamSeats() const;
    void updateName(const std::string & name);
    void updateType(const bool & LorA);
    void updateNSeats(const int & NSeats);
    void updateNExamSeats(const int & NExamSeats);
private:

    int _id; //codice identificativo
    std::string _name; //nome aula
    bool _lab; //lab o aula?
    int _nSeats; //capienza massima dell'aula
    int _nExamSeats; //capienza all'esame
};


#endif //PROGETTO_V3_CLASSROOM_H
