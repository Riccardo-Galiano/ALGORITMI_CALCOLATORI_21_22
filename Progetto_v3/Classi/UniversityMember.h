//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_UNIVERSITYMEMBER_H
#define PROGETTO_V3_UNIVERSITYMEMBER_H


#include <string>
#include "Date.h"

class UniversityMember {
    typedef struct {
        Date _birth;
        Date _registrationOrEntry;
        std::string _address;
    } updateMember;

public:
    ///constructor
    UniversityMember();
    UniversityMember(const int & matr, const std::string & name, const std::string & surname,const  std::string & email);
    UniversityMember(const int & matr, const std::string & name, const std::string & surname,const  std::string & email,const std::string & birth, const std::string & registrationOrEntry,const  std::string & address);

    ///getter
    const std::string &getName() const;
    const std::string &getSurname() const;
    const int getId() const;
    const std::string &getEmail() const;
    std::string getOtherInfoString() const ;


    ///update
    void setName(const std::string &name);
    void setSurnName(const std::string & surname);
    void setEmail(const std::string & eMail);



protected:
    std::string _name;
    std::string _surname;
    int _id;
    std::string _email;
    updateMember _othersInfoMember;
};


#endif //PROGETTO_V3_UNIVERSITYMEMBER_H
