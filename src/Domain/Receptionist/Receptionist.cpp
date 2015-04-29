//
// Created by lucas on 4/20/15.
//

#include "Receptionist.h"
#include "../../Util/Logger/Logger.h"
#include "../../Config/Config.h"


Receptionist::Receptionist(int cookAmount) {
    Logger::logger().log("Receptionist waking up");
    this->cookAmount = cookAmount;
    orderChannel.abrir();
    processedOrderAmount.crear(CONFIG_FILE2, 'L');
    processedOrderChannel.abrir();
    startPollingForOrders();
    Logger::logger().log("Receptionist dying");
}

void Receptionist::startPollingForOrders() {
    char buffer[MESSAGE_LENGTH];
    while (true) {
        int uncookedOrderAmount = processedOrderAmount.leer();
        ssize_t readBytes = orderChannel.leer(buffer, MESSAGE_LENGTH);

        while (uncookedOrderAmount > 2 * cookAmount && readBytes > 0) {
            if (readBytes > 0) {
                Logger::logger().log(string("Recepcionista no atiende ") + buffer);
            }
            uncookedOrderAmount = processedOrderAmount.leer();
            //Ignore orders
            readBytes = orderChannel.leer(buffer, MESSAGE_LENGTH);
        }
        if (readBytes > 0) {
            std::string orderStr = buffer;
            orderStr.resize(MESSAGE_LENGTH);
            Logger::logger().log(string("Recepcionista recibe ") + string(orderStr));
            processOrder(orderStr);
        } else {
            Logger::logger().log("Recepcionista lee EOF");
            processedOrderChannel.cerrar();
            orderChannel.cerrar();
            processedOrderAmount.liberar();
            break;
        }
    }

}

void Receptionist::processOrder(string &orderStr) {
    string processedOrder = string("Recepcionista procesa ") + orderStr;
    Logger::logger().log(processedOrder);
    processedOrder.resize(MESSAGE_LENGTH);
    if (int written = processedOrderChannel.escribir(orderStr.c_str(), MESSAGE_LENGTH) != MESSAGE_LENGTH) {
        Logger::logger().log(string("Error al escribir procesada") + to_string(written));
        perror("Proccessed pipe");
    } else {
        Logger::logger().log("Cant pedidos tomados: " + to_string(processedOrderAmount.leer()));
        processedOrderAmountLock.tomarLock();
        processedOrderAmount.escribir(processedOrderAmount.leer() + 1);
        processedOrderAmountLock.liberarLock();
    }
}

std::string Receptionist::getOrderFifoName() {
    return "OrderFifo";
}

std::string Receptionist::getProcessedOrderFifoName() {
    return "ProcessedOrderFifo";
}
