//
// Created by lucas on 4/20/15.
//

#ifndef CONCUTP_RECEPTIONIST_H
#define CONCUTP_RECEPTIONIST_H


#include "../../Util/Pipes/Pipe.h"
#include "../../Util/Fifos/FifoLectura.h"
#include <string>


class Receptionist {

public:
    Receptionist(Pipe &theProcessedOrdersChannel);
    static std::string getFifoName();
private:
    FifoLectura orderChannel = FifoLectura(Receptionist::getFifoName());;
    Pipe processedOrdersChannel;

    void startPollingForOrders();

    void processOrder(std::string &orderStr);
};


#endif //CONCUTP_RECEPTIONIST_H
