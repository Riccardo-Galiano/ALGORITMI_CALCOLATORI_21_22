//
// Created by lucam on 05/11/2021.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "University.h"

///costruisce la struttura dell'intera università leggendo i file da linea di comando
University::University() {
    readStudents();
    readProfessor();
    readClassroom();

}

///per ogni riga del file in input scinde le varie info delimitate da ";"
std::vector<std::string> splittedLine(const std::string &s,char delimiter ) {


    std::vector<std::string> toReturn; //conterrà la riga splittata nelle sue informazioni necessarie e indicizzate con vector
    std::istringstream line(s); // mi serve per per poter manipolare le stringhe
    std::string token; //buffer di appoggio per salvare l'informazione appena ricavata


    //fin quando la riga non è finita prende l'intera riga e salva in un vettore di string l'informazione fino al prossimo delimitatore
    while (std::getline(line, token, delimiter)) {
        toReturn.push_back(token);
    }

    return toReturn;
}




/// come faccio a creare il file se non lo trovo?

void University::readStudents() {
    std::ifstream fileIn("../Sources/db_studenti.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw std::invalid_argument("errore apertura database studenti");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens;    //accoglierà il vettore con la riga del file scissa
    char c;    //accoglierà la s della matricola presa dal file, a noi inutile
    int nMatr; //accoglierà il codice identificativo della matricola presa dal file, a noi utile


    while (std::getline(fileIn, line)) {//finchè il file non sarà finito
        tokens = splittedLine(line,';');

        std::stringstream ss(tokens[0]); //manipolo la stringa della matricola
        ss >> c >> nMatr; //la "s" la scarto in "c", tengo il codice identificativo da mettere in un intero

        ///controllo se la matricola è già esistente; in quel caso lancio un'eccezione, altrimenti inserisco lo studente con tutti i suoi dati
        if (_students.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _students.insert(std::pair<int, Student>(nMatr, Student(nMatr, tokens[1], tokens[2], tokens[3])));//la chiave intera è la matricola; ad ogni chiave/matricola è associato uno studente
    }

    fileIn.close();
}

///inserisco un nuovo studente
bool University::insertStuds(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in); //apro il file
    if (!fIn.is_open()) { //controllo se il file è aperto
        std::cerr << "rotto\n";
        return false;
    }
    std::string line; //stringa di appoggio
    std::vector<std::string> tokens; //vettore di stringhe che accoglierà il ritorno della funzione split
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line,';');
        int matr = getNewStudentId(); //calcolo la matricola del nuovo studente
        _students.insert(std::pair<int, Student>(matr, Student(matr, tokens[0], tokens[1], tokens[2]))); //inserisco il nuovo studente nella mappatura interna
    }
    fIn.close();

    return true;
}

///mi serve capire qual è la nuova matricola da associare al nuovo studente
int University::getNewStudentId() {
   // int newStudId = 0;

    ///non mi piace. Associa una vecchia matricola a un nuovo studente
   /* for (int i = 0; i < _students.size(); i++) {
        Student s = _students.at(i);
        if (s.getId() != newStudId)
            return newStudId;
        else
            newStudId++;
    }
     return newStudId;*/

    Student s = _students.at(_students.size()-1);
    return s.getId()+1; //il nuovo studente avrà la matricola successiva all'ultimo studente iscritto

}

///identico alla readStudents(); si evita di commentare per non sporcare il codice
void University::readProfessor() {
    std::ifstream fileIn("../Sources/db_professori.txt");
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura database professori");
    }
    std::string line;
    std::vector<std::string> tokens;
    char c;
    int nMatr;
    while (std::getline(fileIn, line)) {
        tokens = splittedLine(line,';');
        std::stringstream ss(tokens[0]);
        ss >> c >> nMatr;
        if (_professors.count(nMatr))
            throw std::logic_error("due matricole uguali");
        else
            _professors.insert(std::pair<int, Professor>(nMatr, Professor(nMatr, tokens[1], tokens[2], tokens[3])));
    }
    fileIn.close();
}

///identico alla insertStudents(); si evita di commentare per non sporcare il codice
bool University::insertProfessors(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in);
    if (!fIn.is_open()) {
        std::cerr << "rotto\n";
        return false;
    }
    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line,';');
        int matr = getNewProfessorId();
        _professors.insert(std::pair<int, Professor>(matr, Professor(matr, tokens[0], tokens[1], tokens[2])));
    }
    fIn.close();
    return true;
}

///identico alla getNewStudentsId(); si evita di commentare per non sporcare il codice
int University::getNewProfessorId() {


    Professor p = _professors.at(_students.size()-1);
    return p.getId()+1; //il nuovo professore avrà la matricola successiva all'ultimo professore iscritto

}

void University::readClassroom() {
    std::ifstream fileIn("../Sources/db_aule.txt");
    if (!fileIn.is_open()) {
        throw std::invalid_argument("errore apertura database aule");
    }
    std::string line;
    std::vector<std::string> tokens;
    char c;
    int nCod=0;
    while (std::getline(fileIn, line)) {
        tokens = splittedLine(line,';');
        std::stringstream ss(tokens[0]);
        ss >> c >> nCod;

        if (_classroom.count(nCod))
            throw std::logic_error("due codici uguali");
        else {
            _classroom.insert(std::pair<int, Classroom>(nCod, Classroom(nCod, tokens[1], tokens[2], std::stoi(tokens[3]), std::stoi(tokens[4]))));

        }
    }
    fileIn.close();
}



bool University::insertClassroom(const std::string &fileIn) {
    std::fstream fIn(fileIn, std::ios::in); ///piccolezza: abbiamo aperto gli altri file con ifstream, lasciamo fstream per far vedere che conosciamo altro o uniformiamo il codice?
    if (!fIn.is_open()) {
        throw std::invalid_argument("errore apertura database aule");
        return false;
    }
    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(fIn, line)) {
        tokens = splittedLine(line,';');
        int id = getNewClassroomId();
        _classroom.insert(std::pair<int, Classroom>(id, Classroom(id, tokens[1], tokens[2], std::stoi(tokens[3]),std::stoi(tokens[4]))));
    }
    fIn.close();
    return true;
}


/// potrebbe non esserci niente ad un certo i
/// il codice dell'aula può essere riassegnato?
int University::getNewClassroomId() {
    /*int newRoomId = 0;
    for (int i = 0; i < _classroom.size(); i++) {
        if (_classroom.at(i).getId() != newRoomId)
            return newRoomId;
        else
            newRoomId++;
    }
    return newRoomId;*/
    Classroom c = _classroom.at(_classroom.size()-1);
    return c.getId()+1; //la nuova aula avrà la matricola successiva all'ultima della mappa
}


//primo approccio, da cambiare
void University::readStudyCourse() {
    char c;
    std::ifstream fileIn("../Sources/db_corsi_studi.txt");
    if (!fileIn.is_open()) {
        //std::cerr << "errore apertura database studenti" << std::endl;
        throw std::invalid_argument("errore apertura database corsi di studi");
    }
    std::string line;     //stringa di appoggio in cui mettere l'intero rigo
    std::vector<std::string> tokens1;    //accoglierà il vettore con la riga del file scissa
    std::vector<std::string> tokens2;
    std::vector<std::string> tokens3;
    std::vector<std::string> tokens4;

    while(getline(fileIn,line)){

       tokens1 = splittedLine(line,';');

        std::stringstream ss(tokens1[2]);
        ss>>c>>c;
        tokens2 = splittedLine(tokens1[2],'}');
        for(int i = 0; i<tokens2.size();i++){
            if (i != 0) {
                std::stringstream s1(tokens1[2]);
                s1>>c>>c;
            }
                tokens3 = splittedLine(tokens2[i], ',');
                std::stringstream s2(tokens1[3]);
                s2>>c;
                for(int j = 0; j<tokens1[3].size();j++)
                tokens4 = splittedLine(tokens3[j],',');
                

            }


        ///inserire nella mappa prendendo i tokens giusti
        }



    }




