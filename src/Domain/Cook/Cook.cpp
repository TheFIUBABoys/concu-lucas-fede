//
// Created by lucas on 4/22/15.
//

#include <unistd.h>
#include "Cook.h"
#include "../../Util/Logger/Logger.h"
#include "../../Util/Seniales/SignalHandler.h"


#define COOK_DELAY 5

//Create receptionist in new thread and start polling for orders
Cook::Cook() : Process() {
    Logger::logger().log("Cook waking up");
    processedOrdersChannel.abrir();
    pizzaChannel.abrir();
    if (processedOrderAmount.crear(LOCKFILE_HANDLED_ORDERS, 'L') != SHM_OK) {
        string error = "Error creando caja de memoria compartida";
        perror(error.c_str());
        Logger::logger().log(error);
    }
    startPollingForOrders();
    Logger::logger().log("Cook dying");
}

void Cook::startPollingForOrders() {

    char buffer[MESSAGE_LENGTH];
    while (!sigint_handler.getGracefulQuit()) {
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
    //Graceful quit
    Logger::logger().log("Graceful quit Cook");
    processedOrdersChannel.cerrar();
    pizzaChannel.cerrar();
    processedOrderAmount.liberar();
    SignalHandler::destruir();
}

void Cook::cookOrder(string &orderStr) {
    string processedOrder = string("Cocinera cocina ") + orderStr;
    Logger::logger().log(processedOrder);
    sleep(COOK_DELAY); // Cook
    if (!sigint_handler.getGracefulQuit()) {
        //Wait for free oven
        if (freeOvenSemaphore.p() < 0) {
            string err = "Error decrementando semaforo";
            perror(err.c_str());
            Logger::logger().log(err);
        }
        Logger::logger().log(string("Cocinera encontro horno libre para orden: ") + orderStr);

        processedOrder.resize(MESSAGE_LENGTH);
        if (int written = pizzaChannel.escribir(orderStr.c_str(), (int const) orderStr.size()) != MESSAGE_LENGTH) {
            Logger::logger().log(string("Error al escribir procesada") + to_string(written));
            perror("Proccessed pipe");
        } else {
            processedOrderAmountLock.tomarLockWr();
            processedOrderAmount.escribir(processedOrderAmount.leer() - 1);
            processedOrderAmountLock.liberarLock();
        }
    }
}

std::string Cook::getPizzaFifoName() {
    return "PizzaFifo";
}