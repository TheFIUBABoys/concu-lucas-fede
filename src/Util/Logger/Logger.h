//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_LOGGER_H
#define CONCUTP_LOGGER_H

#include "../Locks/LockFile.h"
#include "../../Config/Config.h"


#include <string>

using namespace std;

class Logger {
public:
    static Logger &logger() {
        //remove(LOG_FILE);
        static Logger instance;
        return instance;
    }

    void log(string value);

    bool debug = true;
private:
    Logger() { }

    LockFile lock = LockFile(LOG_FILE);

    // C++ 11 - Deleting the methods we don't want.
    Logger(Logger const &) = delete;

    void operator=(Logger const &) = delete;

};


#endif //CONCUTP_LOGGER_H
