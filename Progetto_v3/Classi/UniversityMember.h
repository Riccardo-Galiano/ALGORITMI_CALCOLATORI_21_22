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
    UniversityMember(const int &, const std::string &, const std::string &,const  std::string &);
    UniversityMember(const int &, const std::string &, const std::string &,const  std::string &,const std::string &, const std::string &,const  std::string &);

    ///getter
    const std::string &getName() const;
    const std::string &getSurname() const;
    const int getId() const;
    const std::string &getEmail() const;
    std::string getOtherInfoString();
    Date getBirth();
    Date getRegistrationOrEntry();
    std::string getAddress();

    ///update
    void setName(const std::string &);
    void setSurnName(const std::string &);
    void setEmail(const std::string &);
    void setBirth(const std::string &);
    void setRegistration(const std::string &);
    void setAdress(const std::string &);


protected:
    std::string _name;
    std::string _surname;
    int _id;
    std::string _email;
    updateMember _othersInfoMember;
};


#endif //PROGETTO_V3_UNIVERSITYMEMBER_H
