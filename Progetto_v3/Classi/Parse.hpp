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
    static std::vector<int> posSemiColon(std::string &input) {
        std::vector<int> output;
        std::size_t found = input.find_first_of(';');
        while (found !=
               std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            output.push_back(found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = input.find_first_of(';', found + 1);//continuo a controllare la stringa
        }
        return output;
    }
    static std::vector<std::string> SplittedGroupedID(std::string &input){
        input = input.substr(1,input.size() - 2);// tolgo le { } che racchiudono gli id
        return splittedLine(input, ',');//scissione degli id dei corsi raggruppati
    };
    static int getAcStartYear(std::string& input){
        std::string acStartYear = input.substr(0, 4);
        try {
            return checkedStoi(acStartYear,"Errore anno accademico.");
        }catch (std::invalid_argument& err){
            std::string generalError = err.what();
            throw std::invalid_argument( generalError + " La stringa in input non puo' essere un anno \n");
        }
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
    static bool controlFieldsVectorAreEmpty(std::vector<std::string> infoVector) {
        for (int i = 0; i < infoVector.size(); i++) {
            if (infoVector[i].empty())
                return true;
        }
        return false;
    }
    static bool controlItCanBeAnId(std::string Id, int numMatr) {
        std::stringstream ss(Id);
        char c;
        std::string matrWithoutLetter;
        ss >> c >> matrWithoutLetter;
        if (matrWithoutLetter.size() != numMatr) {
            return false;
        }
        try {
            checkedStoi(matrWithoutLetter,"Errore matricola");
        } catch (std::invalid_argument &err) {
            return false;
        }
        return true;
    }

    static bool controlItCanBeAnAcYear(std::string input) {
        //AAAA-AAAA
        if (input.size() != 9)
            return false;
        std::string acStartYear = input.substr(0, 4);
        std::string acEndYear = input.substr(5, 4);
        int acStart;
        int acEnd;
        try {
            acStart = checkedStoi(acStartYear,"Errore anno accademico");
        }catch (std::invalid_argument& err){
            return false;
        }
        try {
            acEnd = checkedStoi(acEndYear,"Errore anno accademico");
        }catch (std::invalid_argument& err){
            return false;
        }
        return acStart + 1 == acEnd;
    }
    static int checkedStoi(std::string& input,std::string specificError){
        for(int i=0; i<input.size(); i++){
            char currentChar = input[i];
            if(currentChar < 48 || currentChar > 57)
                throw std::invalid_argument("Errore conversione della stringa in intero." + specificError +"\n");
        }
        if(stoi(input) < 0)
            throw  std::invalid_argument("Errore numero negativo."+ specificError +"\n");
        else
            return stoi(input);
    }
};


#endif //PROGETTO_V3_PARSE_HPP
