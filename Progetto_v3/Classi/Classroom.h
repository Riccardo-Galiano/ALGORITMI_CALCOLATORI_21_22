#ifndef PROGETTO_V3_CLASSROOM_H
#define PROGETTO_V3_CLASSROOM_H


#include <cstring>
#include <map>

class Classroom {
public:

    Classroom(int , std::string, std::string, int, int);

    const int getId() const;
    const std::string getName() const;
    const bool getLab() const;
    const int getNSeats() const;
    const int getNExamSeats() const;

    void updateName(const std::string&);
    void updateType(const bool&);
    void updateNSeats(const int&);
    void updateNExamSeats(const int&);

    std::string setCod(int) const ;

    bool checkAvailability(std::string &);
    void setDisavailability(std::string&);
private:

    int _id; //codice identificativo
    std::string _name; //nome aula
    bool _lab; //lab o aula?
    int _nSeats; //capienza massima dell'aula
    int _nExamSeats; //capienza all'esame
    //key: "Date;Slot"
    //value: non ci interessa.. se esiste la tupla -> allora occupata!
    std::map<std::string,int> _disavailableRooms;

};
std::ostream &operator<<(std::ostream &room, const Classroom &s);

#endif //PROGETTO_V3_CLASSROOM_H
