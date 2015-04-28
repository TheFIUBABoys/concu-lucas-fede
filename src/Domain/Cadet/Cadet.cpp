//
// Created by fede on 25/04/15.
//

#include <random>
#include "Cadet.h"
#include "../../Config/Config.h"
#include "../../Util/Logger/Logger.h"

//Create receptionist in new thread and start polling for orders
Cadet::Cadet() {
    Logger::logger().log("Cadet waking up");
    payDesk.crear(CONFIG_FILE, 'L');
    cookedPizzaChannel.abrir();

    startPollingForOrders();

    payDesk.liberar();
    Logger::logger().log("Cadet dying");
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

float Cadet::getPizzaPrice(){
    return 50.0;
}

void Cadet::chargePizza(string &orderStr) {
    Logger::logger().log("Cadeta llevo " + orderStr);

    Logger::logger().log("Cadeta tomo lock " +  to_string(payDeskLock.tomarLock()));
    float current = payDesk.leer();
    payDesk.escribir(current + getPizzaPrice());
    sleep(3); //Para validar que efectivamente se este lockeando
    payDeskLock.liberarLock();
    Logger::logger().log("Cadeta cobro " + orderStr + " total caja: " + to_string(current + getPizzaPrice()));
}

std::string Cadet::getPizzaCookedFifoName() {
    return "PizzaCookedFifo";
}

