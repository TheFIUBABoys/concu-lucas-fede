//
// Created by lucas on 4/22/15.
//

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "Logger.h"

void Logger::log(string value) {
    string stdoutPrint = to_string(getpid()) + string(" - ") + value;
    int color = getpid() % 8 + 30;
    stdoutPrint = "\033[1;"+to_string(color)+ "m" + stdoutPrint +  "\033[0m";
    cout << stdoutPrint  << endl;
    string toPrint = string("\n") + stdoutPrint;
    if (debug) {
        lock.tomarLockWr();
        lock.escribir(toPrint.c_str(), strlen(toPrint.c_str()));
        lock.liberarLock();
    }

}
