//
// Created by Andrea on 29/11/2021.
//

#ifndef PROGETTO_V3_PARSE_HPP
#define PROGETTO_V3_PARSE_HPP


#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "Date.h"

class Parse {
public:
    ///per ogni riga del file in input scinde le varie info delimitate da ";"
    static std::vector<std::string> splittedLine(const std::string &s, const char &delimiter) {

        std::vector<std::string> toReturn; //conterrà la riga splittata nelle sue informazioni necessarie e indicizzate
        std::istringstream line(s); // mi serve per poter manipolare le stringhe
        std::string token; //buffer di appoggio per salvare l'informazione appena ricavata

        //fin quando la riga non è finita prende l'intera riga(line) e salva in una stringa del vettore di stringhe(tokens) l'informazione fino al prossimo delimitatore
        while (std::getline(line, token, delimiter)) {
            toReturn.push_back(token);
        }
        return toReturn;
    }
    ///restituisce un vettore di stringhe con i prof di ciascun corso in parallelo
    static std::vector<std::string> getProfPar(std::string &input, int num_parallel_courses) {
        std::vector<std::string> profCorsiPar;
        std::vector<int> posFinCorsiPar;
        int numCoursesParsed = 0;
        std::vector<int> posCBrackets = posCurlyBrackets(input);//prendo le posizioni delle graffe che userò per dividere gli id dei prof dei vari corsi in parallelo
        for (int i = 0; i < posCBrackets.size() - 1; i++) {//fino a quando non finiscono le parentesi graffe o il numero di corsi in parallelo
            if (input[posCBrackets[i] + 1] ==
                ']') {//la ricorrenza }] si ha alla fine di ogni corso in parallelo; se dopo una parentesi graffa si ha una quadra il corso in parallelo è finito
                posFinCorsiPar.push_back(
                        posCBrackets[i] + 2); //salva la pos della graffa successiva a quella presa in analisi: }] } <--
                numCoursesParsed++;
            }
        }
        int lastPosFin = 0;

        ///divido le info dei vari corsi in parallelo
        for (int i = 0; i < numCoursesParsed; i++) {
            profCorsiPar.push_back(input.substr(1 + lastPosFin, posFinCorsiPar[i] - lastPosFin - 1));//inserisco nel vettore di stringhe le info del corso in parallelo
            lastPosFin = posFinCorsiPar[i] + 2; //salva la pos della terza graffa: }]}, { <--
        }

        if (profCorsiPar.size() != num_parallel_courses)
            throw std::invalid_argument("docenti non congruenti con il numero di corsi paralleli");
        return profCorsiPar;
    };
    static std::vector<int> posCurlyBrackets(std::string &input) {
        std::vector<int> output;
        std::size_t found = input.find_first_of("{}");
        while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            output.push_back( found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = input.find_first_of("{}", found + 1);//continuo a controllare la stringa
        }
        return output;
    }
    static std::vector<int> posSquareBrackets(std::string &input) {
        std::vector<int> output;
        std::size_t found = input.find_first_of("[]");
        while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            output.push_back( found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = input.find_first_of("[]", found + 1);//continuo a controllare la stringa
        }
        return output;
    }
    static std::vector<std::string> SplittedGroupedID(std::string &input){
        input = input.substr(1,input.size() - 2);// tolgo le { } che racchiudono gli id
        return splittedLine(input, ',');//scissione degli id dei corsi raggruppati
    };
    static int getAcStartYear(std::string& input){
        return stoi(input.substr(0,4));
    }
    static std::vector<Date> getDates(std::string& input){
        std::vector<std::string> dates = splittedLine(input,'_');
        std::stringstream ss;
        int yy, mm, dd;
        char c;
        std::vector<Date> realDates;
        for(int i=0; i<2; i++){
            ss << dates[i];
            ss >> yy >> c >> mm >> c >> dd;
            realDates.push_back(Date(yy,mm,dd));
            ss.clear();
        }
        return realDates;
    }
    static int getMatr(std::string& input){
        int nMatr;
        char c;
        std::stringstream ss(input);
        ss >> c >> nMatr;
        return nMatr;
    }
    static std::string setId(char letterId, int numTot, int cod){
        std::stringstream ss;
        ss << letterId << std::setfill('0') <<std::setw(numTot)<<cod;
        return ss.str();
    }
};


#endif //PROGETTO_V3_PARSE_HPP
