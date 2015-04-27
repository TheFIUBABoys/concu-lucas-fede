//
// Created by fede on 25/04/15.
//

#ifndef CONCUTP_CADET_H
#define CONCUTP_CADET_H

#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"
#include "../Cook/Cook.h"
#include <string>

class Cadet {
public:
    Cadet();
    static std::string getPizzaCookedFifoName();
private:
    void startPollingForOrders();
    void chargePizza(string &orderStr);
    FifoLectura cookedPizzaChannel = FifoLectura(Cadet::getPizzaCookedFifoName());
};

#endif //CONCUTP_CADET_H
