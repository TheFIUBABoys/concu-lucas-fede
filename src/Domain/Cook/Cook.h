//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_COOK_H
#define CONCUTP_COOK_H


#include "../Pizza/Pizza.h"
#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"

class Cook {
public:
    Cook(Pipe orderChannel);
    Pizza cookOrder(Order order);

private:
    Pipe processedOrdersChannel;

    void startPollingForOrders();
};


#endif //CONCUTP_COOK_H
