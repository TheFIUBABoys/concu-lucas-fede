//
// Created by fede on 25/04/15.
//

#include <random>
#include "Cadet.h"
#include "../../Config/Config.h"
#include "../../Util/Logger/Logger.h"
#include "../../Util/Seniales/SignalHandler.h"

#define REGISTER_USE_DELAY 3
//Create receptionist in new thread and start polling for orders
Cadet::Cadet() : Process() {
    Logger::logger().log("Cadet waking up");
    if (payDesk.crear(LOCKFILE_PAYDESK, 'L')!=SHM_OK){
        string error = "Error creando caja de memoria compartida";
        perror(error.c_str());
        Logger::logger().log(error);
    }
    payDesk.escribir(0.0);
    cookedPizzaChannel.abrir();

    startPollingForOrders();

    payDesk.liberar();
    Logger::logger().log("Cadet dying");
}

void Cadet::startPollingForOrders() {

    char buffer[MESSAGE_LENGTH];
    while (!sigint_handler.getGracefulQuit()) {
        ssize_t bytesLeidos = cookedPizzaChannel.leer(buffer, MESSAGE_LENGTH);
        if (bytesLeidos > 0) {
            string orderStr = takePizzaFromOven(buffer);
            chargePizza(orderStr);
        } else {
            Logger::logger().log("Cadeta lee EOF");
            cookedPizzaChannel.cerrar();
            break;
        }
    }
    //Graceful quit
    Logger::logger().log("Graceful quit Cadet");
    cookedPizzaChannel.cerrar();
    payDesk.liberar();
    SignalHandler::destruir();
}

string Cadet::takePizzaFromOven(char buffer[]) {
    string orderStr = buffer;
    orderStr.resize(MESSAGE_LENGTH);
    Logger::logger().log(string("Cadeta recibe ") + orderStr);

    //Signal there's an available oven
    if (freeOvenSemaphore.v() < 0) {
        string err = "Error incrementando semaforo";
        perror(err.c_str());
        Logger::logger().log(err);
    }

    return orderStr;
}

float Cadet::getPizzaPrice(){
    return 50.0;
}

void Cadet::chargePizza(string &orderStr) {
    Logger::logger().log("Cadeta llevo " + orderStr);
    payDeskLock.tomarLockWr();
    float current = payDesk.leer();
    Logger::logger().log("Cadeta tomo lock de caja, que tiene $" +  to_string(current));
    payDesk.escribir(current + getPizzaPrice());
    sleep(REGISTER_USE_DELAY); //Para validar que efectivamente se este lockeando
    payDeskLock.liberarLock();
    Logger::logger().log("Cadeta cobro " + orderStr + " total caja: " + to_string(current + getPizzaPrice()));
}

std::string Cadet::getPizzaCookedFifoName() {
    return "PizzaCookedFifo";
}

