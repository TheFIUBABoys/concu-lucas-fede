//
// Created by lucas on 4/20/15.
//

#ifndef CONCUTP_RECEPTIONIST_H
#define CONCUTP_RECEPTIONIST_H

#include <string>
#include "../../Util/Pipes/Pipe.h"
#include "../../Util/Fifos/FifoLectura.h"
#include "../../Util/Fifos/FifoEscritura.h"
#include "../../Util/MemoriaCompartida/MemoriaCompartida.h"
#include "../../Util/Locks/LockFile.h"
#include "../../Config/Config.h"


class Receptionist {

public:
    Receptionist(int cookAmount);
    static std::string getOrderFifoName();
    static std::string getProcessedOrderFifoName();
private:
    FifoLectura orderChannel = FifoLectura(Receptionist::getOrderFifoName());
    FifoEscritura processedOrderChannel = FifoEscritura(Receptionist::getProcessedOrderFifoName());
    MemoriaCompartida<int> processedOrderAmount;
    int cookAmount;
    LockFile processedOrderAmountLock = LockFile(CONFIG_FILE2);
    void startPollingForOrders();

    void processOrder(std::string &orderStr);

};


#endif //CONCUTP_RECEPTIONIST_H
