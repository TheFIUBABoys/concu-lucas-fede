//
// Created by lucas on 4/22/15.
//

#include <iostream>
#include <unistd.h>
#include "Logger.h"

void Logger::log(string value) {
    if (debug) {
        cout << getpid() << " - "<< value << endl;
    }
}
