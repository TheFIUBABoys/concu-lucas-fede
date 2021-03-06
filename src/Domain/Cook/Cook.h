//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_COOK_H
#define CONCUTP_COOK_H


#include <string>
#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"
#include "../../Util/Fifos/FifoLectura.h"
#include "../Receptionist/Receptionist.h"
#include "../../Util/Semaforo/Semaforo.h"
#include "../Process/Process.h"

class Cook : Process {
public:
    Cook();
    static string getPizzaFifoName();
private:

    FifoLectura processedOrdersChannel = FifoLectura(Receptionist::getProcessedOrderFifoName());
    FifoEscritura pizzaChannel = FifoEscritura(Cook::getPizzaFifoName());
    MemoriaCompartida<int> processedOrderAmount;
    LockFile processedOrderAmountLock = LockFile(LOCKFILE_HANDLED_ORDERS);
    void startPollingForOrders();

    void cookOrder(string& orderStr);

    Semaforo freeOvenSemaphore = Semaforo(SEMAPHORE_NAME);


};


#endif //CONCUTP_COOK_H
