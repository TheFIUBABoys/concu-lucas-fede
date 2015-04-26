//
// Created by lucas on 4/20/15.
//

#ifndef CONCUTP_RECEPTIONIST_H
#define CONCUTP_RECEPTIONIST_H


#include "../../Util/Pipes/Pipe.h"
#include "../../Util/Fifos/FifoLectura.h"
#include "../../Util/Fifos/FifoEscritura.h"
#include <string>


class Receptionist {

public:
    Receptionist();
    static std::string getOrderFifoName();
    static std::string getProcessedOrderFifoName();
private:
    FifoLectura orderChannel = FifoLectura(Receptionist::getOrderFifoName());
    FifoEscritura processedOrderChannel = FifoEscritura(Receptionist::getProcessedOrderFifoName());

    void startPollingForOrders();

    void processOrder(std::string &orderStr);

};


#endif //CONCUTP_RECEPTIONIST_H
