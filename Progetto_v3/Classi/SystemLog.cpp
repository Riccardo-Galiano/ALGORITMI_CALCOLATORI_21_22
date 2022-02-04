//
// Created by lucam on 12/01/22.
//

#include "SystemLog.h"

///ciò che serve stampare
void SystemLog::appendToLog(std::string &toAppend, bool isError) {
    _log.append(toAppend);
    if(!_notOK && isError)
        _notOK = isError;
}

///prende _log
std::string SystemLog::getLog() const {
    return _log;
}

bool SystemLog::errorsOccurred() const {
    return _notOK;
}
