//
// Created by lucas on 4/22/15.
//

#include <unistd.h>
#include "Cook.h"
#include "../../Util/Logger/Logger.h"
#include "../../Config/Config.h"

//Create receptionist in new thread and start polling for orders
Cook::Cook() {
    Logger::logger().log("Cook waking up");
    processedOrdersChannel.abrir();
    pizzaChannel.abrir();
    startPollingForOrders();
    Logger::logger().log("Cook dying");
}

void Cook::startPollingForOrders() {

    char buffer[MESSAGE_LENGTH];
    while (true) {
        ssize_t bytesLeidos = processedOrdersChannel.leer(buffer, MESSAGE_LENGTH);
        if (bytesLeidos > 0) {
            std::string orderStr = buffer;
            orderStr.resize(MESSAGE_LENGTH);
            Logger::logger().log(string("Cocinera recibe ") + orderStr);
            cookOrder(orderStr);
        } else {
            Logger::logger().log("Cocinera lee EOF");
            processedOrdersChannel.cerrar();
            pizzaChannel.cerrar();
            break;
        }
    }
}

void Cook::cookOrder(string &orderStr) {
    string processedOrder = string("Cocinera cocina ") + orderStr;
    Logger::logger().log(processedOrder);
    processedOrder.resize(MESSAGE_LENGTH);
    pizzaChannel.escribir(orderStr.c_str(), (int const) orderStr.size());
}

std::string Cook::getPizzaFifoName() {
    return "PizzaFifo";
}