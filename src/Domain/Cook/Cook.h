//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_COOK_H
#define CONCUTP_COOK_H


#include <string>
#include "../Pizza/Pizza.h"
#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"

class Cook {
public:
    Cook(Pipe orderChannel);


private:
    Pipe processedOrdersChannel;

    void startPollingForOrders();

    void cookOrder(string& orderStr);
};


#endif //CONCUTP_COOK_H
