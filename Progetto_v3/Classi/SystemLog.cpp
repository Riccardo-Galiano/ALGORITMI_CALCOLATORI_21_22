//
// Created by antonio_vespa on 12/01/22.
//

#include "SystemLog.h"


void SystemLog::appendToLog(std::string &toAppend, bool isError) {
    _log.append(toAppend);
    _notOK = isError;
}

std::string SystemLog::getLog() {
    return _log;
}

bool SystemLog::errorsOccurred() const {
    return _notOK;
}
