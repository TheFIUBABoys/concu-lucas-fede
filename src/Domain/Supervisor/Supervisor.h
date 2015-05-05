//
// Created by fede on 25/04/15.
//

#ifndef CONCUTP_SUPERVISOR_H
#define CONCUTP_SUPERVISOR_H

#define CHECK_DELAY 5    // s

#define TIMEOUT 5 //Attempts

#include "../../Util/Pipes/Pipe.h"
#include "../../Util/MemoriaCompartida/MemoriaCompartida.h"
#include "../../Util/Locks/LockFile.h"
#include "../../Config/Config.h"
#include "../../Util/Seniales/SIGINT_Handler.h"

class Supervisor {
public:
    Supervisor();
private:
    MemoriaCompartida<float> payDesk;
    LockFile payDeskLock = LockFile(LOCKFILE_PAYDESK);
    void startCheckingCash();

    SIGINT_Handler sigint_handler;
};

#endif //CONCUTP_SUPERVISOR_H
