//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_UNIVERSITYMEMBER_H
#define PROGETTO_V3_UNIVERSITYMEMBER_H


#include <string>

class UniversityMember {
public:
    UniversityMember();

    UniversityMember(const int &, const std::string &, const std::string &,const  std::string &);

    const std::string &getName() const;
    const std::string &getSurname() const;
    const int getId() const;
    const std::string &getEmail() const;

    void updateName(const std::string &);
    void updateSurnName(const std::string &);
    void updateEmail(const std::string &);

    const std::string setId(int) const;

protected:
    std::string _name;
    std::string _surname;
    int _id;
    std::string _email;
};


#endif //PROGETTO_V3_UNIVERSITYMEMBER_H
