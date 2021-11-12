//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_UNIVERSITYMEMBER_H
#define PROGETTO_V3_UNIVERSITYMEMBER_H


#include <string>

class UniversityMember {
public:
    UniversityMember(int, std::string, std::string, std::string);

protected:
    std::string _name;
    std::string _surname;
    int _id;
public:
    const std::string &getName() const;

    const std::string &getSurname() const;

    int getId() const;

    const std::string &getEmail() const;

protected:
    std::string _email;
};


#endif //PROGETTO_V3_UNIVERSITYMEMBER_H
