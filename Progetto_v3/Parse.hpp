//
// Created by andre on 29/11/2021.
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
    ///controlla che ci sia una chiocciola nell'email
    static bool controlSnailCharacter(std::string email){
        int countSnailCharacter = 0;
        for(int i = 0 ;i<email.size(); i++){
            if(email[i] == '@'){
                countSnailCharacter++;
            }
        }
        return countSnailCharacter == 1;
    }
    ///restituisce un vettore di stringhe con i prof di ciascun corso in parallelo
    static std::vector<std::string> getProfPar(std::string &input, int num_parallel_courses) {
        std::vector<std::string> profCorsiPar;
        std::vector<int> posFinCorsiPar;
        int numCoursesParsed = 0;
        //prendo le posizioni delle graffe che userò per dividere gli id dei prof dei vari corsi in parallelo
        std::vector<int> posCBrackets = posCurlyBrackets(input);
        //fino a quando non finiscono le parentesi graffe o il numero di corsi in parallelo
        for (int i = 0; i < posCBrackets.size() - 1; i++) {
            //la ricorrenza }] si ha alla fine di ogni corso in parallelo; se dopo una parentesi graffa si ha una quadra il corso in parallelo è finito
            if (input[posCBrackets[i] + 1] == ']') {
                posFinCorsiPar.push_back(posCBrackets[i] + 2); //salva la pos della graffa successiva a quella presa in analisi: }] } <--
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
            throw std::invalid_argument("Numero di raggruppamenti dei professori non congruente con il numero di corsi paralleli ");
        return profCorsiPar;
    };

    ///controlla le informazioni in input per l'esame
    static void controlExamInfo(std::vector<std::string> examData){

          int examDuration = Parse::checkedStoi(examData[0]," della durata dell'esame ");
          if(examDuration == 0){
             throw std::invalid_argument("La durata dell'esame non puo' essere zero \n");

          }
          int timeEntry = Parse::checkedStoi(examData[1]," del tempo di entrata ");
          if( timeEntry == 0){
              throw std::invalid_argument("Il tempo di entrata non puo' essere zero \n ");

          }
          int exitTime = Parse::checkedStoi(examData[2]," del tempo di uscita ");
          if(exitTime == 0){
              throw std::invalid_argument("Il tempo di uscita non puo' essere zero \n");

          }
          std::string modality = examData[3];
          if( modality != "S" && modality != "O" && modality != "SO" && modality != "P"){
              throw std::invalid_argument("La modalita' d'esame non e' S/O/SO/P \n ");
          }
          std::string place = examData[4];
          if(place != "A" && place != "B")
              throw std::invalid_argument("Il luogo dell'esame non e' A/L \n");

    }

    ///prende le posizioni delle parentesi graffe in una stringa
    static std::vector<int> posCurlyBrackets(std::string &input) {
        std::vector<int> output;
        std::size_t found = input.find_first_of("{}");
        while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            output.push_back( found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = input.find_first_of("{}", found + 1);//continuo a controllare la stringa
        }
        return output;
    }

    ///prende le posizioni delle parentesi quadre in una stringa
    static std::vector<int> posSquareBrackets(std::string &input) {
        std::vector<int> output;
        std::size_t found = input.find_first_of("[]");
        while (found != std::string::npos) {//massimo valore per variabile di tipo size_t. In altre parole il fine stringa
            output.push_back( found);//prendo la posizione del carattere trovato dalla find_first_of e lo inserisco in un vettore posizioni
            found = input.find_first_of("[]", found + 1);//continuo a controllare la stringa
        }
        return output;
    }

    ///prende le posizioni dei punti e virgola in una stringa
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

    ///splitta gli id dei raggruppati
    static std::vector<std::string> SplittedGroupedID(std::string &input){
        input = input.substr(1,input.size() - 2);// tolgo le { } che racchiudono gli id
        return splittedLine(input, ',');//scissione degli id dei corsi raggruppati
    };

    ///prende il primo anno di un anno accademico
    static int getAcStartYear(std::string& input){
        std::string acStartYear = input.substr(0, 4);
        try {
            return checkedStoi(acStartYear," dell'anno accademico.");
        }catch (std::invalid_argument& err){
            std::string generalError = err.what();
            throw std::invalid_argument( generalError + " La stringa in input non puo' essere un anno \n");
        }
    }

    ///prende delle date da una stringa
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

    ///prende la matricola prima di lettere identificative all'inizio
    static int getMatr(std::string& input){

            std::string nMatr;
            char c;
            std::stringstream ss(input);
            ss >> c >> nMatr;
            return Parse::checkedStoi(nMatr, " della matricola");

    }

    ///setta l'id per un prof o studente
    static std::string setId(char letterId, int numTot, int cod){
        std::stringstream ss;
        ss << letterId << std::setfill('0') <<std::setw(numTot)<<cod;
        return ss.str();
    }

    ///controlla se un vettore sia vuoto
    static bool controlFieldsVectorAreEmpty(std::vector<std::string> infoVector) {
        for (int i = 0; i < infoVector.size(); i++) {
            if (infoVector[i].empty())
                return true;
        }
        return false;
    }

    ///cotrolla se la stringa possa essere un ID
    static bool controlItCanBeAnId(std::string Id, int numMatr,char letter) {
        std::stringstream ss(Id);
        char c;
        std::string matrWithoutLetter;
        ss >> c;
        if(c != letter){
            return false;
        }
        ss >> matrWithoutLetter;
        if (matrWithoutLetter.size() != numMatr) {
            return false;
        }
        try {
            checkedStoi(matrWithoutLetter," della matricola");
        } catch (std::invalid_argument &err) {
            return false;
        }
        return true;
    }

    ///contorlla la sintassi dei corsi paralleli in addCourses e InsertCourses????????
    static void checkSyntaxProfInfoANDExamData(std::string &str){
        if(str[0] == '{' || str[str.length()-1] =='}'){
            std::string string = str.substr(1, str.size() - 2);
            std::vector<std::string> splitting = splittedLine(string,',');
            if(splitting.size() == 4)
                return;
        }
        throw std::invalid_argument("ERRORE FORMATO");
    }

    ///vedere se un carattere e utilizzabile per l'assegnazione corsi
    static bool thisCharShouldBeConsidered(char c) {
        if (c == '[' || c == ']' || c == '{' || c == '}')
            return true;
        else
            return false;
    }

    ///controlla la sintassi dei corsi paralleli
    static void checkSyntaxProfs(std::string &str){
        int state = 0;
        bool isNotOk = false;
        for (int i = 0; i < str.length(); i++) {
            if (thisCharShouldBeConsidered(str[i])) {
                char c = str[i];
                switch (state) {
                    case 0: {
                        if(c == '[')
                            state = 1;
                        else
                            isNotOk = true;
                        break;
                    }
                    case 1: {
                        if(c == '{')
                            state = 2;
                        else if(c == ']')
                            state = 0;
                        else
                            isNotOk = true;
                        break;
                    }
                    case 2: {
                        if(c == '[')
                            state = 3;
                        else if(c == '}')
                            state = 1;
                        else
                            isNotOk = true;
                        break;
                    }
                    case 3: {
                        if(c == '{')
                            state = 4;
                        else if(c == ']')
                            state = 2;
                        else
                            isNotOk = true;
                        break;
                    }
                    case 4: {
                        if(c == '}')
                            state = 3;
                        else
                            isNotOk = true;
                        break;
                    }
                }
            }
        }
        if(isNotOk == true || state != 0)
            throw std::invalid_argument("Errore del formato dei corsi paralleli ");
    }

    ///controllo se la stringa passata possa essere un anno accademico
    static bool controlItCanBeAnAcYear(std::string input) {
        //AAAA-AAAA
        std::vector<std::string> acYear = Parse::splittedLine(input,'-');
        if(acYear.size() != 2){
            throw std::invalid_argument("Formato dell'anno accademico errato\n");
        }
        int acStart = checkedStoi(acYear[0]," dell'anno accademico");
        int acEnd = checkedStoi(acYear[1]," dell'anno accademico");
        if((acStart <= 1900 || acStart >= 2100) || (acEnd<= 1900 || acEnd >= 2100)){
            throw std::invalid_argument("L'anno accademico deve essere incluso tra il 1900 e il 2100\n");
        }
        return acEnd == acStart+1;
    }

    ///controllo del formato per i piano di studio
    static void controlStudyPlanFormat(std::vector<int> pos,std::string line) {
        if (pos.size() != 2) {
            throw std::invalid_argument(" devono esserci due graffe\n");

        } else {
            if (pos[0] != 18 || line[18] != 123) {// s000000;AAAA-AAAA; (dopo 18 caratteri deve esserci un '{' che in ASCII è 123)
                //se la prima parentesi graffa non si trova in posizione 18 del rigo o non è una '{'
                throw std::invalid_argument(" il piano di studio va racchiuso da due graffe{}, le uniche presenti \n");

            } else if (pos[1] != line.size() - 1 || line[line.size() - 1] != 125) {//l'ultimo carattere deve essere una '}'
                throw std::invalid_argument(" il piano di studio va racchiuso da due graffe{}, le uniche presenti\n");

            }
        }
    }

    ///controllo che la stringa in ingresso possa essere un ID o no
    static void controlItCanBeACourseId(std::string idCourse) {
        if(idCourse.size() != 7)
            throw std::invalid_argument("Errore formato corso");
        for(int i = 0; i<idCourse.size(); i++){
            if(i == 0 || i == 1){
                if(idCourse[i] < 48 || idCourse[i]> 59)
                    throw std::invalid_argument("Errore formato corso");
            } else{
                if (idCourse[i] < 65 || idCourse[i] > 90)
                    throw std::invalid_argument("Errore formato corso");
            }
        }

    }

    ///controllo che possa essere una data
    static Date controlItCanBeADate(std::string input) {
        //AAAA-MM-GG
        std::vector<std::string> date = Parse::splittedLine(input,'-');
        if(date.size() != 3)
            throw std::invalid_argument("Formato errato della data");
        int year = Parse::checkedStoi(date[0]," di un anno");
        int month = Parse::checkedStoi(date[1]," di un mese");
        int day = Parse::checkedStoi(date[2]," di un giorno");
        return Date(input);

    }

    ///controllo sui numeri, convertibili ad intero positivo
    static int checkedStoi(std::string& input,std::string specificError){
        for(int i=0; i<input.size(); i++){
            char currentChar = input[i];
            if(currentChar < 48 || currentChar > 57)
                throw std::invalid_argument("Errore conversione " + specificError + " da stringa ad intero positivo\n");
        }
          return stoi(input);
    }
};


#endif //PROGETTO_V3_PARSE_HPP
