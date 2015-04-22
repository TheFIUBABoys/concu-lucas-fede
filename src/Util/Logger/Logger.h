//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_LOGGER_H
#define CONCUTP_LOGGER_H


#include <string>

using namespace std;
class Logger {
public:
    static Logger &logger() {
        static Logger instance;
        return instance;
    }
    void log(string value);

private:
    Logger() { };
    bool debug = true;
    // C++ 11 - Deleting the methods we don't want.
    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;

};


#endif //CONCUTP_LOGGER_H
