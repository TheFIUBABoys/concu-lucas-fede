//
// Created by lucas on 4/22/15.
//

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "Logger.h"

void Logger::log(string value) {
    string toPrint = string("\n") + to_string(getpid()) + string(" - ") + value ;
    cout << toPrint;
    if (debug) {
        lock.tomarLockWr();
        lock.escribir(toPrint.c_str(), strlen(toPrint.c_str()));
        lock.liberarLock();
    }

}
