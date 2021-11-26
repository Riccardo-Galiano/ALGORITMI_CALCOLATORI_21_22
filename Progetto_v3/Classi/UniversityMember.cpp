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
void UniversityMember::updateName(const std::string& name) {
    _name = name;

}

///aggiorna il cognome
void UniversityMember::updateSurnName(const std::string& surName) {
    _surname = surName;

}

///aggiorna l'email
void UniversityMember::updateEmail(const std::string & eMail) {
    _email = eMail;
}

UniversityMember::UniversityMember() {}

const std::string UniversityMember::setId(int nMatr) const {
    std::stringstream output;
    output<<std::setfill('0')<<std::setw(6)<<nMatr;
    return output.str();
}
