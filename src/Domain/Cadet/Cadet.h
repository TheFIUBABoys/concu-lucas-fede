//
// Created by fede on 25/04/15.
//

#ifndef CONCUTP_CADET_H
#define CONCUTP_CADET_H

#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"
#include "../Cook/Cook.h"
#include "../../Util/MemoriaCompartida/MemoriaCompartida.h"
#include "../../Util/Locks/LockFile.h"
#include "../../Config/Config.h"
#include "../Process/Process.h"
#include <string>

class Cadet : Process {
public:
    Cadet();
    static std::string getPizzaCookedFifoName();
private:
    void startPollingForOrders();
    void chargePizza(string &orderStr);
    MemoriaCompartida<float> payDesk;
    LockFile payDeskLock = LockFile(LOCKFILE_PAYDESK);
    FifoLectura cookedPizzaChannel = FifoLectura(Cadet::getPizzaCookedFifoName());

    Semaforo freeOvenSemaphore = Semaforo(SEMAPHORE_NAME);
    float getPizzaPrice();

    string takePizzaFromOven(char buffer[]);
};

#endif //CONCUTP_CADET_H
