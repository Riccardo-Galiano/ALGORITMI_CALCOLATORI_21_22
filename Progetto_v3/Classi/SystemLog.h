//
// Created by andre on 12/01/22.
//

#ifndef PROGETTO_V3_SYSTEMLOG_H
#define PROGETTO_V3_SYSTEMLOG_H


#include <string>

class SystemLog {
public:
    void appendToLog(std::string& toAppend, bool isError);
    std::string getLog();
    bool errorsOccurred() const;
private:
    std::string _log;
    bool _notOK;
};


#endif //PROGETTO_V3_SYSTEMLOG_H
