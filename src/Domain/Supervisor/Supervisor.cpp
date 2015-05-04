//
// Created by fede on 25/04/15.
//

#include <iostream>
#include "Supervisor.h"
#include "../../Util/Logger/Logger.h"

//Create receptionist in new thread and start polling for orders
Supervisor::Supervisor() {
    Logger::logger().log("Supervisor waking up");

    payDesk.crear(CONFIG_FILE, 'L');

    startCheckingCash();

    payDesk.liberar();

    Logger::logger().log("Supervisor dying");
}

void Supervisor::startCheckingCash() {
    while(true) {
        sleep(CHECK_DELAY);

        Logger::logger().log("Supervisor tomo lock " +  to_string(payDeskLock.tomarLockRd()));
        float current = payDesk.leer();
        Logger::logger().log("Supervisor lee recaudacion de " + to_string(current));
        payDeskLock.liberarLock();
    }
}