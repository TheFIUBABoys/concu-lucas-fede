//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_OVEN_H
#define CONCUTP_OVEN_H


#include <bits/basic_string.h>
#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"
#include "../../Util/Fifos/FifoLectura.h"
#include "../Cook/Cook.h"
#include "../Cadet/Cadet.h"
#include "../../Util/Semaforo/Semaforo.h"
#include "../Process/Process.h"


class Oven : Process {
public:
    Oven();

    void startWaitingForPizzas();

private:
    FifoLectura pizzaChannel = FifoLectura(Cook::getPizzaFifoName());
    FifoEscritura cookedPizzaChannel = FifoEscritura(Cadet::getPizzaCookedFifoName());

    Semaforo freeOvenSemaphore = Semaforo(SEMAPHORE_NAME);
    void cookPizza(string pizza);

};


#endif //CONCUTP_OVEN_H
