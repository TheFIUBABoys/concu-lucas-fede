//
// Created by lucas on 4/20/15.
//

#ifndef CONCUTP_RECEPTIONIST_H
#define CONCUTP_RECEPTIONIST_H


#include "../../Util/Pipes/Pipe.h"
#include <string>


class Receptionist {

public:
    Receptionist(Pipe &theOrderChannel, Pipe &theProcessedOrdersChannel);

private:
    Pipe orderChannel, processedOrdersChannel;

    Receptionist() { }

    void startPollingForOrders();

    void processOrder(std::string &orderStr);
};


#endif //CONCUTP_RECEPTIONIST_H
