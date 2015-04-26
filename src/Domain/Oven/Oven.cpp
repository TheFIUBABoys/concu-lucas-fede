//
// Created by lucas on 4/22/15.
//

#include <random>
#include "Oven.h"
#include "../../Util/Logger/Logger.h"
#include "../../Config/Config.h"

Pizza Oven::cookOrder(Order order) {
    return Pizza();
}

Oven::Oven() {
    Logger::logger().log("Preheating oven");
    pizzaChannel.abrir();
    startWaitingForPizzas();
    Logger::logger().log("Oven shutting down");
}

void Oven::startWaitingForPizzas() {
    char buffer[MESSAGE_LENGTH];;
    while (true) {
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
}

void Oven::cookPizza(string pizzaStr) {
    Logger::logger().log(string("Cocinando pizza: ") + pizzaStr);
    default_random_engine generator;
    uniform_int_distribution<int> distribution(1, 6);
    sleep((unsigned int) distribution(generator));
    Logger::logger().log(string("Termino de cocinar pizza: ") + pizzaStr);
}
