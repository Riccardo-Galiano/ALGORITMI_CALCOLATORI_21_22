#ifndef PROGETTO_V3_CLASSROOM_H
#define PROGETTO_V3_CLASSROOM_H


#include <cstring>
#include <map>
#include "Date.h"
#include "Course.h"
typedef struct {
    int _drawingTable;
    int _computer;
    int _projector;
    int _blackBoard;
} updateClassroom;

class Classroom {
public:

    Classroom(int , std::string, std::string, int, int);
    Classroom(int , std::string, std::string, int, int,int,int,int,int);

    const int getId() const;
    const std::string getName() const;
    const bool getLab() const;
    const int getNSeats() const;
    const int getNExamSeats() const;

    void updateName(const std::string&);
    void updateType(const bool&);
    void updateNSeats(const int&);
    void updateNExamSeats(const int&);
    bool checkAvailability(Date&,int);
    void setDisavailability(Date&,int,int);
    std::string getOthersInfo();
    int getDrawingTable();
    int getComputer();
    int getProjector();
    int getBlackBoard();
    void updateDrawingTable(int);
    void updateComputer(int);
    void updateProjector(int);
    void updateBlackBoard(int);
private:

    int _id; //codice identificativo
    std::string _name; //nome aula
    bool _lab; //lab o aula?
    int _nSeats; //capienza massima dell'aula
    int _nExamSeats; //capienza all'esame
    //key: "Date;Slot"
    //value: non ci interessa.. se esiste la tupla -> allora occupata!
    std::map<std::string,int> _disavailableRooms;
    updateClassroom _othersInfoClassroom;

};
std::ostream &operator<<(std::ostream &room, const Classroom &c);

#endif //PROGETTO_V3_CLASSROOM_H
