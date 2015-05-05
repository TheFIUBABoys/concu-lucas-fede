//
// Created by fede on 25/04/15.
//

#include <iostream>
#include "Supervisor.h"
#include "../../Util/Logger/Logger.h"
#include "../../Util/Seniales/SignalHandler.h"

//Create receptionist in new thread and start polling for orders
Supervisor::Supervisor() {
    Logger::logger().log("Supervisor waking up");

    payDesk.crear(LOCKFILE_PAYDESK, 'L');

    startCheckingCash();

    payDesk.liberar();

    Logger::logger().log("Supervisor dying");
}

void Supervisor::startCheckingCash() {
    float previous = -100, i=0;
    while(!sigint_handler.getGracefulQuit()) {
        sleep(CHECK_DELAY);
        payDeskLock.tomarLockWr();
        Logger::logger().log("Supervisor tomo lock de caja");
        float current = payDesk.leer();
        Logger::logger().log("Supervisor lee recaudacion de " + to_string(current));
        payDeskLock.liberarLock();
        Logger::logger().log("Supervisor libero lock de caja");
        if (previous==current){
            i++;
        }else{
            i=0;
        }
        if (i>=TIMEOUT) break; //Timeout
        previous = current;
    }
    //Graceful quit
    Logger::logger().log("Graceful quit Supervisor");
    payDesk.liberar();
    SignalHandler::destruir();
}