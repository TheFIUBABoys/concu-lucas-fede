//
// Created by lucas on 4/22/15.
//

#include <random>
#include "Oven.h"
#include "../../Util/Logger/Logger.h"
#include "../../Config/Config.h"
#include "../../Util/Seniales/SignalHandler.h"

Oven::Oven() {
    Logger::logger().log("Preheating oven");
    SignalHandler::getInstance()->registrarHandler ( SIGINT,&sigint_handler);
    pizzaChannel.abrir();
    cookedPizzaChannel.abrir();
    freeOvenSemaphore.v();
    startWaitingForPizzas();
    Logger::logger().log("Oven shutting down");
}

void Oven::startWaitingForPizzas() {
    char buffer[MESSAGE_LENGTH];;
    while (!sigint_handler.getGracefulQuit()) {
        ssize_t bytesLeidos = pizzaChannel.leer(buffer, MESSAGE_LENGTH);
        string pizzaStr = buffer;
        pizzaStr.resize(MESSAGE_LENGTH);
        if (bytesLeidos > 0) {
            cookPizza(pizzaStr);
        } else {
            Logger::logger().log("Horno lee EOF");
            pizzaChannel.cerrar();
            break;
        }
    }
    //Graceful quit
    Logger::logger().log("Graceful quit Oven");
    pizzaChannel.cerrar();
    cookedPizzaChannel.cerrar();
    SignalHandler::destruir();
}


void Oven::cookPizza(string pizzaStr) {
    Logger::logger().log(string("Horno cocinando pizza: ") + pizzaStr);
    default_random_engine generator;
    uniform_int_distribution<int> distribution(1, 6);
    sleep((unsigned int) distribution(generator));
    Logger::logger().log(string("Horno termino de cocinar pizza: ") + pizzaStr);
    cookedPizzaChannel.escribir(pizzaStr.c_str(), (int const) pizzaStr.size());
}
