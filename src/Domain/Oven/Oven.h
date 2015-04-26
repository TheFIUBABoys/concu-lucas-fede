//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_OVEN_H
#define CONCUTP_OVEN_H


#include <bits/basic_string.h>
#include "../Pizza/Pizza.h"
#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"
#include "../../Util/Fifos/FifoLectura.h"
#include "../Cook/Cook.h"

class Oven {
public:
    Oven();
    Pizza cookOrder(Order order);

    void startWaitingForPizzas();
private:
    FifoLectura pizzaChannel = FifoLectura(Cook::getPizzaFifoName());

    void cookPizza(string pizza);
};


#endif //CONCUTP_OVEN_H
