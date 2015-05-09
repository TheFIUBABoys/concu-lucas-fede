//
// Created by fede on 09/05/15.
//

#ifndef CONCUTP_PROCESS_H
#define CONCUTP_PROCESS_H

#include "../../Util/Seniales/SIGINT_Handler.h"

class Process {
public:
    Process();
protected:
    SIGINT_Handler sigint_handler;
};

#endif //CONCUTP_PROCESS_H
