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

    Classroom(int id, std::string AorL, std::string name, int seats, int examSeats);
    Classroom(int id, std::string AorL, std::string name, int seats, int examSeats,int blackBoard,int computer,int drawingTable,int projector);

    const int getId() const;
    const std::string getName() const;
    const bool isThisLab() const;
    const int getNSeats() const;
    const int getNExamSeats() const;

    void setName(const std::string& name);
    void setType(const bool& LorA);
    void setNSeats(const int& NSeats);
    void setNExamSeats(const int& NExamSeats);
    bool checkAvailability(Date& date,int slotHour);
    void setDisavailability(Date& date,int slotsStartHour,int numSlot);
    std::string getOthersInfo();
    int getDrawingTable();
    int getComputer();
    int getProjector();
    int getBlackBoard();
    void setDrawingTable(int drawingTable );
    void setComputer(int computer);
    void setProjector(int projector);
    void setBlackBoard(int blackBoard);

    ///remove
    void eraseThisAppealFromClassrooms(Date& date, int startSlot, int numSlots);
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
    std::vector<std::string> _errorStringClassrooms;

};
std::ostream &operator<<(std::ostream &room, const Classroom &c);

#endif //PROGETTO_V3_CLASSROOM_H
