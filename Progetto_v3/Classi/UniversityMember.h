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
    void updateName(const std::string &);
    void updateSurnName(const std::string &);
    void updateEmail(const std::string &);
    void updateBirth(const std::string &);
    void updateRegistration(const std::string &);
    void updateAdress(const std::string &);


protected:
    std::string _name;
    std::string _surname;
    int _id;
    std::string _email;
    updateMember _othersInfoMember;
};


#endif //PROGETTO_V3_UNIVERSITYMEMBER_H
