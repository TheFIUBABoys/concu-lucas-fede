//
// Created by lucas on 4/22/15.
//

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "Logger.h"
#include "../Locks/LockFile.h"
#include "../../Config/Config.h"

#define LINE_SIZE   MESSAGE_LENGTH + 10

Logger::Logger() {
    remove(LOG_FILE);
}

void Logger::log(string value) {
    char toPrint[LINE_SIZE];
    sprintf(toPrint, "%ld - %s\n", (long int)getpid(), value.c_str());
    if (debug) {
        cout << toPrint;
    }

    LockFile lock ( LOG_FILE );
    lock.tomarLockWr();
    lock.escribir(static_cast<const void *>(toPrint), strlen(toPrint));
    lock.liberarLock();
}
