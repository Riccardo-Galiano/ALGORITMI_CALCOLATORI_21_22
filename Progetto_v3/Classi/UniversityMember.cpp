//
// Created by lucam on 05/11/2021.
//

#include <sstream>
#include <iomanip>
#include "UniversityMember.h"

UniversityMember::UniversityMember(const int &matricola, const std::string &name, const std::string &surname, const std::string &email) {
    _name = name;
    _surname = surname;
    _email = email;
    _id = matricola;
    _othersInfoMember._birth = Date();
    _othersInfoMember._registrationOrEntry = Date();
    _othersInfoMember._address = "indefinito";
}
UniversityMember::UniversityMember(const int & matricola, const std::string & name, const std::string &surname, const std::string &email,
                                   const std::string &birth, const std::string &registrationOrEntry, const std::string &address) {
    _name = name;
    _surname = surname;
    _email = email;
    _id = matricola;
    _othersInfoMember._birth = birth;
    _othersInfoMember._registrationOrEntry = registrationOrEntry;
    _othersInfoMember._address = address;
}

std::string UniversityMember::getOtherInfoString() const{
    std::stringstream ss;
    ss << _othersInfoMember._birth <<";"<< _othersInfoMember._registrationOrEntry << ";" <<_othersInfoMember._address;
    return ss.str();
}


///prende il nome, solo lettura
const std::string &UniversityMember::getName() const {
    return _name;
}

///prende il cognome, solo lettura
const std::string &UniversityMember::getSurname() const {
    return _surname;
}

///prende la matricola, solo lettura
const int UniversityMember::getId() const {
    return _id;
}

///prende l'email, solo lettura
const std::string &UniversityMember::getEmail() const {
    return _email;
}

///aggiorna il nome
void UniversityMember::setName(const std::string& name) {
    _name = name;

}

///aggiorna il cognome
void UniversityMember::setSurnName(const std::string& surName) {
    _surname = surName;

}

///aggiorna l'email
void UniversityMember::setEmail(const std::string & eMail) {
    _email = eMail;
}

UniversityMember::UniversityMember() {}


Date UniversityMember::getBirth() const {
    return _othersInfoMember._birth;
}

Date UniversityMember::getRegistrationOrEntry() const {
    return _othersInfoMember._registrationOrEntry;
}

std::string UniversityMember::getAddress() const {
    return _othersInfoMember._address;
}

void UniversityMember::setBirth(const std::string & birth) {
  _othersInfoMember._birth = birth;
}

void UniversityMember::setRegistration(const std::string &registrationOrEntry) {
_othersInfoMember._registrationOrEntry = registrationOrEntry;
}

void UniversityMember::setAdress(const std::string & address) {
_othersInfoMember._address = address;
}


