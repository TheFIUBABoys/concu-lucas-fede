//
// Created by lucas on 4/20/15.
//

#include "Receptionist.h"
#include "../../Util/Logger/Logger.h"
#include "../../Config/Config.h"


//Create receptionist in new thread and start polling for orders
Receptionist::Receptionist(Pipe &theOrderChannel, Pipe &theProcessedOrdersChannel) {
    Logger::logger().log("Receptionist waking up");
    orderChannel = theOrderChannel;
    processedOrdersChannel = theProcessedOrdersChannel;
    startPollingForOrders();
    Logger::logger().log("Receptionist dying");
}

void Receptionist::startPollingForOrders() {
    char buffer[MESSAGE_LENGTH];
    int timeout = 0;
    while (true) {
        ssize_t bytesLeidos = orderChannel.leer(buffer, MESSAGE_LENGTH);
        std::string orderStr = buffer;
        orderStr.resize(MESSAGE_LENGTH);

        if (bytesLeidos > 0) {
            Logger::logger().log(string("Lei esto: ") + string(orderStr));
            processOrder(orderStr);
        } else {
            Logger::logger().log("Lei EOF");
            timeout++;
        }
        if (timeout > 3) break;
        sleep(3);
    }

}

void Receptionist::processOrder(string &orderStr) {
    sleep(3); //Procesar
    string processedOrder = string("Procesada ") + string(orderStr);
    Logger::logger().log(processedOrder);
    processedOrder.resize(MESSAGE_LENGTH);
    processedOrdersChannel.escribir(processedOrder.c_str(), MESSAGE_LENGTH);
}
