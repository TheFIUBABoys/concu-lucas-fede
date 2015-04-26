//
// Created by lucas on 4/20/15.
//

#include "Receptionist.h"
#include "../../Util/Logger/Logger.h"
#include "../../Config/Config.h"


Receptionist::Receptionist(Pipe &theProcessedOrdersChannel) {
    Logger::logger().log("Receptionist waking up");
    orderChannel.abrir();
    processedOrdersChannel = theProcessedOrdersChannel;
    startPollingForOrders();
    Logger::logger().log("Receptionist dying");
}

void Receptionist::startPollingForOrders() {
    char buffer[MESSAGE_LENGTH];
    while (true) {
        ssize_t bytesLeidos = orderChannel.leer(buffer, MESSAGE_LENGTH);
        std::string orderStr = buffer;
        orderStr.resize(MESSAGE_LENGTH);

        if (bytesLeidos > 0) {
            Logger::logger().log(string("Recepcionista recibe ") + string(orderStr));
            processOrder(orderStr);
        } else {
            Logger::logger().log("Lei EOF");
            processedOrdersChannel.cerrar();
            break;
        }
    }

}

void Receptionist::processOrder(string &orderStr) {
    string processedOrder = string("Recepcionista procesa ") + orderStr;
    Logger::logger().log(processedOrder);
    processedOrder.resize(MESSAGE_LENGTH);
    if(int written = processedOrdersChannel.escribir(orderStr.c_str(), MESSAGE_LENGTH)!=MESSAGE_LENGTH){
        Logger::logger().log(string("Error al escribir procesada")+to_string(written));
        perror("Proccessed pipe");
    }
}

std::string Receptionist::getFifoName() {
    return "OrderFifo";
}
