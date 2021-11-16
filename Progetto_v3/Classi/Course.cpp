//
// Created by lucam on 05/11/2021.
//

#include "Course.h"

Course::Course(const std::string &idCorso) {
_id = idCorso;

}

Course::Course(const std::string &idCorso, const std::string &nomeCorso, const int &oreLezione, const int &oreEsercitazione, const int &oreLaboratorio) {
_id = idCorso;
_name = nomeCorso;
_hours._lec = oreLezione;
_hours._ex = oreEsercitazione;
_hours._lab = oreLaboratorio;
}
