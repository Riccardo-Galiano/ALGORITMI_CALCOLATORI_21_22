//
// Created by lucam on 05/11/2021.
//

#ifndef PROGETTO_V3_PROFESSOR_H
#define PROGETTO_V3_PROFESSOR_H

#include <vector>
#include "UniversityMember.h"

//std::cout << std::put_time(std::localtime(&now), "%Y-%m-%d") << "\n";
class Professor : public UniversityMember {
public:
    //Professor() = delete;
    Professor():UniversityMember(){};
    Professor(int matr);
    Professor(int, std::string, std::string, std::string); ///matricola,nome, cognome, email
private:
    std::vector<std::pair<std::string, std::string>> non_avaib;
};

std::ostream &operator<<(std::ostream &prof, const Professor &s);


#endif //PROGETTO_V3_PROFESSOR_H
