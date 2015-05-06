//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_COOK_H
#define CONCUTP_COOK_H


#include <string>
#include "../Pizza/Pizza.h"
#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"
#include "../../Util/Fifos/FifoLectura.h"
#include "../Receptionist/Receptionist.h"
#include "../../Util/Semaforo/Semaforo.h"

class Cook {
public:
    Cook();
    static string getPizzaFifoName();
private:

    FifoLectura processedOrdersChannel = FifoLectura(Receptionist::getProcessedOrderFifoName());
    FifoEscritura pizzaChannel = FifoEscritura(Cook::getPizzaFifoName());
    MemoriaCompartida<int> processedOrderAmount;
    LockFile processedOrderAmountLock = LockFile(LOCKFILE_HANDLED_ORDERS);
    void startPollingForOrders();

    SIGINT_Handler sigint_handler;

    void cookOrder(string& orderStr);

    Semaforo freeOvenSemaphore = Semaforo("Oven");


};


#endif //CONCUTP_COOK_H
