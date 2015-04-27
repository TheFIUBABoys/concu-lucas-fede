//
// Created by fede on 25/04/15.
//

#include "Cadet.h"
#include "../../Config/Config.h"
#include "../../Util/Logger/Logger.h"

//Create receptionist in new thread and start polling for orders
Cadet::Cadet() {
    Logger::logger().log("Cadet waking up");
    cookedPizzaChannel.abrir();
    startPollingForOrders();
    Logger::logger().log("Cadet diying");
}

void Cadet::startPollingForOrders() {

    char buffer[MESSAGE_LENGTH];
    while (true) {
        ssize_t bytesLeidos = cookedPizzaChannel.leer(buffer, MESSAGE_LENGTH);
        if (bytesLeidos > 0) {
            std::string orderStr = buffer;
            orderStr.resize(MESSAGE_LENGTH);
            Logger::logger().log(string("Cadeta recibe ") + orderStr);
            chargePizza(orderStr);
        } else {
            Logger::logger().log("Cadeta lee EOF");
            cookedPizzaChannel.cerrar();
            break;
        }
    }
}

void Cadet::chargePizza(string &orderStr) {
    Logger::logger().log("Cadeta cobra " + orderStr);
}

std::string Cadet::getPizzaCookedFifoName() {
    return "PizzaCookedFifo";
}